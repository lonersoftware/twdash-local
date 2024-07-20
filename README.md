# twdash-local version 1.4.4
Local server with containers and auto SSL (CLI).

Twdash-local is a reduced version of twdash. It was created to provide containers and auto SSL capabilities with just one or two commands, without requiring knowledge of container network creation or the LEMP server setup. However, gaining this knowledge is beneficial in the long term. Hence, all Docker commands have remained identical for consistency. It's open-source, and feedback is welcome.

Ubuntu and Debian repositories have versions of Docker that still don't allow sudoless commands for Docker. Therefore, twdash installs directly from the official Docker website, ensuring the latest stable release. Due to this limitation, it only works on Debian 12 and later environments, also in Ubuntu 22.04 enviroments and forward.

Compatibility Fedora environments is still being tested. Feel free to provide feedback.

The CLI's first version is ready to use and should work as expected. Any bugs can be reported as issues on this GitHub repository. The UI version is currently in development and was expected to be released in April 2024, although development has not yet begun.
