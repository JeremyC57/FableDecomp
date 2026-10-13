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
export FABLE_PAD_MAPPINGS="$DIR/gamecontrollerdb.txt"   # the built-in pad's SDL mapping
export FABLE_OVERLAY=1          # the pad's menu/function key (or Back+Start held) opens the host menu
export FABLE_HOST_GFX=1         # graphics settings live in the host menu; the game's Video Options entry is inert
# first-run graphics preset for this device (save/graphics.txt keeps what the host menu sets afterwards):
# shadows and water reflections are whole extra scene passes, the biggest cost on the Mali-G31
export FABLE_GFX_DEFAULTS="${FABLE_GFX_DEFAULTS:-resolution_scale=100,refresh_rate=0,antialiasing=0,aspect_ratio=0,landscape_detail=1,model_detail=1,texture_detail=1,shadow_quality=0,effects_quality=1,brightness=50,vsync=0,draw_distance=100,glow=1,water_reflection=0,weather=1}"
export FABLE_PAD_QUIT=1         # Back+Start held for two seconds exits
export FABLE_VOLUME_KEYS=1      # the volume rocker sets the game's master volume
export FABLE_PROFILE="${FABLE_PROFILE:-60}"  # sampling profiler: the hottest functions every 60 s in log.txt (0 = off)
mkdir -p "$DIR/save/Documents/My Games/Fable/Saves/Default"  # the game only probes the save tree, it never creates it
# Clocks: the stock OS idles the H700 down; run the CPU and the GPU at their performance
# governors for the session (restored on exit). FABLE_NO_PERF=1 leaves them alone.
restore_governors() {
  [ -n "$OLD_CPU_GOV" ] && for g in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor; do echo "$OLD_CPU_GOV" > "$g" 2>/dev/null; done
  [ -n "$OLD_GPU_GOV" ] && for g in /sys/class/devfreq/*/governor; do echo "$OLD_GPU_GOV" > "$g" 2>/dev/null; done
}
if [ "$FABLE_NO_PERF" != 1 ]; then
  OLD_CPU_GOV=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 2>/dev/null)
  for g in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor; do echo performance > "$g" 2>/dev/null; done
  OLD_GPU_GOV=$(cat /sys/class/devfreq/*/governor 2>/dev/null | head -1)
  for g in /sys/class/devfreq/*/governor; do echo performance > "$g" 2>/dev/null; done
  trap restore_governors EXIT
fi
# GBM/EGL/GLES come from the OS's GPU driver (the game looks for them itself); Mesa's copies
# in lib/fallback (software rendering) are used only with FABLE_GL_FALLBACK=1
LIBS="$DIR/lib"
[ "$FABLE_GL_FALLBACK" = 1 ] && LIBS="$LIBS:$DIR/lib/fallback"
[ -f "$DIR/game/FableRecomp_controller.ini" ] || cp "$DIR/FableRecomp_controller.ini.default" "$DIR/game/FableRecomp_controller.ini" 2>/dev/null
"$DIR/lib/ld-linux-aarch64.so.1" --library-path "$LIBS" "$DIR/FableRecomp" \
  --game "$DIR/game" --fullscreen --ui-scale=0 "$@" > "$DIR/log.txt" 2>&1
restore_governors
