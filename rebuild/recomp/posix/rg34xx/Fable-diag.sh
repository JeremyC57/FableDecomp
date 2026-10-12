#!/bin/sh
# Collects what the game needs to know about this device's display stack into
# Fable/diag.txt (next to this script: Fable-diag.sh -> Fable/). Launch it like Fable.sh and
# send diag.txt along with log.txt when the game does not start.
HERE=$(cd "$(dirname "$0")" && pwd)
DIR="$HERE/Fable"
OUT="$DIR/diag.txt"
{
  echo "== $(date) =="
  echo "--- system"; uname -a; cat /etc/os-release 2>/dev/null; id
  echo "--- devices"; ls -la /dev/fb* /dev/dri /dev/dri/* /dev/mali* /dev/graphics 2>&1
  echo "--- framebuffer"
  for f in /sys/class/graphics/fb0/*; do [ -f "$f" ] && printf '%s: ' "$(basename "$f")" && cat "$f" 2>/dev/null; done
  command -v fbset >/dev/null && fbset -i 2>&1
  echo "--- kernel modules"; cat /proc/modules 2>/dev/null
  echo "--- GPU/EGL libraries on the system"
  for d in /lib /usr/lib /usr/lib64 /usr/local/lib /usr/lib/aarch64-linux-gnu /usr/lib/arm-linux-gnueabihf /usr/lib/mali /usr/lib/egl /opt /vendor /system/lib64 /mnt/vendor; do
    [ -d "$d" ] && find "$d" -maxdepth 3 \( -name 'libEGL*' -o -name 'libGLES*' -o -name 'libmali*' -o -name 'libMali*' -o -name 'libgbm*' -o -name 'libdrm*' -o -name 'libSDL2*' -o -name 'libvulkan*' -o -name '*_dri.so' \) -exec ls -la {} \; 2>/dev/null
  done
  echo "--- strings in the EGL library (which window system it was built for)"
  for l in /usr/lib/libmali.so /usr/lib/aarch64-linux-gnu/libmali.so /usr/lib/libEGL.so.1 /usr/lib/aarch64-linux-gnu/libEGL.so.1 /usr/lib/libMali.so; do
    [ -e "$l" ] && { echo "$l:"; strings "$l" 2>/dev/null | grep -iE 'fbdev|gbm|wayland|x11|dummy|surfaceless|mali-|r[0-9]+p[0-9]|version' | sort -u | head -40; }
  done
  echo "--- environment"; env | sort
  echo "--- ports launcher libs (PortMaster/stock)"
  ls -la /usr/lib/libSDL2* /roms/ports/PortMaster/libs* /mnt/sdcard/Roms/PORTS/PortMaster/libs* 2>&1 | head -40
  echo "--- clocks (cpufreq / devfreq governors and frequencies)"
  for f in /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq; do printf '%s: ' "$f"; cat "$f" 2>&1; done
  for d in /sys/class/devfreq/*; do [ -d "$d" ] && for f in governor available_governors cur_freq max_freq available_frequencies; do printf '%s/%s: ' "$d" "$f"; cat "$d/$f" 2>&1; done; done
  cat /sys/kernel/debug/mali*/gpu_freq 2>/dev/null; ls /sys/devices/platform/*gpu*/ 2>/dev/null | head; cat /proc/meminfo | head -3
  echo "--- input devices"
  for d in /sys/class/input/event*; do printf '%s: ' "$(basename "$d")"; cat "$d/device/name" 2>/dev/null; done
  echo "--- pad probe: for 30 seconds press, in this order: A B X Y L1 R1 L2 R2 Select Start, D-pad up down left right, L3 R3, the menu key, volume up, volume down; then move the left stick right/down and the right stick right/down"
  FABLE_PAD_MAPPINGS="$DIR/gamecontrollerdb.txt" "$DIR/lib/ld-linux-aarch64.so.1" --library-path "$DIR/lib" "$DIR/FableRecomp" --pad-probe 2>&1
  echo "--- EGL probe (window surface on the framebuffer; the screen should flash red, green, blue)"
  FABLE_RECOMP_LOG=1 FABLE_GL_WINDOW=window "$DIR/lib/ld-linux-aarch64.so.1" --library-path "$DIR/lib" "$DIR/FableRecomp" --gl-probe 2>&1
  echo "--- EGL probe (pbuffer + copy into /dev/fb0)"
  FABLE_RECOMP_LOG=1 FABLE_GL_WINDOW=copy "$DIR/lib/ld-linux-aarch64.so.1" --library-path "$DIR/lib" "$DIR/FableRecomp" --gl-probe 2>&1
  echo "--- EGL probe with Mesa's bundled libraries (software rendering)"
  FABLE_RECOMP_LOG=1 FABLE_GL_WINDOW=copy "$DIR/lib/ld-linux-aarch64.so.1" --library-path "$DIR/lib:$DIR/lib/fallback" "$DIR/FableRecomp" --gl-probe 2>&1
} > "$OUT" 2>&1
echo "wrote $OUT"
