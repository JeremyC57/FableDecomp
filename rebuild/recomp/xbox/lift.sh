#!/bin/sh
# Lifts the Xbox default.xbe with the options the Xbox host expects.
#   lift.sh <fable_recomp> <iso folder>/default.xbe <output folder>
set -e
LIFTER="$1"; XBE="$2"; OUT="$3"
exec "$LIFTER" "$XBE" - "$OUT" \
  --mmio 0x84E460-0x863200 \
  --safepoints \
  --hook 0x86ED4B=hle_XInitDevices \
  --hook 0x86FFFF=hle_XGetDevices \
  --hook 0x870021=hle_XGetDeviceChanges \
  --hook 0x8700F0=hle_XInputOpen \
  --hook 0x870146=hle_XInputClose \
  --hook 0x870152=hle_XInputGetState \
  --hook 0x8701C5=hle_XInputSetState
