#!/usr/bin/env python3
"""Respell file-scope stand-in names of the two retail roots (`CBaseClassNative`, `CBase_0099a2d0`) to the PDB classes.

Same root rules as fam_cbase_ph.py (it supplies the transform): A = CBaseClass (ctor 0x0099A2D0 QAE, dtor 0x0099A2E0
UAE, public), N = CBaseClassNonCopyable (ctor 0x0099A2F0 IAE, dtor 0x0099A300 MAE, protected). The root may be a
placeholder root (`virtual void v00(); X();` / `virtual void v00(); ~X();`) or already carry a virtual dtor
(`X(); virtual ~X();`, ctor optionally with an empty inline body). ALIAS gives each row's stand-in name and root:
the root is the row's relocation target as recorded by the wave-2A deferral (naming/w2a/DEFERRED.tsv: the stand-in
symbol bound to 0x0099A2D0 / 0x0099A2E0 -> A). Idempotent (a second run finds no stand-in name).
usage: fam_cbase_alias.py [--check] <file>...
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import row_of, run
from fam_cbase_ph import respell, A, N

ALIAS = {
    # CBase_0099a2d0: ctor 0x0099A2D0 / dtor 0x0099A2E0 targets
    **{a: ('CBase_0099a2d0', A) for a in '0041be50 00437260 0048f50c 004c2dd0 004c31e0 006a6b80 007e8310 008fd255 009fdb80 00a00600 00ab37f0'.split()},
    # CBaseClassNative: ctor 0x0099A2D0 / dtor 0x0099A2E0 targets
    **{a: ('CBaseClassNative', A) for a in (
        '008976f0 0089f910 0089f9e0 0089faa0 0089fbb0 008a23b0 008a24c0 008a2590 008a2770 008a2a30 008a3a60 008a3fd0 '
        '008a41b0 008a4390 008a4730 008a4910 008a4ae0 008a5300 008a55c0 008a6570 008a6750 008a6840 008a6a20 008a76f0 '
        '008a77f0 008a7e00 00d1e310 00d9c2b0 00f00ae0').split()},
}


def transform(text, path):
    a, is_test = row_of(path)
    if a not in ALIAS:
        return text, 0, []
    old, new = ALIAS[a]
    return respell(text, a, is_test, old, new, plain=True)


if __name__ == '__main__':
    run(transform, sys.argv[1:])
