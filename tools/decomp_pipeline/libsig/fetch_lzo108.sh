#!/usr/bin/env bash
# Fetch the LZO 1.08 source tree (public: https://www.oberhumer.com/opensource/lzo/, GPL) that retail Fable.exe links:
# the shipped MASM byte tables of src/i386/d_asm2/*.asm are the bytes of 0x00C069D0 (_lzo1x_decompress_asm_fast).
# lib_verify.py proves such rows from this tree (docs/pipeline/LIBRARY_SIGNATURES.md, "Third-party assembled objects").
#   usage: bash tools/decomp_pipeline/libsig/fetch_lzo108.sh [DEST]   (default $S/thirdparty; tree lands in DEST/lzo/lzo-1.08)
# Then (optional) export FABLE_THIRDPARTY=DEST. The tarball is checked against the SHA-256 below. Never commit the tree.
set -euo pipefail
DEST=${1:-${S:?set S or pass DEST}/thirdparty}
URL="https://www.oberhumer.com/opensource/lzo/download/LZO-v1/lzo-1.08.tar.gz"
SHA256=bcead46b0bd887c3987e977a0a7d4c547b0f70f926d45e253784137569f57b9a
mkdir -p "${DEST:?}/dl" "${DEST:?}/lzo"
tgz="${DEST:?}/dl/lzo-1.08.tar.gz"
if ! { [ -f "$tgz" ] && [ "$(sha256sum "$tgz" | cut -d' ' -f1)" = "$SHA256" ]; }; then
  curl -sS -L --retry 2 --max-time 600 -o "$tgz.part" "$URL"
  got=$(sha256sum "$tgz.part" | cut -d' ' -f1)
  [ "$got" = "$SHA256" ] || { echo "SHA-256 MISMATCH lzo-1.08.tar.gz ($got, want $SHA256): left as $tgz.part" >&2; exit 1; }
  mv "$tgz.part" "$tgz"
fi
tar -xzf "$tgz" -C "${DEST:?}/lzo"
echo "ok    ${DEST:?}/lzo/lzo-1.08 (src/i386/d_asm2/lzo1x_f1.asm)"
