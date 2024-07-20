#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>
#include <time.h>
#include <curl/curl.h>
#include <stdarg.h>
#define LOCAL_VERSION "1.4.3"
#define MAX_WORDS 5 
#define GREEN "\033[0;32m"
#define ORANGE "\033[0;33m"
#define NC "\033[0m"

void show_menu();
int check_docker_installed();
int check_docker_compose_installed(); 
int is_container_built(const char *container_name);
int is_container_running(const char *container_name);
int is_directory(const char *path);
int is_user_in_group(const char *username, const char *groupname);
void create_directory(const char *path);
void create_file(const char *path, const char *content);
void install_docker();
void rm_docker();
void install_twdash();
void install_twdash_mysql();
int install_twdash_php();
int install_ssl_ca();
int install_ssl(const char *domain_name); 
void network_create(const char *NETWORK_NAME);
void network_rm(const char *NETWORK_NAME);
int list_alias();
void alias_create(const char *container_alias);
void alias_rm(const char *container_alias);
void stop_container(const char *CONTAINER_NAME,int OPTION);
void rm_container(const char *container_name);
char* get_container_ip(const char *container_name);
void get_container_logs(const char *container_name);
void enter_container(const char *container_name);
void twtree();
void rmf(const char *container_alias);
void rmi(const char *container_name);
int is_image(const char *image_name_id);
void upload_db_backup(const char *db_name, const char *file_path);
void db_backup(const char *db_name);
void db_enter();
void db_enter_table(const char *db_name);
void db_table_backup(const char *db_name, const char *table_name);
void nginx_php_website(char *dn);
int start_container(const char *container_name);
int restart_container(const char *container_name);
int is_name_present(const char *name);
int is_ip_present(const char *ip_address);
int is_ip_and_name_present(const char *ip_address, const char *name);
void prepareHostsFile();
int hostsupdate(char *action);
int strsize(char *str);
char* strjoin(char *input, ...);
void* print(char *input, ...);
void nginx_wpwebsite(char *dn);
void nginx_html_website(char* dn);
void install_phpmyadmin_website();

int get_year() {
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    return (localTime->tm_year + 1900) % 100;
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        char choice[100];
        char *newline;
        char *words[MAX_WORDS];
        char *token;
        int word_count;
        words[0]="";
        printf(" _               _           _       _                 _ \n| |___      ____| | __ _ ___| |__   | | ___   ___ __ _| |\n| __\\ \\ /\\ / / _` |/ _` / __| '_ \\  | |/ _ \\ / __/ _` | |\n| |_ \\ V  V / (_| | (_| \\__ \\ | | | | | (_) | (_| (_| | |\n \\__| \\_/\\_/ \\__,_|\\__,_|___/_| |_| |_|\\___/ \\___\\__,_|_|\n");
        print("\t\t\t\t\tv%s  2024-20%02d",LOCAL_VERSION,get_year());

        while (1) {
            printf("\n»» ");
            fflush(stdout);
            fgets(choice, sizeof(choice), stdin);
            word_count = 0;
            newline = strchr(choice, '\n');
            if (newline != NULL) {
                *newline = '\0';
            } else {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
            token = strtok(choice, " ");
            while (token != NULL && word_count < MAX_WORDS) {
                words[word_count] = malloc(strlen(token) + 1);
                if (words[word_count] == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(1);
                }
                strcpy(words[word_count], token);

                word_count++;
                token = strtok(NULL, " ");
            }
            if (strcmp(words[0], "mkdir") == 0){
                system(strjoin("mkdir /home/%s/twdash",getenv("USER")));
                print("[%s OK %s] main directory twdash created",GREEN,NC);
            } 
            if (strcmp(words[0], "exit") == 0) {
                print("Exiting twdash local v%s",LOCAL_VERSION);
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                exit(0);
            } else if (strcmp(words[0], "docker") == 0){
                if(word_count == 2){
                    if (strcmp(words[1], "install") == 0)
                        install_docker();
                    else if (strcmp(words[1], "rm") == 0)
                        rm_docker();
                    else
                        print("%susage: docker ( install | rm )%s",ORANGE,NC);
                } else {
                    print("%susage: docker ( install | rm )%s",ORANGE,NC);
                }
            } else if (strcmp(words[0], "install") == 0) {
                if (word_count>=2 && strcmp(words[1], "main") == 0){
                    if(word_count==2)
                        install_twdash();
                    else
                        print("%susage: install main%s",ORANGE,NC);
                } else if (word_count>=2 && strcmp(words[1], "phpmyadmin") == 0) {
                    if(word_count==2)
                        install_phpmyadmin_website();
                    else
                        print("%susage: install phpmyadmin%s",ORANGE,NC);
                } else if (word_count>=2 && strcmp(words[1], "htmlwebsite") == 0) {
                    if(word_count==3)
                        nginx_html_website(words[2]);
                    else
                        print("%susage: install htmlwebsite <network_name>%s",ORANGE,NC);
                } else if (word_count>=2 && strcmp(words[1], "phpwebsite") == 0) {
                    if(word_count==3)
                        nginx_php_website(words[2]);
                    else
                        print("%susage: install phpwebsite <network_name>%s",ORANGE,NC);
                } else if (word_count>=2 && strcmp(words[1], "wpwebsite") == 0) {
                    if(word_count==3)
                        nginx_wpwebsite(words[2]);
                    else
                        print("%susage: install wpwebsite <network_name>%s",ORANGE,NC);
                } else {
                    print("%susage: install ( main | htmlwebsite | phpwebsite | wpwebsite | phpmyadmin)%s",ORANGE,NC);
                }
            } else if (strcmp(words[0], "network") == 0){
                if (strcmp(words[1], "ls") == 0) {
                    if(word_count==3)
                        system("docker network ls");
                    else
                        print("%susage: network ls%s",ORANGE,NC);
                } else if (strcmp(words[1], "create") == 0){
                    if(word_count == 3)
                        network_create(words[2]);
                    else
                        print("%susage: network create <network_name>%s",ORANGE,NC);
                } else if (strcmp(words[1], "rm") == 0) {
                    if(word_count == 3)
                        network_rm(words[2]);
                    else
                        print("%susage: network rm <network_name>%s",ORANGE,NC);
                } else {
                    print("%susage: network rm <network_name>%s",ORANGE,NC);
                }
            } else if (strcmp(words[0], "alias") == 0){
                if (word_count>=2 && strcmp(words[1], "ls") == 0) {
                    if(word_count==2)
                        list_alias();
                    else
                        print("%susage: alias ls%s",ORANGE,NC);
                } else if (word_count>=2 && strcmp(words[1], "create") == 0){
                    if(word_count == 3)
                        alias_create(words[2]);
                    else
                        print("%susage: alias create <network_name>%s",ORANGE,NC);
                } else if (word_count>=2 && strcmp(words[1], "rm") == 0) {
                    if(word_count == 3)
                        alias_rm(words[2]);
                    else
                        print("%susage: alias rm <network_name>%s",ORANGE,NC);
                } else {
                    print("%susage: alias ( ls | create | rm ) <network_name>%s",ORANGE,NC);
                }
            } else if (strcmp(words[0], "ps") == 0) {
                if(word_count < 3){
                    if(word_count == 2){
                        if (strcmp(words[1], "-a") == 0)
                            system("docker ps -a");
                        else if (strcmp(words[1], "-m") == 0)
                            system("docker ps --format \"table {{.ID}}\t{{.Image}}\t{{.Status}}\t{{.Ports}}\t{{.Names}}\" | column -t -s $'\t'");
                        else if (strcmp(words[1], "-ma") == 0)
                            system("docker ps -a --format \"table {{.ID}}\t{{.Image}}\t{{.Status}}\t{{.Ports}}\t{{.Names}}\" | column -t -s $'\t'");
                        else
                            print("%susage: ps -a%s",ORANGE,NC);
                    } else {
                            system("docker ps");
                    }
                } else {
                    print("%susage: ps <-a|-m|-ma>%s",ORANGE,NC);
                }
            } else if (strcmp(words[0],"stop") == 0) {
                if(word_count==2){
                    stop_container(words[1],0);
                } else if(word_count==3 && strcmp(words[2],"--rme")==0){
                    stop_container(words[1],1);
                } else {
                    print("%susage: stop <container_name>%s",ORANGE,NC);
                }
            } else if (strcmp(words[0],"rm") == 0) {
                if(word_count==2)
                    rm_container(words[1]);
                else
                    print("%susage: rm <container_name>%s",ORANGE,NC);
            } else if (strcmp(words[0], "logs") == 0) {
                if(word_count==2)
                    get_container_logs(words[1]);
                else
                    print("%susage: logs <container_name>%s",ORANGE,NC);
            } else if (strcmp(words[0], "exec") == 0) {
                if(word_count==2)
                    enter_container(words[1]); 
                else
                    print("%susage: exec <container_name>%s",ORANGE,NC);
            } else if (strcmp(words[0], "ip") == 0){
                if(word_count==2){
                    print("%s",get_container_ip(words[1]));
                } else {
                    print("%susage: ip <container_name>%s",ORANGE,NC);
                }
            } else if (strcmp(words[0], "tree") == 0){
                if(word_count == 1)
                    twtree();
                else
                    print("%susage: tree%s",ORANGE,NC);
            } else if (strcmp(words[0], "images") == 0){
                if(word_count == 2)
                    system("docker images");
                else
                    print("%susage: images%s",ORANGE,NC);
            } else if (strcmp(words[0], "rmf") == 0){
                if(word_count==2)
                    rmf(words[1]); 
                else
                    print("%susage: rmf <container_name>%s",ORANGE,NC);
            } else if (strcmp(words[0], "rmi") == 0){
                if(word_count==2)
                    rmi(words[1]); 
                else
                    print("%susage: rmi <container_name>%s",ORANGE,NC);
            } else if (strcmp(words[0], "db") == 0){
                if(strcmp(words[1],"enter")==0){
                    if(word_count==2)
                        db_enter(); 
                    else if(word_count==3)
                        db_enter_table(words[2]);
                    else
                        print("%susage: db enter <database_name>%s",ORANGE,NC);
                } else if(strcmp(words[1],"backup")==0){
                    if(word_count==3)
                        db_backup(words[2]); 
                    else if(word_count==4)
                        db_table_backup(words[2],words[3]); 
                    else 
                        print("%susage: db backup <database_name> -<table_name>%s",ORANGE,NC);
                } else if(strcmp(words[1],"restore")==0){
                    if(word_count==4)
                        upload_db_backup(words[1],words[2]); 
                    else
                        print("%susage: db restore <database_name> <file_name>%s",ORANGE,NC);
                } else {
                    print("%susage: db ( enter | backup | restore ) <database_name> <file_name>%s",ORANGE,NC);
                }
            } else if (strcmp(words[0], "ca") == 0){
                if(word_count==1)
                    install_ssl_ca();
                else
                    print("%susage: ca%s",ORANGE,NC);
            } else if (strcmp(words[0], "ssl") == 0){
                if(word_count == 2)
                    install_ssl(words[1]);
                else
                    print("%susage: ssl <domain_name>%s",ORANGE,NC);
            } else if (strcmp(words[0], "start") == 0) {
                if(word_count==2)
                    start_container(words[1]); 
                else
                    print("%susage: restart <container_name>%s",ORANGE,NC);
            } else if (strcmp(words[0], "restart") == 0) {
                if(word_count==2)
                    restart_container(words[1]); 
                else
                    print("%susage: restart <container_name>%s",ORANGE,NC);
            } else if (strcmp(words[0], "--version") == 0  || strcmp(words[0], "-v") == 0) {
                if(word_count==1)
                    print("twdash local v%s",LOCAL_VERSION);
                else
                    print("%susage: ( -v | --version )%s",ORANGE,NC);
            } else if (strcmp(words[0], "--help") == 0 || strcmp(words[0], "-h") == 0){
                if(word_count==1)
                    show_menu();
                else
                    print("%susage: ( -h | --help )%s",ORANGE,NC);
            } else if (strcmp(words[0], "clear") == 0){
                if(word_count==1)
                    system("clear");    
                else
                    print("%susage: twd clear %s",ORANGE,NC);
            } else {
                print("%schoose an option!! \t -h for more information%s",ORANGE,NC);
            }
            for (int i = 0; i < word_count; i++) {
                free(words[i]);
            }
        }
    } else if(argc < 6){ 
        if (strcmp(argv[1], "exit") == 0) {
                print("twdash local v%s",LOCAL_VERSION);
                exit(0);
        } else if (strcmp(argv[1], "docker") == 0){
            if(argc==3){
                if (strcmp(argv[2], "install") == 0)
                    install_docker();
                else if (strcmp(argv[2], "rm") == 0)
                    rm_docker();
                else
                    print("%susage: docker ( install | rm )%s",ORANGE,NC);
            } else {
                    print("%susage: docker ( install | rm )%s",ORANGE,NC);
            }
        } else if (strcmp(argv[1], "install") == 0) {
            if (strcmp(argv[2], "main") == 0) {
                if(argc==3)
                    install_twdash();
                else
                    print("%susage: twd install main%s",ORANGE,NC);
            } else if (strcmp(argv[2], "phpmyadmin") == 0) {
                if(argc==3)
                    install_phpmyadmin_website();
                else
                    print("%susage: twd install phpmyadmin%s",ORANGE,NC);
            } else if (strcmp(argv[2], "htmlwebsite") == 0) {
                if(argc==4)
                    nginx_html_website(argv[3]);
                else
                    print("%susage: twd install htmlwebsite <domain_name>%s",ORANGE,NC);
            } else if (strcmp(argv[2], "phpwebsite") == 0) {
                if(argc==4)
                    nginx_php_website(argv[3]);
                else
                    print("%susage: twd install phpwebsite <domain_name>%s",ORANGE,NC);
            } else if (strcmp(argv[2], "wpwebsite") == 0) {
                if(argc==4)
                    nginx_wpwebsite(argv[3]);
                else
                    print("%susage: twd install wpwebsite <domain_name>%s",ORANGE,NC);
            } else {
                print("%susage: twd install ( main | htmlwebsite | phpwebsite | wpwebsite)%s",ORANGE,NC);
            }
        } else if (strcmp(argv[1], "network") == 0){
            if (strcmp(argv[2], "ls") == 0) {
                if(argc==3)
                    system("docker network ls");
                else
                    print("%susage: twd network ls%s",ORANGE,NC);
            } else if (strcmp(argv[2], "create") == 0){
                if(argc == 4)
                    network_create(argv[3]);
                else
                    print("%susage: twd network create <network_name>%s",ORANGE,NC);
            } else if (strcmp(argv[2], "rm") == 0) {
                if(argc == 4)
                    network_rm(argv[3]);
                else
                    print("%susage: twd network rm <network_name>%s",ORANGE,NC);
            } else {
                print("%susage: twd network ( ls | create | rm ) <network_name>%s",ORANGE,NC);
            }
        } else if (strcmp(argv[1], "alias") == 0){
            if (argc>=3 && strcmp(argv[2], "ls") == 0) {
                if(argc==3)
                    list_alias();
                else
                    print("%susage: twd alias ls%s",ORANGE,NC);
            } else if (argc>=3 && strcmp(argv[2], "create") == 0){
                if(argc == 4)
                    alias_create(argv[3]);
                else
                    print("%susage: twd alias create <network_name>%s",ORANGE,NC);
            } else if (argc>=3 && strcmp(argv[2], "rm") == 0) {
                if(argc == 4)
                    alias_rm(argv[3]);
                else
                    print("%susage: alias rm <network_name>%s",ORANGE,NC);
            } else {
                print("%susage: twd alias ( ls | create | rm ) <network_name>%s",ORANGE,NC);
            }
        } else if (strcmp(argv[1], "ps") == 0) {
            if(argc < 4){
                if(argc == 3){
                    if (strcmp(argv[2], "-a") == 0)
                        system("docker ps -a");
                    else if (strcmp(argv[2], "-m") == 0)
                        system("docker ps --format \"table {{.ID}}\t{{.Image}}\t{{.Status}}\t{{.Ports}}\t{{.Names}}\" | column -t -s $'\t'");
                    else if (strcmp(argv[2], "-ma") == 0)
                        system("docker ps -a --format \"table {{.ID}}\t{{.Image}}\t{{.Status}}\t{{.Ports}}\t{{.Names}}\" | column -t -s $'\t'");
                    else
                        print("%susage: twd ps -a%s",ORANGE,NC);
                } else {
                        system("docker ps");
                }
            } else {
                print("%susage: ps <-a|-m|-ma>%s",ORANGE,NC);
            }
        } else if (strcmp(argv[1], "stop") == 0) {
            if(argc==3)
                stop_container(argv[2],0);
            else
                print("%susage: twd stop <container_name>%s",ORANGE,NC);
        } else if (strcmp(argv[1], "rm") == 0) {
            if(argc==3)
                rm_container(argv[2]);
            else
                print("%susage: twd rm <container_name>%s",ORANGE,NC);
        } else if (strcmp(argv[1], "logs") == 0) {
            if(argc==3)
                get_container_logs(argv[2]);
            else
                print("%susage: twd logs <container_name>%s",ORANGE,NC);
        } else if (strcmp(argv[1], "exec") == 0) {
            if(argc==3)
                enter_container(argv[2]); 
            else
                print("%susage: twd exec <container_name>%s",ORANGE,NC);
        } else if (strcmp(argv[1], "ip") == 0) {
            if(argc==3) {
                print("%s",get_container_ip(argv[2]));
            } else {
                print("%susage: twd ip <container_name>%s",ORANGE,NC);
            }
        } else if (strcmp(argv[1], "tree") == 0) {
            if(argc == 2)
                twtree();
            else
                print("%susage: twd tree%s",ORANGE,NC);
        } else if (strcmp(argv[1], "images") == 0) {
            if(argc == 2)
                system("docker images");
            else
                print("%susage: twd images%s",ORANGE,NC);
        } else if (strcmp(argv[1], "rmf") == 0) {
            if(argc==3)
                rmf(argv[2]); 
            else
                print("%susage: twd rmf <container_name>%s",ORANGE,NC);
        } else if (strcmp(argv[1], "rmi") == 0) {
            if(argc==3)
                rmi(argv[2]); 
            else
                print("%susage: twd rmi <container_name>%s",ORANGE,NC);
        } else if (strcmp(argv[1], "db") == 0){
            if(strcmp(argv[2],"enter")==0){
                if(argc==3)
                    db_enter(); 
                else if(argc==4)
                    db_enter_table(argv[3]);
                else
                    print("%susage: twd db enter <database_name>%s",ORANGE,NC);
            } else if(strcmp(argv[2],"backup")==0){
                if(argc==4)
                    db_backup(argv[3]); 
                else if(argc==5)
                    db_table_backup(argv[3],argv[4]); 
                else 
                    print("%susage: twd db backup <database_name> -<table_name>%s",ORANGE,NC);
            } else if(strcmp(argv[2],"restore")==0){
                if(argc==5)
                    upload_db_backup(argv[3],argv[4]); 
                else
                    print("%susage: twd db restore <database_name> <file_name>%s",ORANGE,NC);
            } else {
                print("%susage: twd db ( enter | backup | restore ) <database_name> <file_name>%s",ORANGE,NC);
            }
            
        } else if (strcmp(argv[1], "ca") == 0) {
            if(argc==2)
                install_ssl_ca();
            else
                print("%susage: twd ca%s",ORANGE,NC);
        } else if (strcmp(argv[1], "ssl") == 0){
            if(argc == 3)
                install_ssl(argv[2]);
            else
                print("%susage: twd ssl <domain_name>%s",ORANGE,NC);
        } else if (strcmp(argv[1], "start") == 0) {
            if(argc==3)
                start_container(argv[2]); 
            else
                print("%susage: twd start <container_name>%s",ORANGE,NC);
        } else if (strcmp(argv[1], "restart") == 0) {
            if(argc==3)
                restart_container(argv[2]); 
            else
                print("%susage: twd restart <container_name>%s",ORANGE,NC);
        } else if (strcmp(argv[1], "--version") == 0  || strcmp(argv[1], "-v") == 0) {
            if(argc==2)
                print("twdash local v%s",LOCAL_VERSION);
            else
                print("%susage: twd ( -v | --version )%s",ORANGE,NC);
        } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0){
            if(argc==2)
                show_menu();
            else
                print("%susage: twd ( -h | --help )%s",ORANGE,NC);
        } else if (strcmp(argv[1], "clear") == 0){
            if(argc==2)
                system("clear");    
            else
                print("%susage: twd clear %s",ORANGE,NC);
        } else {
            print("%schoose an option!! \t -h for more information%s",ORANGE,NC);
        }
    } else {
        print("%sthe number of argumens seem to high, use --help for more information%s",GREEN,NC);
    }

    return 0;
}

const char *phpmyadminconf = "<VirtualHost *:80>\n\
        ServerAdmin webmaster@localhost\n\
        DocumentRoot /var/www/html\n\
\n\
        ErrorLog ${APACHE_LOG_DIR}/error.log\n\
        CustomLog ${APACHE_LOG_DIR}/access.log combined\n\
</VirtualHost>";
const char *phpmyadminconf2 = "<VirtualHost *:443>\n\
    ServerAdmin webmaster@db.local\n\
    ServerName db.local\n\
    ServerAlias www.db.local\n\
\n\
    DocumentRoot /var/www/html\n\
\n\
    SSLEngine on\n\
    SSLCertificateFile /etc/twdssl/db.local/fullchain.pem\n\
    SSLCertificateKeyFile /etc/twdssl/db.local/cert-key.pem\n\
\n\
    <Directory /var/www/html>\n\
        Options Indexes FollowSymLinks\n\
        AllowOverride All\n\
        Require all granted\n\
    </Directory>\n\
\n\
    ErrorLog ${APACHE_LOG_DIR}/db.local_error.log\n\
    CustomLog ${APACHE_LOG_DIR}/db.local_access.log combined\n\
</VirtualHost>\n\
\n\
<VirtualHost *:80>\n\
        ServerAdmin webmaster@localhost\n\
        DocumentRoot /var/www/html\n\
\n\
        Redirect permanent / https://db.local\n\
\n\
        ErrorLog ${APACHE_LOG_DIR}/error.log\n\
        CustomLog ${APACHE_LOG_DIR}/access.log combined\n\
</VirtualHost>";

const char *defaultconf = "server {\n\
    listen 80;\n\
    server_name DOMAIN_NAME www.DOMAIN_NAME;\n\
    return 301 https://DOMAIN_NAME$request_uri;\n\
}\n\
server {\n\
    listen 443 ssl;\n\
    server_name www.DOMAIN_NAME;\n\
    ssl_certificate /etc/twdssl/DOMAIN_NAME/fullchain.pem;\n\
    ssl_certificate_key /etc/twdssl/DOMAIN_NAME/cert-key.pem;\n\
    return 301 https://DOMAIN_NAME$request_uri;\n\
}\n\
server {\n\
    listen 443 ssl;\n\
    server_name DOMAIN_NAME;\n\
    ssl_certificate /etc/twdssl/DOMAIN_NAME/fullchain.pem;\n\
    ssl_certificate_key /etc/twdssl/DOMAIN_NAME/cert-key.pem;\n\
    client_max_body_size 50M;\n\
    root /var/www/html;\n\
    index index.php index.html index.htm;\n\
    location / {\n\
        try_files $uri $uri/ /index.php?@rewrite;\n\
    }\n\
    location @rewrite {\n\
        rewrite ^/(.*)$ /$1.php last;\n\
    }\n\
    error_page 404 /404.php;\n\
    location ~ \\.php$ {\n\
        include fastcgi_params;\n\
        fastcgi_pass twdash-php:9000;\n\
        fastcgi_param SCRIPT_FILENAME /var/www/html/DOMAIN_NAME$fastcgi_script_name;\n\
        try_files $uri =404;\n\
    }\n\
}";

int strsize(char *str){
  int n;
  for (n = 0; str[n] != '\0'; n++);
  return n;
}

char* strjoin(char *input, ...) {
    int num_vars = 0;
    char *ptr = input;
    while (*ptr) {
        if (*ptr == '%' && *(ptr + 1) == 's')
            num_vars++;
        ptr++;
    }
    va_list args;
	va_start(args, input);
    int sizeNeeded = strsize(input) + 1;
    for (int i = 0; i < num_vars; i++) {
        char *arg = va_arg(args,char *);
        sizeNeeded += strsize(arg);
    }
    va_end(args);
    va_start(args, input);
    static char result[2048];
    vsnprintf(result, 2048, input, args);
    va_end(args);
    return result;
}

void* print(char *input, ...) {
    int num_vars = 0;
	char *ptr = input;
    while(*ptr) {
        if (*ptr == '%') {
            if (*(ptr + 1) == 's')
                num_vars++;
        }
        ptr++;
    }
    va_list args;
    va_start(args, input);
    int sizeNeeded = strsize(input) + 1;
    for (int i = 0; i < num_vars; i++) {
        char *arg = va_arg(args,char *);
        sizeNeeded += strsize(arg);
    }
    va_end(args);
    va_start(args,input);
	char array[sizeNeeded];
    vsnprintf(array, sizeNeeded, input, args);
    va_end(args);
	printf("%s\n",array);
}

int is_directory(const char *path){
    struct stat stats;
    if (stat(path, &stats) == 0 && S_ISDIR(stats.st_mode))
        return 1;
    return 0;
}

void show_menu() {
    print(" 1) docker install\t\t(with sudo, only on cli)\n"
       " 2) docker rm\t\t\t(with sudo, only on cli)\n"
       " 3) install main\n"
       " 4) install phpmyadmin\n"
       " 5) install htmlwebsite \n"
       " 6) install phpwebsite \n"
       " 7) install wpwebsite \n"
       " 8) ps\n"
       " 9) ps -a\n"
       "10) network ls\n"
       "11) network create\n"
       "12) network rm\n"
       "13) stop\n"
       "14) rm\n"
       "15) ip\n"
       "16) start\n"
       "17) restart\n"
       "18) alias ls\n"
       "19) alias create\n"
       "20) alias rm\n"
       "21) ca\n"
       "22) ssl\n"
       "23) logs\n"
       "24) exec\n"
       "25) tree\n"
       "26) rmf\n"
       "27) images\n"
       "28) rmi\n"
       "29) db enter\n"
       "30) db backup\n"
       "31) db restore\n"
       "32) git\n"                //todo:
       "97) --version\n"    
       "98) --help\n"
       "99) clear\n"
        "0) exit");
}

int check_docker_installed() {
    int status = system("docker --version >/dev/null 2>&1");
    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 1 : 0;
}

void prepareHostsFile() {
    FILE *file;
    char line[1024];
    int start_found = 0;
    int end_found = 0;
    const char *start_pattern = "## twdash alias ##";
    const char *end_pattern = "##end twdash ##";
    file = fopen("/etc/hosts","r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, start_pattern)) {
            start_found = 1;
        }
        if (strstr(line, end_pattern)) {
            end_found = 1;
        }
    }
    if (!start_found) {
        fprintf(file,"\n%s\n",start_pattern);
    }
    if (!end_found) {
        fprintf(file,"%s\n",end_pattern);
    }
    fclose(file);
}

int start_container(const char *container_name){
    if (strcmp(container_name, "--all") == 0) {
        if (system("docker start twdash-php") == 0)
            print("[%s OK %s] started container twdash-php",GREEN,NC);
        else
            print("[%s Warnning %s] failed to start container twdash-php",ORANGE,NC);
        if (system("docker start twdash-mysql") == 0)
            print("[%s OK %s] started container twdash-mysql",GREEN,NC);
        else
            print("[%s Warnning %s] failed to start container twdash-mysql",ORANGE,NC);
        FILE *docker_ps = popen("docker ps -a --format \"{{.Names}}\"", "r");
        if (docker_ps != NULL) {
            char cont_names[1024];
            while (fgets(cont_names, sizeof(cont_names), docker_ps) != NULL) {
                char *token = strtok(cont_names, "\n");
                while (token != NULL) {
                    if (strcmp(token, "twdash-php") != 0 && strcmp(token, "twdash-mysql") != 0){
                        if (system(strjoin("docker start %s",token)) == 0) {
                            print("[%s OK %s] started container %s",GREEN,NC,token);
                            alias_create(token);
                        } else {
                            print("[%s Warnning %s] failed to start container %s",ORANGE,NC,token);
                        }
                    }
                    token = strtok(NULL, "\n");
                }
            }
            pclose(docker_ps);
        }
    } else {
        if (system(strjoin("docker start %s",container_name)) == 0) {
            print("[%s OK %s] started container %s",GREEN,NC,container_name);
            alias_create(container_name);
        } else {
            print("[%s Warnning %s] failed to start container %s",ORANGE,NC,container_name);
        }
    }
}

int restart_container(const char *container_name){
    if (strcmp(container_name, "--all") == 0) {
        if (system("docker restart twdash-php") == 0)
            print("[%s OK %s] container twdash-php has been restarted",GREEN,NC);
        else
            print("[%s Warnning %s] failed to restart container twdash-php",ORANGE,NC);
        if (system("docker restart twdash-mysql") == 0)
            print("[%s OK %s] restarted container twdash-mysql",GREEN,NC);
        else
            print("[%s Warnning %s] failed to restart container twdash-mysql",ORANGE,NC);
        FILE *docker_ps = popen("docker ps -a --format \"{{.Names}}\"", "r");
        if (docker_ps != NULL) {
            char cont_names[1024];
            while (fgets(cont_names, sizeof(cont_names), docker_ps) != NULL) {
                char *token = strtok(cont_names,"\n");
                while (token != NULL) {
                    if (strcmp(token,"twdash-php") != 0 && strcmp(token,"twdash-mysql") != 0){
                        if (system(strjoin("docker restart %s",token)) == 0) {
                            print("[%s OK %s] restarted container %s", GREEN,NC,token);
                            alias_create(token);
                        } else {
                            print("[%s Warnning %s] failed to restart container %s",ORANGE,NC,token);
                        }
                    }
                    token = strtok(NULL, "\n");
                }
            }
            pclose(docker_ps);
        }
    } else {
        if (system(strjoin("docker restart %s", container_name)) == 0) {
            print("[%s OK %s] restarted container %s",GREEN,NC,container_name);
            alias_create(container_name);
        } else {
            print("[%s Warnning %s] failed to restart container %s",ORANGE,NC,container_name);
        }
    }
}

int check_docker_compose_installed() {
    int status = system("docker-compose --version > /dev/null 2>&1");
    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 1 : 0;
}

int is_user_in_group(const char *username, const char *groupname) {
    int ngroups_max = sysconf(_SC_NGROUPS_MAX);
    if (ngroups_max < 0) {
        perror("sysconf");
        return -1;
    }
    gid_t *groups = malloc(ngroups_max * sizeof(gid_t));
    if (groups == NULL) {
        perror("malloc");
        return -1;
    }
    int ngroups = getgrouplist(username, getgid(), groups, &ngroups_max);
    if (ngroups < 0) {
        perror("getgrouplist");
        free(groups);
        return -1;
    }
    struct group *grp;
    for (int i = 0; i < ngroups; i++) {
        grp = getgrgid(groups[i]);
        if (grp != NULL && strcmp(grp->gr_name, groupname) == 0) {
            free(groups);
            return 1;
        }
    }
    free(groups);
    return 0;
}

int is_container_built(const char *container_name){
    int status = system(strjoin("docker inspect %s >/dev/null 2>&1",container_name));
    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 1 : 0;
}

int is_container_running(const char *container_name){
    int status = system(strjoin("docker ps --format '{{.Names}}' | grep -wq '%s'",container_name));
    return status == 0 ? 1 : 0;
}

void create_directory(const char *path){
    if(!is_directory(path)) {
        mkdir(path, 0755);
        if(is_directory(path))
            print("[%s OK %s] created directory %s",GREEN,NC,path);
        else
            print("[%s Warning %s] was not created directory %s",ORANGE,NC,path);
    } else {
        print("[%s OK %s] already setup directory %s",GREEN,NC,path);
    }
}

void create_file(const char *path, const char *content){
    FILE *check_fp = fopen(path, "r");
    if (check_fp != NULL) {
        fclose(check_fp);
        print("[%s OK %s] already exists file %s",GREEN,NC,path);
    } else {
        FILE *fp = fopen(path, "w");
        if (fp != NULL) {
            fprintf(fp, "%s", content);
            fclose(fp);
            print("[%s OK %s] created file %s",GREEN,NC,path);
        } else {
            print("[%s Warning %s] was not created file %s",ORANGE,NC,path);
        }
    }
}

void install_docker(){
    int user_os = 0;
    int user_choice = 0;
    char input[7];
    int wasinstalled = 0;
    int wasdocker = 0;
    if (check_docker_installed()){
        wasinstalled = 1;
    } else {
        while(user_os==0){
            printf("\n\nChoose the docker based distribution to use:\n1) Debian\n2) Ubuntu\n3) Fedora\n0) Exit\n");
            printf("\n» ");
            fgets(input,sizeof(input),stdin); 
            if (input[strlen(input) - 1] == '\n')
                input[strlen(input) - 1] = '\0';
            if(strcmp(input,"Debian") == 0 || strcmp(input,"debian") == 0 || strcmp(input,"1") == 0){
                user_os = 1;
            } else if(strcmp(input,"Ubuntu") == 0 || strcmp(input,"ubuntu") == 0 || strcmp(input,"2") == 0){
                user_os = 2;
            } else if(strcmp(input,"Fedora") == 0 || strcmp(input,"fedora") == 0 || strcmp(input,"3") == 0){
                user_os = 3; 
            } else if(strcmp(input,"Exit") == 0 || strcmp(input,"exit") == 0 || strcmp(input,"0") == 0){
                user_os = 4;
            }
        }
        print("\n%sinstalling docker%s",GREEN,NC);
        char input2[7];
        if(user_os==1||user_os==2){
            system("sudo apt install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common tree");
            if(user_os==1){
                system("curl -fsSL https://download.docker.com/linux/debian/gpg | sudo apt-key add -");
                while(user_choice==0){
                    printf("\n\nChoose debian realease:\n1) Bookworm\n2) Bullseye\n3) Buster\n0) Exit\n");
                    printf("\n» ");
                    fflush(stdout);
                    fgets(input2,sizeof(input2),stdin); 
                    if (input2[strlen(input2) - 1] == '\n')
                        input2[strlen(input2) - 1] = '\0';
                    if(strcmp(input2,"Bookworm") == 0 || strcmp(input2,"bookworm") == 0 || strcmp(input2,"1") == 0){
                        system("sudo add-apt-repository \"deb [arch=amd64] https://download.docker.com/linux/debian bookworm stable\"");
                        user_choice = 1;
                    } else if(strcmp(input2,"Bullseye") == 0 || strcmp(input2,"bullseye") == 0 || strcmp(input2,"2") == 0){
                        system("sudo add-apt-repository \"deb [arch=amd64] https://download.docker.com/linux/debian bullseye stable\"");
                        user_choice = 2;
                    } else if(strcmp(input2,"Buster") == 0 || strcmp(input2,"buster") == 0 || strcmp(input2,"3") == 0){
                        system("sudo add-apt-repository \"deb [arch=amd64] https://download.docker.com/linux/debian buster stable\"");
                        user_choice = 3; 
                    } else if(strcmp(input2,"Exit") == 0 || strcmp(input2,"exit") == 0 || strcmp(input2,"0") == 0){
                        user_choice = 4;
                    }
                }
            } else if(user_os==2){
                system("curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -");
                while(user_choice==0){
                    printf("\n\nChoose ubuntu realease:\n1) Noble\n2) Jammy\n3) Focal\n0) Exit\n");
                    printf("\n» ");
                    fflush(stdout);
                    fgets(input2,sizeof(input2),stdin); 
                    if (input2[strlen(input2) - 1] == '\n')
                        input2[strlen(input2) - 1] = '\0';
                    if(strcmp(input2,"Noble") == 0 || strcmp(input2,"noble") == 0 || strcmp(input2,"1") == 0){
                        system("sudo add-apt-repository \"deb [arch=amd64] https://download.docker.com/linux/ubuntu noble stable\"");
                        user_choice = 1;
                    } else if(strcmp(input2,"Jammy") == 0 || strcmp(input2,"jammy") == 0 || strcmp(input2,"2") == 0){
                        system("sudo add-apt-repository \"deb [arch=amd64] https://download.docker.com/linux/ubuntu jammy stable\"");
                        user_choice = 2;
                    } else if(strcmp(input2,"Focal") == 0 || strcmp(input2,"focal") == 0 || strcmp(input2,"3") == 0){
                        system("sudo add-apt-repository \"deb [arch=amd64] https://download.docker.com/linux/ubuntu focal stable\"");
                        user_choice = 3; 
                    } else if(strcmp(input2,"Exit") == 0 || strcmp(input2,"exit") == 0 || strcmp(input2,"0") == 0){
                        user_choice = 4;
                    }
                }
            }
            if(user_choice!=4)
                system("sudo apt install docker-ce docker-ce-cli containerd.io -y");
        } else if(user_os==3){
            system("sudo dnf install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common tree");
            system("curl -fsSL https://download.docker.com/linux/fedora/gpg | sudo apt-key add -");
            while(user_choice==0){
                printf("\n\nChoose fedora realease:\n1) 40\n2) 39\n3) 38\n0) Exit\n");
                printf("\n» ");
                fflush(stdout);
                fgets(input2,sizeof(input2),stdin); 
                if (input2[strlen(input2) - 1] == '\n')
                    input2[strlen(input2) - 1] = '\0';
                if(strcmp(input2,"40") == 0 || strcmp(input2,"1") == 0){
                    system("sudo add-apt-repository \"deb [arch=amd64] https://download.docker.com/linux/fedora 40 stable\"");
                    user_choice = 1;
                } else if(strcmp(input2,"39") == 0 || strcmp(input2,"2") == 0){
                    system("sudo add-apt-repository \"deb [arch=amd64] https://download.docker.com/linux/fedora 39 stable\"");
                    user_choice = 2;
                } else if(strcmp(input2,"38") == 0 || strcmp(input2,"3") == 0){
                    system("sudo add-apt-repository \"deb [arch=amd64] https://download.docker.com/linux/fedora 38 stable\"");
                    user_choice = 3; 
                } else if(strcmp(input2,"Exit") == 0 || strcmp(input2,"exit") == 0 || strcmp(input2,"0") == 0){
                    user_choice = 4;
                }
            }
            if(user_choice!=4)
                system("sudo dnf install docker-ce docker-ce-cli containerd.io -y");
        }
    }
    if (check_docker_installed()){
		int dcompose = 1;
		if(!check_docker_compose_installed() && user_choice!=4){
			print("\n%sinstalling docker-compose%s", GREEN, NC);
            system("sudo curl -SL https://github.com/docker/compose/releases/latest/download/docker-compose-linux-x86_64 -o /usr/local/bin/docker-compose");
            system("sudo chmod -R a+x /usr/local/bin/docker-compose");
            system("sudo ln -s /usr/local/bin/docker-compose /usr/bin/docker-compose");
			dcompose = 0;
		}
		if(wasinstalled)
			print("[%s OK %s] docker was already installed",GREEN,NC);
		else
			print("[%s OK %s] docker installed successfully",GREEN,NC);
		if (check_docker_compose_installed()){
			if(dcompose)
            	print("[%s OK %s] docker-compose was already installed",GREEN,NC);
			else
            	print("[%s OK %s] docker-compose installed succefully",GREEN,NC);
        } else {
			if(dcompose)
            	print("[%s Warnning %s] docker-compose was not installed",ORANGE,NC);
			else
            	print("[%s Warnning %s] docker-compose was not installed correctly",ORANGE,NC);
        }
        struct group *grp = getgrnam("docker");
        int grps = grp != NULL ? 1 : 0;
		if (grps) {
			print("[%s OK %s] docker group already exists",GREEN,NC);
		} else {
			system("sudo groupadd docker");
			print("[%s OK %s] group docker added",GREEN,NC);
		}
		if (is_user_in_group(getenv("USER"),"docker")){
			wasdocker = 1;
			print("[%s OK %s] user %s was already in docker group",GREEN,NC,getenv("USER"));
		} else {
			system(strjoin("sudo usermod -aG docker %s",getenv("USER")));
			system("sudo chown root:docker /etc/hosts");
			system("sudo chmod 664 /etc/hosts");
			if (is_user_in_group(getenv("USER"),"docker"))
				print("[%s OK %s] user %s added to docker group",GREEN,NC,getenv("USER"));
			else
				print("[%s Warnning %s] user %s has not been add to sudo group",ORANGE,NC,getenv("USER"));
		}
        int status = system("docker info > /dev/null 2>&1");
        int is_docker_running = WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 1 : 0;
        if (!is_docker_running){
            system("systemctl start docker");
            if(is_docker_running)
                print("[%s OK %s] docker is running",GREEN,NC);
            else
                print("[%s Warnning %s] docker is not running",ORANGE,NC);
        } else {
            print("[%s OK %s] docker running successfully",GREEN,NC);
        } 
    } else {
        print("[%s Warnning %s] docker was not installed successfully",ORANGE,NC);
    }
    if(!wasdocker){
        print("%splease re-login to user added to sudo group take effect or just use 'su - %s'%s",GREEN,getenv("USER"),NC);
    }
}

void rm_docker(){
    int user_os = 0;
    char input[7];
    while(user_os==0){
        printf("\n\nChoose the docker based distribution:\n1) Debian/Ubuntu\n2) Fedora \n0) Exit\n");
        printf("\n» ");
        fflush(stdout);
        fgets(input,sizeof(input),stdin); 
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';
        if(strcmp(input,"Debian") == 0 || strcmp(input,"debian") == 0 || strcmp(input,"1") == 0 || strcmp(input,"Ubuntu") == 0 || strcmp(input,"ubuntu") == 0 || strcmp(input,"2") == 0){
            user_os = 1;
        } else if(strcmp(input,"Fedora") == 0 || strcmp(input,"fedora") == 0 || strcmp(input,"2") == 0){
            user_os = 2;
        } else if(strcmp(input,"Exit") == 0 || strcmp(input,"exit") == 0 || strcmp(input,"0") == 0){
            user_os = 3;
        }
    }
    if(user_os!=3){
        print("%sRemoving and purging Docker%s",GREEN,NC);
        if(user_os==1){
            system("sudo apt remove docker.io docker-compose -y");
            system("sudo apt purge docker-ce docker-ce-cli");
        } else if(user_os==2){
            system("sudo dnf remove docker.io docker-compose -y");
            system("sudo dnf purge docker-ce docker-ce-cli");
        }
        system("sudo rm -rf /etc/docker /var/lib/docker");
        system("sudo deluser $USER docker");
        system("sudo groupdel docker");
        system("sudo rm -rf /usr/bin/docker /usr/local/bin/docker-compose");
        system("sudo rm -rf /usr/bin/docker-compose");
        print("%sRunning autoremove --purge%s", GREEN, NC);
        if(user_os==1)
            system("sudo apt autoremove --purge -y");
        else if(user_os==2)
            system("sudo dnf autoremove --purge -y");

        if(check_docker_installed())
            print("\n[%s Warnning %s] Docker has not been removed corretly",ORANGE,NC);
        else
            print("\n[%s OK %s] Docker removed successfully",GREEN,NC);
        if(check_docker_compose_installed())
            print("[%s Warnning %s] Docker-compose has not been removed corretly",ORANGE,NC);
        else
            print("[%s OK %s] Docker-compose removed successfully",GREEN,NC);
    }
}

void install_twdash(){
    print("");
    if(!is_directory(strjoin("/home/%s/twdash",getenv("USER")))){
        create_directory(strjoin("/home/%s/twdash",getenv("USER")));
        create_directory(strjoin("/home/%s/twdash/sites",getenv("USER")));
        create_directory(strjoin("/home/%s/twdash/admin",getenv("USER")));
        create_directory(strjoin("/home/%s/twdash/admin/ssl",getenv("USER")));
        create_directory(strjoin("/home/%s/twdash/admin/nginx",getenv("USER")));
    }
	prepareHostsFile();
    install_ssl_ca();
    install_twdash_mysql();
    install_twdash_php();
    print("");
}

void install_twdash_mysql(){
    if(!is_container_running("twdash-mysql")){
        if(!is_container_built("twdash-mysql")){
            print("%sinstalling twdash-mysql container%s", GREEN, NC);
            const char *docker_compose_content = 
            "services:\n"
            "  twdash-mysql:\n"
            "    container_name: twdash-mysql\n"
            "    image: mysql:latest\n"
            "    environment:\n"
            "      MYSQL_ROOT_PASSWORD: pass\n"
            "    volumes:\n"
            "      - db_data:/var/lib/mysql\n"
            "    networks:\n"
            "      - twdash_local\n" 
            "\n"
            "networks:\n"
            "  twdash_local:\n"
            "       name: twdash_local\n"
            "\n"
            "volumes:\n"
            "  db_data:\n";
            system(strjoin("docker-compose -f - up -d 2>&1 <<EOF\n%s\nEOF",docker_compose_content));
			if(is_container_running("twdash-mysql"))
				print("[%s OK %s] twdash-mysql is running",GREEN,NC);
			else
				print("[%s Warnning %s] twdash-mysql installed but not running",ORANGE,NC);
        } else {
            start_container("twdash-mysql");
			if(is_container_built("twdash-mysql")){
				if(is_container_running("twdash-mysql"))
					print("[%s OK %s] twdash-mysql is running",GREEN,NC);
				else
					print("[%s Warnning %s] twdash-mysql installed but not running",ORANGE,NC);

			} else {
				print("[%s Warnning %s] twdash-mysql container not installed succefully",ORANGE,NC);
			}
        }
    } else {
		print("[%s OK %s] twdash-mysql already installed and running",GREEN,NC);
    }
}

int install_twdash_php(){
    if(!is_container_running("twdash-php")){
        if(!is_container_built("twdash-php")){
            char *DOCKERFILE_CONTENT = 
            "FROM php:fpm\n"
            "\n"
            "RUN docker-php-ext-install mysqli pdo pdo_mysql\n"
            "RUN apt-get update -y && apt-get install -y \\\n"
            "    webp \\\n"
            "    libcurl4-openssl-dev \\\n"
            "    libonig-dev \\\n"
            "    libxml2-dev \\\n"
            "    libzip-dev \\\n"
            "    libgd-dev \\\n"
            "    libmagickwand-dev --no-install-recommends\n";
            create_file(strjoin("/home/%s/twdash/admin/nginx/twdash-Dockerfile.tmp",getenv("USER")),DOCKERFILE_CONTENT);
            if (chmod(strjoin("/home/%s/twdash/admin/nginx/twdash-Dockerfile.tmp",getenv("USER")), 0755) == -1) {
                perror("Error changing permissions of Dockerfile");
                return 1;
            }
            char *DOCKER_COMPOSE_CONTENT =
            "services:\n"
            "  twdash-php:\n"
            "    container_name: twdash-php\n"
            "    build:\n"
            "      dockerfile: /home/%s/twdash/admin/nginx/twdash-Dockerfile.tmp\n"
            "    volumes:\n"
            "      - /home/%s/twdash/sites:/var/www/html\n"
            "    networks:\n"
            "      - twdash_local\n"
            "\n"
            "networks:\n"
            "  twdash_local:\n"
            "       name: twdash_local\n";
            char * res = strjoin(DOCKER_COMPOSE_CONTENT,getenv("USER"),getenv("USER"));
            size_t buffer_size = snprintf(NULL, 0,"echo \"%s\" | docker-compose -f - up -d 2>&1",res) + 1;
            char *d = malloc(buffer_size);
            int res2 = 1;
            if (d != NULL) {
                snprintf(d, buffer_size,"echo \"%s\" | docker-compose -f - up -d 2>&1",res);
                res2 = system(d);
                free(d);
            }
            if (res2 == -1) {
                perror("Error executing docker-compose command");
                return 1;
            }
            if (remove(strjoin("/home/%s/twdash/admin/nginx/twdash-Dockerfile.tmp",getenv("USER"))) != 0) {
                perror("Error removing temporary Dockerfile");
                return 1;
            }
			if(is_container_running("twdash-php")){
				print("[%s OK %s] twdash-php is running",GREEN,NC);
			} else {
				if(is_container_built("twdash-php"))
					print("[%s Warnning %s] twdash-php installed but not running",ORANGE,NC);
				else
					print("[%s Warnning %s] twdash-php not installed and not running",ORANGE,NC);
			}
            return 0;
        } else {
            print("[%s OK %s]twdash-php isnt running but its already installed",GREEN,NC);
            start_container("twdash-php");
            if(is_container_running("twdash-php"))
                print("[%s OK %s]twdash-php has been started",GREEN,NC);
        }
    } else {
        print("[%s OK %s] twdash-php already installed and running",GREEN,NC);
    }
}
char htmlconf[] = "server {\n\
    listen 80;\n\
    server_name DOMAIN_NAME www.DOMAIN_NAME;\n\
    return 301 https://DOMAIN_NAME$request_uri;\n\
}\n\
server {\n\
    listen 443 ssl;\n\
    server_name www.DOMAIN_NAME;\n\
    ssl_certificate /etc/twdssl/DOMAIN_NAME/fullchain.pem;\n\
    ssl_certificate_key /etc/twdssl/DOMAIN_NAME/cert-key.pem;\n\
    return 301 https://DOMAIN_NAME$request_uri;\n\
}\n\
server {\n\
    listen 443 ssl;\n\
    server_name DOMAIN_NAME;\n\
    ssl_certificate /etc/twdssl/DOMAIN_NAME/fullchain.pem;\n\
    ssl_certificate_key /etc/twdssl/DOMAIN_NAME/cert-key.pem;\n\
    client_max_body_size 50M;\n\
    root /var/www/html;\n\
    index index.html index.htm;\n\
    location / {\n\
        try_files $uri $uri/ @rewrite;\n\
    }\n\
    location @rewrite {\n\
        rewrite ^/(.*)$ /$1.html last;\n\
        try_files $uri =404\n\
    }\n\
}";

void nginx_html_website(char* dn){
    if(is_container_built(dn)){
		print("[%s Warnning %s] Already exist a container with this name",ORANGE,NC);
	} else {
		print("");
		create_directory(strjoin("/home/%s/twdash/sites/%s",getenv("USER"),dn));
		create_file(strjoin("/home/%s/twdash/sites/%s/index.html",getenv("USER"),dn),strjoin("<h2>%s</h2><br>",dn));
		create_file(strjoin("/home/%s/twdash/admin/%s.conf",getenv("USER"),dn),htmlconf);
		system(strjoin("sed -i 's/DOMAIN_NAME/%s/g' '/home/%s/twdash/admin/%s.conf'",dn,getenv("USER"),dn));
		const char *dcc_template =
            "services:\n"
            "   %s:\n"
            "    image: nginx:alpine\n"
            "    container_name: %s\n"
            "    volumes:\n"
            "      - /home/%s/twdash/admin/ssl/%s:/etc/twdssl/%s\n"
            "      - /home/%s/twdash/admin/%s.conf:/etc/nginx/conf.d/default.conf\n"
            "      - /home/%s/twdash/sites/%s:/var/www/html\n"
            "    networks:\n"
            "      - twdash\n"
            "\n"
            "networks:\n"
            "  twdash:\n"
            "       name: twdash\n";
        FILE *pipe = popen("docker-compose -f - up -d", "w");
        if (pipe == NULL) {
            printf("[%s Warning %s] Failed to open pipe to docker-compose\n",ORANGE,NC);
            return;
        }
        fprintf(pipe, dcc_template, dn, dn, getenv("USER"), dn, dn, getenv("USER"), dn, getenv("USER"), dn);
        if (pclose(pipe) == -1) {
            printf("[%s Warning %s] Failed to close pipe to docker-compose\n",ORANGE,NC);
            return;
        }
		alias_create(dn);
	}
}


void install_phpmyadmin_website(){
    char *dn = "db.local";
	if(is_container_built(dn)){
		print("[%s Warnning %s] Already exist a container with this name",ORANGE,NC);
	} else {
		print("");
		install_ssl(dn);
		create_directory(strjoin("/home/%s/twdash/admin/nginx/%s",getenv("USER"),dn));
		create_file(strjoin("/home/%s/twdash/admin/nginx/%s/default.conf",getenv("USER"),dn),phpmyadminconf);
		system(strjoin("sed -i 's/DOMAIN_NAME/%s/g' '/home/%s/twdash/admin/nginx/%s/default.conf'",dn,getenv("USER"),dn));
		const char *ddcc_template =
            "services:\n"
            "  db.local:\n"
            "    image: phpmyadmin/phpmyadmin\n"
            "    container_name: db.local\n"
            "    environment:\n"
            "      PMA_HOST: twdash-mysql\n"
            "      PMA_USER: root\n"
            "      PMA_PASSWORD: pass\n"
            "      PMA_PORT: 3306\n"
            "    volumes:\n"
            "      - /home/%s/twdash/admin/ssl/%s:/etc/twdssl/%s\n"
            "      - /home/%s/twdash/admin/nginx/%s/default.conf:/etc/apache2/sites-available/000-default.conf\n"
            "    networks:\n"
			"      - twdash_local\n"
            "networks:\n"
            "  twdash_local:\n"
            "       name: twdash_local\n";
        FILE *pipe = popen("docker-compose -f - up -d", "w");
        if (pipe == NULL) {
            printf("[%s Warning %s] Failed to open pipe to docker-compose\n",ORANGE,NC);
            return;
        }
        fprintf(pipe, ddcc_template, getenv("USER"), dn, dn, getenv("USER"), dn);
        if (pclose(pipe) == -1) {
            printf("[%s Warning %s] Failed to close pipe to docker-compose\n",ORANGE,NC);
            return;
        }
		alias_create(dn);
        system("docker exec -it db.local a2enmod ssl");
        system(strjoin("echo '%s' > /home/%s/twdash/admin/nginx/%s/default.conf",phpmyadminconf2, getenv("USER"),dn));
        system("twd restart db.local");
		print("\n\n%shttps://%s%s\n",GREEN,dn,NC);
	}
}




void nginx_php_website(char *dn){
	if(is_container_built(dn)){
		print("[%s Warnning %s] Already exist a container with this name",ORANGE,NC);
	} else {
		print("");
		install_ssl(dn);
		create_directory(strjoin("/home/%s/twdash/admin/nginx/%s",getenv("USER"),dn));
		create_directory(strjoin("/home/%s/twdash/sites/%s",getenv("USER"),dn));
		create_file(strjoin("/home/%s/twdash/sites/%s/index.php",getenv("USER"),dn),"<?php echo '<h2>twdash-local website</h2><br>'; ?>");
		create_file(strjoin("/home/%s/twdash/admin/nginx/%s/default.conf",getenv("USER"),dn),defaultconf);
		system(strjoin("sed -i 's/DOMAIN_NAME/%s/g' '/home/%s/twdash/admin/nginx/%s/default.conf'",dn,getenv("USER"),dn));
		const char *dcc_template =
			        "services:\n"
			        "   %s:\n"
			        "    image: nginx:alpine\n"
			        "    container_name: %s\n"
			        "    volumes:\n"
			        "      - /home/%s/twdash/admin/ssl/%s:/etc/twdssl/%s\n"
			        "      - /home/%s/twdash/admin/nginx/%s/default.conf:/etc/nginx/conf.d/default.conf\n"
			        "      - /home/%s/twdash/sites/%s:/var/www/html\n"
			        "    networks:\n"
			        "      - twdash_local\n"
			        "\n"
			        "networks:\n"
			        "  twdash_local:\n"
			        "       name: twdash_local\n";
			    FILE *pipe = popen("docker-compose -f - up -d", "w");
			    if (pipe == NULL) {
			        printf("[%s Warning %s] Failed to open pipe to docker-compose\n",ORANGE,NC);
			        return;
			    }
			    fprintf(pipe, dcc_template, dn, dn, getenv("USER"), dn, dn, getenv("USER"), dn, getenv("USER"), dn);
			    if (pclose(pipe) == -1) {
			        printf("[%s Warning %s] Failed to close pipe to docker-compose\n",ORANGE,NC);
			        return;
			    }
		alias_create(dn);
		char new_str[strlen(dn) + 1]; 
        for (int i = 0; i < strsize(dn); i++) {
            if (dn[i] == '.') {
                new_str[i] = '_'; 
            } else {
                new_str[i] = dn[i]; 
            }
        }
        new_str[strlen(dn)] = '\0';
        system(strjoin("docker exec twdash-mysql mysql -uroot -ppass -e \"CREATE DATABASE %s;\"",new_str));
		print("\n\n%sdatabase credentials:\n\ndb name:\t%s\nuser:\t\troot\npass:\t\tpass\nlocalhost:\ttwdash-mysql\n\nurl:\t\thttps://%s%s\n",GREEN,new_str,dn,NC);
	}
}

void nginx_wpwebsite(char* dn){
	if(is_container_built(dn)){
		print("[%s Warnning %s] Already exist a container with this name",ORANGE,NC);
	} else {
		print("");
		install_ssl(dn);
		create_directory(strjoin("/home/%s/twdash/admin/nginx/%s",getenv("USER"),dn));
		create_file(strjoin("/home/%s/twdash/admin/nginx/%s/default.conf",getenv("USER"),dn),defaultconf);
		system(strjoin("sed -i 's/DOMAIN_NAME/%s/g' '/home/%s/twdash/admin/nginx/%s/default.conf'",dn,getenv("USER"),dn));
		system(strjoin("wget -O /home/%s/twdash/sites/latest.zip https://wordpress.org/latest.zip >/dev/null 2>&1",getenv("USER")));
		system(strjoin("unzip /home/%s/twdash/sites/latest.zip -d /home/%s/twdash/sites >/dev/null 2>&1",getenv("USER"),getenv("USER")));
		system(strjoin("rm /home/%s/twdash/sites/latest.zip",getenv("USER")));
		system(strjoin("mv /home/%s/twdash/sites/wordpress /home/%s/twdash/sites/%s",getenv("USER"),getenv("USER"),dn));
		const char *dcc_template =
	        "services:\n"
	        "   %s:\n"
	        "    image: nginx:alpine\n"
	        "    container_name: %s\n"
	        "    volumes:\n"
	        "      - /home/%s/twdash/admin/ssl/%s:/etc/twdssl/%s\n"
	        "      - /home/%s/twdash/admin/nginx/%s/default.conf:/etc/nginx/conf.d/default.conf\n"
	        "      - /home/%s/twdash/sites/%s:/var/www/html\n"
	        "    networks:\n"
	        "      - twdash_local\n"
	        "\n"
	        "networks:\n"
	        "  twdash_local:\n"
	        "       name: twdash_local\n";
	    FILE *pipe = popen("docker-compose -f - up -d", "w");
	    if (pipe == NULL) {
	        printf("[%s Warning %s] Failed to open pipe to docker-compose\n",ORANGE,NC);
	        return;
	    }
	    fprintf(pipe, dcc_template, dn, dn, getenv("USER"), dn, dn, getenv("USER"), dn, getenv("USER"), dn);
	    if (pclose(pipe) == -1) {
	        printf("[%s Warning %s] Failed to close pipe to docker-compose\n",ORANGE,NC);
	        return;
	    }
        printf("%schanging website file permissions to www-data and 777 (sudo password may be needed)%s\n",GREEN,NC);
        system(strjoin("chmod -R 777 /home/%s/twdash/sites/%s",getenv("USER"),dn));
        system(strjoin("sudo chown -R www-data:www-data /home/%s/twdash/sites/%s",getenv("USER"),dn));
		alias_create(dn);
        char new_str[strlen(dn) + 1]; 
        for (int i = 0; i < strsize(dn); i++) {
            if (dn[i] == '.') {
                new_str[i] = '_'; 
            } else {
                new_str[i] = dn[i]; 
            }
        }
        new_str[strlen(dn)] = '\0';
        system(strjoin("docker exec twdash-mysql mysql -uroot -ppass -e \"CREATE DATABASE %s;\"",new_str));
		print("\n\n%sdatabase credentials:\n\ndb name:\t%s\nuser:\t\troot\npass:\t\tpass\nlocalhost:\ttwdash-mysql\n\nurl:\t\thttps://%s%s\n",GREEN,new_str,dn,NC);
	}
}

int install_ssl_ca(){
    if (system(strjoin("openssl genrsa -aes256 -passout pass:twdash-local-p455w0rd -out /home/%s/twdash/admin/ssl/twdash-key.pem 4096",getenv("USER"))) == -1){
        print("[%s Warnning %s] error executing openssl genrsa command",ORANGE,NC);
        return 1;
    } else {
        print("[%s OK %s] ca twdash-key.pem has been created",GREEN,NC);
        if (system(strjoin("openssl req -new -x509 -sha256 -days 3650 -key /home/%s/twdash/admin/ssl/twdash-key.pem -out /home/%s/twdash/admin/ssl/twdash.pem -passin pass:twdash-local-p455w0rd -subj \"/C=US/ST=New York/L=New York/O=twdash software local/OU=twdash software/CN=twdash\"\n",getenv("USER"),getenv("USER"))) == -1){
            print("[%s Warnning %s] error executing openssl req command",ORANGE,NC);
            return 1;
        } else {
            print("[%s OK %s] ca twdash.pem has been created with success, use this file in your browser trust authorities",GREEN,NC);
        }
    }
}
 
int install_ssl(const char *domain_name){
    struct stat st;
    char* mresul = strjoin("/home/%s/twdash/admin/ssl/%s",getenv("USER"),domain_name);
    if (stat(strjoin("/home/%s/twdash/admin/ssl",getenv("USER")),&st) == 0 && S_ISDIR(st.st_mode)){
        if (stat(strjoin("/home/%s/twdash/admin/ssl/%s",getenv("USER"),domain_name), &st) == 0 && S_ISDIR(st.st_mode)){
            print("[%s OK %s] directory /home/%s/twdash/admin/ssl/%s was already created",GREEN,NC,getenv("USER"),domain_name);
        } else {
            if (mkdir(mresul, 0755) == 0){
                print("[%s OK %s] directory /home/%s/twdash/ssl/%s created",GREEN,NC,getenv("USER"),domain_name);
            } else {
                perror("Error creating directory");
                return 1;
            }
        }
    } else {
        print("[%s Warnning %s]  /home/%s/twdash/admin/ssl/ directory does not exist",ORANGE,NC,getenv("USER"));
        return 1;
    }
    if (system(strjoin("openssl genrsa -out /home/%s/twdash/admin/ssl/%s/cert-key.pem 4096",getenv("USER"),domain_name)) == -1)
        print("[%s Warnning %s] error executing openssl genrsa command",ORANGE,NC);
    else
        print("[%s OK %s] cert-key.pem created",GREEN,NC);
    if (system(strjoin("openssl req -new -sha256 -subj \"/CN=%s\" -key /home/%s/twdash/admin/ssl/%s/cert-key.pem -out /home/%s/twdash/admin/ssl/%s/cert.csr -nodes",domain_name,getenv("USER"),domain_name,getenv("USER"),domain_name)) == -1)
        print("[%s Warnning %s] error executing openssl req command",ORANGE,NC);
    else
        print("[%s OK %s] cert.csr",GREEN,NC);
    if (system(strjoin("echo \"subjectAltName=DNS:%s\" >> /home/%s/twdash/admin/ssl/%s/extfile.cnf",domain_name,getenv("USER"),domain_name)) == -1)
        print("[%s Warnning %s] error executing echo command",ORANGE,NC);
    else
        print("[%s OK %s] cert.csr",GREEN,NC);
    int status_req3 = -1;
	status_req3 = system(strjoin("openssl x509 -req -sha256 -days 3650 -passin pass:twdash-local-p455w0rd -in /home/%s/twdash/admin/ssl/%s/cert.csr -CA /home/%s/twdash/admin/ssl/twdash.pem -CAkey /home/%s/twdash/admin/ssl/twdash-key.pem -out /home/%s/twdash/admin/ssl/%s/cert.pem -extfile /home/%s/twdash/admin/ssl/%s/extfile.cnf -CAcreateserial",getenv("USER"),domain_name,getenv("USER"),getenv("USER"),getenv("USER"),domain_name,getenv("USER"),domain_name)); 
    if (status_req3 == -1)
        print("[%s Warnning %s] error executing openssl x509 command",ORANGE,NC);
    else
        print("[%s OK %s] cert.csr",GREEN,NC);
    if (system(strjoin("rm /home/%s/twdash/admin/ssl/%s/cert.csr",getenv("USER"),domain_name)) == -1)
        print("[%s Warnning %s] error executing rm /home/%s/twdash/ssl/%s/cert.csr",ORANGE,NC,getenv("USER"),domain_name);
    else
        print("[%s OK %s] rm cert.csr",GREEN,NC);
    int status1 = system(strjoin("cat /home/%s/twdash/admin/ssl/%s/cert.pem > /home/%s/twdash/admin/ssl/%s/fullchain.pem",getenv("USER"),domain_name,getenv("USER"),domain_name));
    int status2 = system(strjoin("cat /home/%s/twdash/admin/ssl/twdash.pem >> /home/%s/twdash/admin/ssl/%s/fullchain.pem",getenv("USER"),getenv("USER"),domain_name));
    if (status1 == -1 || status2 == -1)
        print("[%s Warnning %s] error executing cat command",ORANGE,NC);
    else
        print("[%s OK %s] fullchain.pem",GREEN,NC);
    return 0;
}

void network_create(const char *NETWORK_NAME){
    if (system(strjoin("docker network inspect %s > /dev/null 2>&1",NETWORK_NAME)) == 0) {
        print("[%s OK %s] network '%s' already exist",GREEN,NC,NETWORK_NAME);
    } else {
        if (system(strjoin("docker network create %s",NETWORK_NAME)) == 0)
            print("[%s OK %s] network '%s' created successfully",GREEN,NC,NETWORK_NAME);
        else
            print("[%s Warnning %s] failed to create network '%s'",ORANGE,NC,NETWORK_NAME);
    }
}

void network_rm(const char *NETWORK_NAME){
    if (system(strjoin("docker network inspect %s > /dev/null 2>&1",NETWORK_NAME)) == 0){
        if (system(strjoin("docker network rm %s", NETWORK_NAME)) == 0)
            print("[%s OK %s] network '%s' removed successfully",GREEN,NC,NETWORK_NAME);
        else
            print("[%s Warnning %s] failed to remove network '%s'",ORANGE,NC,NETWORK_NAME);
    } else {
        print("[%s OK %s] network '%s' does not exist",GREEN,NC,NETWORK_NAME);
    }
}

void stop_container(const char *CONTAINER_NAME,int OPTION){
    if (strcmp(CONTAINER_NAME,"--all") == 0){
        if (system(strjoin("docker stop $(docker ps -q)")) == 0)
            print("[%s OK %s]  all containers have been stopped",GREEN,NC);
        else
            print("[%s Warnning %s] failed to stop all containers",ORANGE,NC);
    } else {
        if (system(strjoin("docker stop %s", CONTAINER_NAME)) == 0){
            print("[%s OK %s] container %s has been stopped",GREEN,NC,CONTAINER_NAME);
            alias_rm(CONTAINER_NAME);
        } else {
            print("[%s Warnning %s] Failed to stop container %s",ORANGE,NC,CONTAINER_NAME);
        }
        if(OPTION){
            rmf(CONTAINER_NAME);
        }
    }
}

int list_alias(){
	print("");
    system("awk '/## twdash alias ##/,/##end twdash ##/' /etc/hosts");
	print("");
    exit(1);
}

int is_name_present(const char *name) {
    FILE *file = fopen("/etc/hosts", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; 
    }

    char line[100];
    int inside_block = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "## twdash alias ##") != NULL) {
            inside_block = 1;
            continue;
        }
        if (strstr(line, "##end twdash ##") != NULL) {
            break;
        }
        if (inside_block) {
            char *token = strtok(line, "\t");
            if (token != NULL){
                token = strtok(NULL, "\t\n");
                if (token != NULL && strcmp(token, name) == 0) {
                    fclose(file);
                    return 1;
                }
            }
        }
    }
    fclose(file);
    return 0; 
}

int is_ip_present(const char *ip_address) {
    FILE *file = fopen("/etc/hosts", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    char line[100];
    int inside_block = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "## twdash alias ##") != NULL) {
            inside_block = 1;
            continue;
        }
        if (strstr(line, "##end twdash ##") != NULL) {
            break;
        }
        if (inside_block) {
            char *token = strtok(line, "\t");
            if (token != NULL) {
                if (strcmp(token, ip_address) == 0) {
                    fclose(file);
                    return 1; 
                }
            }
        }
    }
    fclose(file);
    return 0;
}

int is_ip_and_name_present(const char *ip_address, const char *name) {
    FILE *file = fopen("/etc/hosts", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[100];
    int inside_block = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "## twdash alias ##") != NULL) {
            inside_block = 1;
            continue;
        }
        if (strstr(line, "##end twdash ##") != NULL) {
            break;
        }
        if (inside_block) {
            char *token = strtok(line, "\t");
            if (token != NULL && strcmp(token, ip_address) == 0) {
                token = strtok(NULL, "\t\n");
                if (token != NULL && strcmp(token, name) == 0) {
                    fclose(file);
                    return 1;
                }
            }
        }
    }
    fclose(file);
    return 0;
}


void alias_create(const char *container_alias){
    if(is_container_built(container_alias)){
        if(is_container_running(container_alias)){
            char *ip = get_container_ip(container_alias);
            int isname = is_name_present(container_alias);
            int isip = is_ip_present(ip);
            const char *start_pattern = "## twdash alias ##";
            const char *end_pattern = "##end twdash ##";
            if(isname && isip){
                if(is_ip_and_name_present(ip,container_alias)==1){
                    print("[%s OK %s] alias and ip already correctly set up",GREEN,NC);
                } else if(is_ip_and_name_present(ip,container_alias)==0){
                    if(hostsupdate(strjoin("/%s/,/%s/{/%s/d}","## twdash alias ##","##end twdash ##",container_alias))){
                        if(strcmp(container_alias,"twdash-php")!=0 && strcmp(container_alias,"twdash-mysql")!=0){
                            if(hostsupdate(strjoin("/%s/a %s\t%s",start_pattern,ip,container_alias)))
                                print("[%s OK %s] alias %s updated",GREEN,NC,container_alias);
                            else
                                print("[%s Warnning %s] alias update failed",ORANGE,NC);
                        }
                    } else {
                        print("[%s Warnning %s] alias update failed",ORANGE,NC);
                    }
                } else {
                    print("[%s Warnning %s] There was an error checking if alias and ip are in same line",ORANGE,NC);
                }
            } else if(!isname && isip){
                if(hostsupdate(strjoin("/%s/,/%s/s/\\<%s\\>.*/%s\t%s/",start_pattern,end_pattern,ip,ip,container_alias)))
                    print("[%s OK %s] host alias updated successfully",GREEN,NC);
                else
                    print("[%s Warnning %s] host alias update failed",ORANGE,NC);
            } else if(isname && !isip){
				if(hostsupdate(strjoin("/%s/,/%s/{/%s/d}","## twdash alias ##","##end twdash ##",container_alias))){
					if(strcmp(container_alias,"twdash-php")!=0 && strcmp(container_alias,"twdash-mysql")!=0){
						if(hostsupdate(strjoin("/%s/a %s\t%s",start_pattern,ip,container_alias)))
							print("[%s OK %s] alias %s updated",GREEN,NC,container_alias);
						else
							print("[%s Warnning %s] alias update failed",ORANGE,NC);
					}
				} else {
					print("[%s Warnning %s] alias update failed",ORANGE,NC);
				}
            } else {
                if(strcmp(container_alias,"twdash-php")!=0 && strcmp(container_alias,"twdash-mysql")!=0){
                    if(hostsupdate(strjoin("/%s/a %s\t%s",start_pattern,ip,container_alias)))
                        print("[%s OK %s] alias %s added",GREEN,NC,container_alias);
                    else
                        print("[%s Warnning %s] adding alias failed",ORANGE,NC);
                }
            }
        } else {
            print("[%s Warnning %s] this container should be running",ORANGE,NC);
        }
    } else {
        print("[%s Warnning %s] this container doesnt seem to exist",ORANGE,NC);
    }
}

int hostsupdate(char *action) {
    int a = 1;
    a = system("cp /etc/hosts /tmp/hosts_backup");
    int size = strsize(action) + 30;
    char contalias[size];
    strcpy(contalias,"sed -i '");
    strcat(contalias,action);
    strcat(contalias,"' /tmp/hosts_backup");
    if (!a)
        a = system(contalias);
    if (!a){
    	a = system("cp /tmp/hosts_backup /etc/hosts");
    }
	if (!a)
        a = system("rm /tmp/hosts_backup");
    return a == 0 ? 1 : 0;
}

void alias_rm(const char *container_alias){
    if(is_container_running(container_alias)){
        print("[%s Warnning %s] container %s is running",ORANGE,NC,container_alias);
    } else {
        if (hostsupdate(strjoin("/%s/,/%s/{/%s/d}","## twdash alias ##","##end twdash ##",container_alias)))
            print("[%s OK %s] alias %s removed",GREEN,NC,container_alias);
        else
            print("[%s Warnning %s] failed to remove alias %s",ORANGE,NC,container_alias);
    }
}

void rm_container(const char *container_name){
    if (strcmp(container_name, "--all") == 0){
        system("docker rm $(docker ps -aq)");
        print("[%s OK %s] all docker containers have been stopped and removed",GREEN,NC); 
    } else {
        if (is_container_built(container_name)){
			if(is_container_running(container_name)){
				print("[%s Warnning %s] container is running",ORANGE,NC);
			} else {
				system(strjoin("docker rm %s",container_name));
				print("[%s OK %s] container %s has been deleted",GREEN,NC,container_name);
			}
        } else {
            print("[%s OK %s] container %s was already not installed",GREEN,NC,container_name);
        }
    }
}

char* get_container_ip(const char *container_name){
    static char ip[20];
    FILE *fp = popen(strjoin("docker inspect %s | grep \"IPAddress\" | cut -d '\"' -f 4 | tr -d '\\n'",container_name), "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening pipe.\n");
        pclose(fp);
        return "\0";
    }
    if (fgets(ip,20,fp) == NULL){
        fprintf(stderr, "Error reading IP address\n");
        pclose(fp);
        return "\0";
    }
    pclose(fp);
    ip[strcspn(ip,"\n")] = '\0';
    return ip;
}

void get_container_logs(const char *container_name){
    FILE *fp = popen(strjoin("docker logs %s", container_name), "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening pipe\n");
        exit(1);
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        print("%s", buffer);
    }
    pclose(fp);
}

void enter_container(const char *container_name){
    system(strjoin("docker exec -it %s sh", container_name));
    print("\033[32mExited from container %s.\033[0m", container_name); 
}

void twtree(){
    system(strjoin("/usr/bin/tree /home/%s/twdash/sites -L 1",getenv("USER")));
}

void rmf(const char *container_alias){
    system(strjoin("rm -rf /home/%s/twdash/admin/nginx/%s",getenv("USER"),container_alias));
    system(strjoin("rm -rf /home/%s/twdash/admin/ssl/%s",getenv("USER"),container_alias));
    system(strjoin("rm -rf /home/%s/twdash/sites/%s",getenv("USER"),container_alias));
    if (access(strjoin("rm -rf /home/%s/twdash/admin/nginx/%s",getenv("USER"),container_alias), F_OK) != -1)
        print("[%s Warnning %s] directory /home/%s/twdash/admin/nginx/%s still exists",ORANGE,NC,getenv("USER"),container_alias);
    else
        print("[%s OK %s] directory /home/%s/twdash/admin/nginx/%s had been deleted succefully",GREEN,NC,getenv("USER"),container_alias);
    if (access(strjoin("rm -rf /home/%s/twdash/admin/ssl/%s",getenv("USER"),container_alias), F_OK) != -1)
        print("[%s Warnning %s] directory /home/%s/twdash/admin/ssl/%s it still exists",ORANGE,NC,getenv("USER"),container_alias);
    else
        print("[%s OK %s] directory /home/%s/twdash/admin/ssl/%s had been deleted succefully",GREEN,NC,getenv("USER"),container_alias);
    if (access(strjoin("rm -rf /home/%s/twdash/sites/%s",getenv("USER"),container_alias), F_OK) != -1)
        print("[%s Warnning %s] directory /home/%s/twdash/sites/%s it still exists",ORANGE,NC,getenv("USER"),container_alias);
    else
        print("[%s OK %s] directory /home/%s/twdash/sites/%s had been deleted succefully",GREEN,NC,getenv("USER"),container_alias);
}

int is_image(const char *image_name_id){
	if(system(strjoin("docker image inspect %s >/dev/null 2>&1",image_name_id)) == 0)
		return 1;
	return 0;
}

void rmi(const char *container_name){
    if (strcmp(container_name, "--all") == 0) {
        system("docker rmi -f $(docker images -q)");
        print("[%s OK %s] all docker images have been stopped and removed",GREEN,NC);
    } else {
        system(strjoin("docker rmi %s",container_name));
		if(is_image(container_name))
        	print("[%s Warnning %s] docker image %s has not been removed",ORANGE,NC,container_name);
        else
			print("[%s OK %s] docker image %s have been removed",GREEN,NC,container_name);
    }
}

void upload_db_backup(const char *db_name, const char *file_path){
    system(strjoin("docker cp %s twdash-mysql:/home/", file_path));
    system(strjoin("docker exec -it twdash-mysql sh -c \"mysql -u root -p %s < /home/%s\"", db_name, file_path));
    system(strjoin("docker exec -it twdash-mysql sh -c \"rm /home/%s\"", file_path));
    print("[%s OK %s] database %s has been updated",GREEN,NC,db_name);
}

void db_backup(const char *db_name) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    if (tm_info == NULL) {
        perror("Error getting current date and time");
        return;
    }

    int year = tm_info->tm_year + 1900;
    int month = tm_info->tm_mon + 1;
    int day = tm_info->tm_mday;
    char *file_name = strjoin("%s_%04d%02d%02d.sql", db_name, year, month, day);
    int status = 1;
    size_t buffer_size = snprintf(NULL, 0,"docker exec -it twdash-mysql sh -c \"mysqldump -u root -p %s > /home/%s\"", db_name, file_name) + 1;
    char *d = malloc(buffer_size);
    if (d != NULL) {
        snprintf(d, buffer_size,"docker exec -it twdash-mysql sh -c \"mysqldump -u root -p %s > /home/%s\"", db_name, file_name);
        status = system(d);
        free(d);
    }
    if (status != 0) {
        fprintf(stderr, "Error: Failed to create database backup.\n");
        return;
    } else {
        size_t buffer_size = snprintf(NULL, 0,"docker cp twdash-mysql:/home/%s ./", file_name) + 1;
        char *e = malloc(buffer_size);
        if (e != NULL) {
            snprintf(e, buffer_size,"docker cp twdash-mysql:/home/%s ./", file_name);
            system(e);
            free(e);
        }
        size_t buffer_size2 = snprintf(NULL, 0,"docker exec -it twdash-mysql sh -c \"rm /home/%s\"",file_name) + 1;
        char *f = malloc(buffer_size2);
        if (f != NULL) {
            snprintf(f, buffer_size2,"docker exec -it twdash-mysql sh -c \"rm /home/%s\"",file_name);
            system(f);
            free(f);
        }
        print("%sbackup file %s has been downloaded%s", GREEN, file_name, NC);
    }
}

void db_table_backup(const char *db_name, const char *table_name){
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    if (tm_info == NULL) {
        perror("Error getting current date and time");
        return;
    }

    int year = tm_info->tm_year + 1900;
    int month = tm_info->tm_mon + 1;
    int day = tm_info->tm_mday;

    char file_name[256];
    snprintf(file_name, sizeof(file_name), "%s_%04d%02d%02d.sql", db_name, year, month, day);

    int status = 1;
    size_t buffer_size = snprintf(NULL, 0,"docker exec -it twdash-mysql sh -c \"mysqldump -u root -p %s %s > /home/%s\"", db_name, table_name, file_name) + 1;
    char *d = malloc(buffer_size);
    if (d != NULL) {
        snprintf(d, buffer_size,"docker exec -it twdash-mysql sh -c \"mysqldump -u root -p %s %s > /home/%s\"", db_name, table_name, file_name);
        status = system(d);
        free(d);
    }
    if (status != 0) {
        fprintf(stderr, "Error: Failed to create database backup.\n");
        return;
    } else {
        size_t buffer_size = snprintf(NULL, 0,"docker cp twdash-mysql:/home/%s ./", file_name) + 1;
        char *e = malloc(buffer_size);
        if (e != NULL) {
            snprintf(e, buffer_size,"docker cp twdash-mysql:/home/%s ./", file_name);
            system(e);
            free(e);
        }
        size_t buffer_size2 = snprintf(NULL, 0,"docker exec -it twdash-mysql sh -c \"rm /home/%s\"",file_name) + 1;
        char *f = malloc(buffer_size2);
        if (f != NULL) {
            snprintf(f, buffer_size2,"docker exec -it twdash-mysql sh -c \"rm /home/%s\"",file_name);
            system(f);
            free(f);
        }
        print("%sbackup file %s has been downloaded%s",GREEN,file_name,NC);
    }
}

void db_enter(){
    if (system("docker exec -it twdash-mysql sh -c 'mysql -u root -p '")!=0)
        print("[%s Warnning %s] failed to enter mysql database",ORANGE,NC);
}

void db_enter_table(const char* db_name){
    if (system(strjoin("docker exec -it twdash-mysql sh -c \"mysql -u root -p %s \"",db_name))!=0)
        print("[%s Warnning %s] failed to enter database",ORANGE,NC);
}



