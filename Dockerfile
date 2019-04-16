FROM ubuntu:14.04
MAINTAINER Berkeley Churchill (berkeley@cs.stanford.edu)

# SSH setup
CMD ["/usr/sbin/sshd", "-D"]
EXPOSE 22
ENV NOTVISIBLE "in users profile"
RUN useradd -ms /bin/bash -ms /bin/bash equivalence

# Build everything 
COPY setup.sh user-setup.sh pldi19-traces.tar.gz /home/equivalence/
RUN chmod +x /home/equivalence/setup.sh && \
    /home/equivalence/setup.sh && \
    rm /home/equivalence/setup.sh \
       /home/equivalence/user-setup.sh
