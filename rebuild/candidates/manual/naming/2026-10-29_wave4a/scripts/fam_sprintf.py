#!/usr/bin/env python3
"""w4a family SPRINTF: retail 0x0099BA70 is CWideString::Sprintf(const wchar_t*, ...) (__cdecl varargs member; PDB ?Sprintf@CWideString@@QAAXPB_WZZ, G form ...PBGZZ). Member Format and the free FormatWideString(CWideString*, fmt, ...) stand-ins respelled; the free form pushes the string first exactly as the member's this, so the bytes are unchanged.
Exact blocks: sprintf.reps.json (keyed by repo-relative path; derived from the verified hand-edited copies by tools/mkreps.py).
usage: fam_sprintf.py [--check] <file>...   (idempotent; a file whose block is in neither form is reported and left alone)
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import w4lib
if __name__ == '__main__':
    w4lib.main('sprintf', sys.argv[1:])
