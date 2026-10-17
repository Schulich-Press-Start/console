# builds 32blit sdk games for the browser, so anyone can try the sdk with only docker installed.
# 2.0.18 is the emscripten version 32blit's own ci builds with
FROM emscripten/emsdk:2.0.18

# the 32blit asset tools crash with bitstring 5. python3-dev is there because bitstring 4
# pulls in bitarray, which has to compile from source on this image's python 3.8
RUN apt-get update \
    && apt-get install -y --no-install-recommends python3-pip python3-dev \
    && rm -rf /var/lib/apt/lists/* \
    && python3 -m pip install --no-cache-dir --upgrade pip \
    && python3 -m pip install --no-cache-dir 32blit "bitstring<5"

# our sdk fork, plus upstream's examples pinned to a commit we've tested
ARG SDK_REPO=https://github.com/Schulich-Press-Start/32blit-sdk
ARG SDK_REF=master
ARG EXAMPLES_REF=1428fdf78214bcfaeaccbc40eefce25d0d8c8a1d
RUN git clone "$SDK_REPO" /opt/32blit-sdk \
    && git -C /opt/32blit-sdk checkout "$SDK_REF" \
    && git clone https://github.com/32blit/32blit-examples /opt/32blit-examples \
    && git -C /opt/32blit-examples checkout "$EXAMPLES_REF"

# emscripten needs an absolute 32BLIT_DIR, even for the examples
ARG EXAMPLES="raycaster doom-fire voxel tunnel flight"
RUN emcmake cmake -S /opt/32blit-examples -B /opt/examples-build \
        -D32BLIT_DIR=/opt/32blit-sdk -DCMAKE_BUILD_TYPE=Release \
    && cmake --build /opt/examples-build -j"$(nproc)" --target $EXAMPLES \
    && mkdir -p /site \
    && for e in $EXAMPLES; do cp /opt/examples-build/$e/$e.html /opt/examples-build/$e/$e.js /opt/examples-build/$e/$e.wasm /site/; done

COPY entrypoint.sh /usr/local/bin/sdk-entrypoint
RUN chmod +x /usr/local/bin/sdk-entrypoint
EXPOSE 8080
ENTRYPOINT ["sdk-entrypoint"]
