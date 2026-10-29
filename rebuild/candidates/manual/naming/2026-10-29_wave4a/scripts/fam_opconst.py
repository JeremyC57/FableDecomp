#!/usr/bin/env python3
"""w4a family OPCONST: retail 0x0099AF00 is CWideString::operator const wchar_t*() const (PDB ??BCWideString@@QBEPB_WXZ, G form ??BCWideString@@QBEPBGXZ): const added to non-const models; 0x0099AF10 stand-ins (operator unsigned short*, c_str, GetText@CWideStringCalls) respelled GetAsCharArray() const; GetChars_0099af00 respelled operator const wchar_t*.
Exact blocks: opconst.reps.json (keyed by repo-relative path; derived from the verified hand-edited copies by tools/mkreps.py).
usage: fam_opconst.py [--check] <file>...   (idempotent; a file whose block is in neither form is reported and left alone)
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import w4lib
if __name__ == '__main__':
    w4lib.main('opconst', sys.argv[1:])
