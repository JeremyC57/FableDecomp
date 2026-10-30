#!/usr/bin/env bash
# Fetch the VS .NET 2003 (cl 13.10.3077 RTM) Vc7/lib files the libsig tools need, one file at a time, from the same
# archive.org item the lane's RTM cl.exe came from (docs/BUILDING.md): archive.org serves single members of
# VisualStudio2003.zip, so the 2 GB zip is not downloaded. Every file is checked against the SHA-1 below.
#   usage: bash tools/decomp_pipeline/libsig/fetch_vs2003_libs.sh [DEST]   (default $S/wine/vs2003lib)
# Then point the tools at it: export FABLE_VS2003_LIB=DEST (libsig_scan.py / lib_verify.py also look in
# $S/wine/vs2003lib and $S/libsig/work/vs2003_lib). These are Microsoft binaries: never commit them.
set -euo pipefail
DEST=${1:-${S:?set S or pass DEST}/wine/vs2003lib}
BASE="https://archive.org/download/visual-studio-2003-unpacked/VisualStudio2003.zip/VisualStudio2003%2FProgram%20Files%2FMicrosoft%20Visual%20Studio%20.NET%202003%2FVc7%2Flib%2F"
mkdir -p "${DEST:?}"
fail=0
while read -r sha name; do
  [ -z "${name:-}" ] && continue
  out="${DEST:?}/${name:?}"
  if [ -f "$out" ] && [ "$(sha1sum "$out" | cut -d' ' -f1)" = "$sha" ]; then echo "ok    $name"; continue; fi
  curl -sS -L --retry 2 --max-time 600 -o "$out.part" "$BASE$name"
  got=$(sha1sum "$out.part" | cut -d' ' -f1)
  if [ "$got" = "$sha" ]; then mv "$out.part" "$out"; echo "fetch $name"
  else echo "SHA-1 MISMATCH $name ($got, want $sha): left as $out.part" >&2; fail=1; fi
done <<'LIST'
1985f22ebd09f513d163d2d7bc1b4a18c85e0f90 RunTmChk.lib
b67703f1911b6259d2ef490105737d8e48ca2e8b binmode.obj
90326b67cf0a5b3f5e466f57db3e6c237502a5e0 chkstk.obj
4f62c53d56552754625f6055be37c4a46b447e09 commode.obj
717d7eda71baf5b8dbc47982cf185dce8106ba5a comsupp.lib
90fe76db774c93ca5954650e60ff745f574077bf delayimp.lib
8f3fe9cd34a5afc3181d5b048977511468a81470 fp10.obj
645aff7e344df2b72d377862357a71dbb7141ec7 kernel32.lib
1490704dd3c39184d4c5a79307f7df3780ed1c8c largeint.lib
e51bb4c7c542710459e9224d072e97a0b472a08b libc.lib
dbbe7b3fbd6eb9d67c9ca9bfa569d1efd49d46ce libcmt.lib
cb0ebaa2c85a933261cc536d367d416c3333d671 libcp.lib
f3d019ffb8d100002fff494870074c973d6ab050 libcpmt.lib
cf812679f559fb50139bc08a640fd06c6788bf27 msvcprt.lib
612facc50f93b3708633f26e8b42fe4fb5f54d64 msvcrt.lib
0c74ebce6969bc15a719b3ee41812668c7b73071 newmode.obj
4a491f027037ce02a4a60ab45f2365a8a84236b4 nochkclr.obj
27ccf97bb6e67313353ab52b973e96fbef0b3fb7 oldnames.lib
537ecb6154e40715703e7f4884d0f414a356f529 setargv.obj
8fbb0af98655038ac3035ffd17a098dd7061a54c thrownew.obj
8709f9d3c9438bbbe7adfb9f287e457bd1ea75e8 wsetargv.obj
LIST
exit $fail
