#!/usr/bin/env python3
"""Respell the retail CRC routine 0x004014A0 to its PDB form `?Calc@CCRC@@SIKKPBXK@Z`
(static unsigned long __fastcall CCRC::Calc(unsigned long, const void*, unsigned long);
Ego_r.pdb + FableWin.pdb; caller InitEntry 0x009CE050).

Landed rows declare a file-scope free stand-in `unsigned long __fastcall <N>(unsigned long seed,
const void|char* data, unsigned long length);` with a 0x004014A0 comment, N in {Calc_004014a0, Calc,
CRC32_Calc, CalculateCRC, CRCCalc}. The stand-in becomes the static member of a file-scope CCRC
(inserted into the file's CCRC model when it has one, else `class CCRC { public: static ... };` in
place of the declaration), calls `N(` -> `CCRC::Calc(`, a test stub definition -> `unsigned long
CCRC::Calc(...)` (only with a `const void*` data parameter). Byte-neutral: same fastcall argument
registers and the same return register; `const char*` -> `const void*` only moves the mangled name.
Comments naming N next to 0x004014A0 become CCRC::Calc. Idempotent.
Not handled: definitions of the routine inside a row (009AD410), the namespaced NErase / NDust /
NFoot models.  usage: fam_ccrc.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, scopes, apply_edits, drop_nested, free_to_static, comment_respell, run

NAMES = ['Calc_004014a0', 'Calc', 'CRC32_Calc', 'CalculateCRC', 'CRCCalc']
U = r'(?:unsigned long|unsigned int|unsigned|long|int)'


def params_ok(p):
    return re.fullmatch(r'%s(?: \w+)?, const (?:void|char) ?\* ?\w*, %s(?: \w+)?' % (U, U), p) is not None


def defparams(p):
    mm = re.fullmatch(r'%s(?: (\w+))?, const void ?\* ?(\w*), %s(?: (\w+))?' % (U, U), p)
    if not mm: return None
    a, b, c = mm.group(1), mm.group(2), mm.group(3)
    return 'unsigned long%s, const void*%s, unsigned long%s' % (' ' + a if a else '', ' ' + b if b else '', ' ' + c if c else '')


SPEC = dict(names=NAMES, ret=r'(?:unsigned\s+long|unsigned\s+int|unsigned)', params=params_ok, cls='CCRC',
            member='static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);',
            comment='// 0x004014A0', qual='CCRC::Calc', defhead='unsigned long CCRC::Calc', defparams=defparams,
            header=r'#\s*include\s*[<"]engine/CCRC\.h[>"]', anchor=r'(?i)0x0*4014A0\b')


def transform(text, path):
    m = mask(text)
    sc = scopes(m)
    edits, notes, done = free_to_static(text, m, sc, SPEC)
    if not done:
        return text, 0, notes
    edits += comment_respell(text, m, NAMES, 'CCRC::Calc', r'(?i)0x0*4014A0\b')
    edits = drop_nested(edits)
    return apply_edits(text, edits), len(edits), notes


if __name__ == '__main__':
    run(transform, sys.argv[1:])
