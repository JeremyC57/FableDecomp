#!/bin/sh
# Fable: The Lost Chapters (recompiled) for the RG34XX SP. This script's files live in the
# folder of the same name next to it (Fable.sh -> Fable/). Put the PC game's files (the folder
# with Fable.exe, data\, ...) in Fable/game/, then launch this script.
HERE=$(cd "$(dirname "$0")" && pwd)
NAME=$(basename "$0" .sh)
DIR="$HERE/$NAME"
cd "$DIR" || exit 1
export FABLE_RECOMP_DATA="$DIR/save"          # saves, profile and settings live here
export SDL_VIDEODRIVER="${SDL_VIDEODRIVER:-KMSDRM}"
export FABLE_RENDERER="${FABLE_RENDERER:-gles}"     # Direct3D 9 on OpenGL ES (the Mali-G31 has no Vulkan driver here)
export SDL_AUDIODRIVER="${SDL_AUDIODRIVER:-alsa}"
export DXVK_WSI_DRIVER=SDL2
export DXVK_LOG_LEVEL="${DXVK_LOG_LEVEL:-warn}"
export FABLE_RECOMP_LOG="${FABLE_RECOMP_LOG:-1}"
mkdir -p "$DIR/save"
# GBM/EGL/GLES come from the OS's GPU driver (the game looks for them itself); Mesa's copies
# in lib/fallback (software rendering) are used only with FABLE_GL_FALLBACK=1
LIBS="$DIR/lib"
[ "$FABLE_GL_FALLBACK" = 1 ] && LIBS="$LIBS:$DIR/lib/fallback"
[ -f "$DIR/game/FableRecomp_controller.ini" ] || cp "$DIR/FableRecomp_controller.ini.default" "$DIR/game/FableRecomp_controller.ini" 2>/dev/null
exec "$DIR/lib/ld-linux-aarch64.so.1" --library-path "$LIBS" "$DIR/FableRecomp" \
  --game "$DIR/game" --fullscreen --ui-scale=0 "$@" > "$DIR/log.txt" 2>&1
