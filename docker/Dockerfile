FROM debian:buster
RUN apt update && apt install -y cmake g++ python python-future python-lxml
COPY . /decode_example
RUN cd /decode_example && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j4 && \
    cp bin/decode_example bin/encode_example /bin && \
    cd / && \
    rm -rf decode_example
    
