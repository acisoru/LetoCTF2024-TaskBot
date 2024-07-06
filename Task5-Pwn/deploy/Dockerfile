FROM ubuntu:22.04

RUN apt-get -y update
RUN apt-get -y install socat

RUN useradd -m ctf
RUN mkdir /app
WORKDIR /app

COPY ./ /app
RUN chmod +x ./run.sh

USER ctf

EXPOSE 1557
CMD ./run.sh

