# twdash-local version 1.4.1
Local server with containers and auto SSL (CLI).

## how to install and use:
download the binary directly from the website or compile this .c file with gcc and move the .out file with no extension to bin folder.

```gcc -o twd twd.c && sudo mv twd /usr/bin/twd```

then just use as a command egg: "twd ps -a"


## more info
Twdash-local is a reduced version of twdash. It was created to provide containers and auto SSL capabilities with just one or two commands, without requiring knowledge of container network creation or the LEMP server setup. However, gaining this knowledge is beneficial in the long term. Hence, all Docker commands have remained identical for consistency. It's open-source, and feedback is welcome.

Ubuntu and Debian repositories have versions of Docker that still don't allow sudoless commands for Docker. Therefore, twdash-local installs directly from the official Docker website, ensuring the latest stable release. Due to this limitation(that is a plus), it only works on Debian 12 and later environments, also in Ubuntu 22.04 enviroments and forward.

Compatibility Fedora environments is still being tested. Feel free to provide feedback.

The CLI's first version is ready to use and should work as expected. Any bugs can be reported as issues on this GitHub repository. The UI version is currently in development and was expected to be released in April 2024, although development has not yet begun.

Everything is still need it in this small project, documentation, UI, design, translation and so on. however the cli is up and going.
