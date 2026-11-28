#!/bin/sh
set -e

# if a game folder is mounted at /game, build it next to the examples
if [ -f /game/CMakeLists.txt ]; then
    echo "Building your game from /game..."
    emcmake cmake -S /game -B /tmp/game-build -D32BLIT_DIR=/opt/32blit-sdk -DCMAKE_BUILD_TYPE=Release
    cmake --build /tmp/game-build -j"$(nproc)"
    find /tmp/game-build -maxdepth 1 \( -name '*.html' -o -name '*.js' -o -name '*.wasm' \) -exec cp {} /site/ \;
fi

# a plain list of every game we built
{
    echo '<!doctype html><meta charset="utf-8"><title>SPS SDK games</title>'
    echo '<style>body{font-family:system-ui,sans-serif;max-width:40rem;margin:3rem auto;padding:0 1rem}a{color:#5D48AD}</style>'
    echo '<h1>SPS SDK games</h1><p>Arrows or WASD move. Z, X, C and V are A, B, X and Y. Esc opens the menu.</p><ul>'
    for f in /site/*.html; do
        name=$(basename "$f" .html)
        [ "$name" = index ] || echo "<li><a href=\"$name.html\">$name</a></li>"
    done
    echo '</ul>'
} > /site/index.html

echo "Open http://localhost:8080 in your browser."
cd /site
exec python3 -m http.server 8080
