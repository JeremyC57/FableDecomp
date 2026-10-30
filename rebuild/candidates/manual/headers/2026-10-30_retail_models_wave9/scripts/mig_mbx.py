#!/usr/bin/env python3
"""Wave 8 family MBX: MovieBase rows whose per-site expansion VC7.1 reproduces by itself (scripts/mbexp.py SAME): drop
the `#pragma inline_depth` scaffolding, the declared `~CScriptGameResourceObjectMovieBase();` (the retail dtor is the
implicit one: engine/CScriptGameResourceObjectMovieBase.h) and the `__forceinline` on the CCountedPointer release dtor
(retail expands it at some sites and calls the COMDAT 0x006E7AB0 at others, which a forced inline cannot give).
usage: mig_mbx.py [--dry] <file>..."""
import sys, re
for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
    t = open(f, encoding="utf-8").read()
    v = "\n".join(l for l in t.split("\n") if not l.strip().startswith("#pragma inline_depth"))
    v = re.sub(r"^\s*~CScriptGameResourceObjectMovieBase\s*\(\s*\)\s*;[^\n]*\n", "", v, flags=re.M)
    v = v.replace("__forceinline ~CCountedPointer", "~CCountedPointer")
    if v == t: print("skip", f, "nochange"); continue
    if "--dry" not in sys.argv: open(f, "w", encoding="utf-8", newline="\n").write(v)
    print("edit", f, "ok")
