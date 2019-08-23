FROM ubuntu:16.04

RUN apt-get update --fix-missing && apt-get install -y file gcc g++ git make wget

RUN cd /opt && git clone https://github.com/anlin00007/OnTAD.git
RUN              cd /opt/OnTAD/src && make clean && make
ENV PATH            /opt/OnTAD/src:${PATH}

CMD /bin/bash
