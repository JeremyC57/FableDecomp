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
export SDL_AUDIODRIVER="${SDL_AUDIODRIVER:-alsa}"
export DXVK_WSI_DRIVER=SDL2
export DXVK_LOG_LEVEL="${DXVK_LOG_LEVEL:-warn}"
export FABLE_RECOMP_LOG="${FABLE_RECOMP_LOG:-1}"
mkdir -p "$DIR/save"
[ -f "$DIR/game/FableRecomp_controller.ini" ] || cp "$DIR/FableRecomp_controller.ini.default" "$DIR/game/FableRecomp_controller.ini" 2>/dev/null
exec "$DIR/lib/ld-linux-aarch64.so.1" --library-path "$DIR/lib" "$DIR/FableRecomp" \
  --game "$DIR/game" --fullscreen --ui-scale=0 "$@" > "$DIR/log.txt" 2>&1
