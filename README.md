# twdash-local version 1.4.1
local server with containers and auto ssl

twdash-local is the reduce version of twdash, twdash-local was created to have containers and auto ssl ready with one or two commands, without needing to have the knowledge of creating the containers network or even the lemp server, anyhow this knowledge is beneficial long term, so as possible all docker commands had stay identical for this same reason.  its open-source, feedbacks are welcome.  

Ubuntu and Debian repositories have a version of docker that still doesnt allow sudoless commands of docker, this way twdash installs directly from the official page of docker the latest stable realease.
For this same reason only works on Debian 12 and forward enviroments. 
On Ubuntu and Fedora enviroments posssibly the same, still on tests. be free to drop feedback.

