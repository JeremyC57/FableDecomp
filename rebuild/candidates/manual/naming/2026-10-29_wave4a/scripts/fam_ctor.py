#!/usr/bin/env python3
"""w4a family CTOR: retail 0x0099B6B0 is CWideString::CWideString(const wchar_t*) (G ??0CWideString@@QAE@PBG@Z) and 0x0099BF30 operator+(const CWideString&, const wchar_t*): const char* models take wide text (literals L"..."), and the namespaced CWideString models (NPlayerGui, Row00e15500) become the global class.
Exact blocks: ctor.reps.json (keyed by repo-relative path; derived from the verified hand-edited copies by tools/mkreps.py).
usage: fam_ctor.py [--check] <file>...   (idempotent; a file whose block is in neither form is reported and left alone)
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import w4lib
if __name__ == '__main__':
    w4lib.main('ctor', sys.argv[1:])
