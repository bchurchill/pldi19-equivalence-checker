FROM bchurchill/pldi19-baseimage
MAINTAINER Berkeley Churchill (berkeley@cs.stanford.edu)

COPY . /home/equivalence/equivalence-checker
RUN apt-get update && apt-get upgrade -y
RUN chown -R equivalence /home/equivalence/equivalence-checker && \
    chmod +x /home/equivalence/equivalence-checker/scripts/docker/user-setup.sh && \
    su -c "/home/equivalence/equivalence-checker/scripts/docker/user-setup.sh" equivalence
