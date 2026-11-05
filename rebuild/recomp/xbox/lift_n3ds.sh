#!/bin/sh
# Lifts default.xbe for the New 3DS: lift.sh plus byte patches for the 32-bit guest memory layout.
#   lift_n3ds.sh <fable_recomp> <iso folder>/default.xbe <output folder> [lifter options]
#
# Small-block allocator (init 0x13340/0x13390, allocate 0x133F0, free 0x13460/0x138D0/0x13960/0xAB970):
# each of its 32 size classes gets 1 MB instead of 16 MB, so the whole range is 32 MB instead of
# 512 MB and fits the 96 MB guest window. hle_SmallBlockAlloc stops with a message if a class fills.
LIFTER="$1"; XBE="$2"; OUT="$3"; shift 3
exec "$(dirname "$0")/lift.sh" "$LIFTER" "$XBE" "$OUT" "$@" \
  --patch 0x13357=000000FE \
  --patch 0x13361=00000002 \
  --patch 0x13373=00000002 \
  --patch 0x13384=00000002 \
  --patch 0x13393=00001000 \
  --patch 0x13424=00001000 \
  --patch 0x13430=14 \
  --patch 0x1346D=14 \
  --patch 0x13477=14 \
  --patch 0x138F0=14 \
  --patch 0x1398A=14 \
  --patch 0xAB984=14 \
  --patch 0xAB995=14
