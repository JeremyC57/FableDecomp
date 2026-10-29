#!/usr/bin/env python3
"""w4a family ZCDROP: the seven rows that compiled with /Zc:wchar_t (FLAGS.tsv drops the flag, owner 2026-10-29): Leaf lines and comments respelled to the G form, <stddef.h> added where wchar_t had been the native keyword.
Exact blocks: zcdrop.reps.json (keyed by repo-relative path; derived from the verified hand-edited copies by tools/mkreps.py).
usage: fam_zcdrop.py [--check] <file>...   (idempotent; a file whose block is in neither form is reported and left alone)
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import w4lib
if __name__ == '__main__':
    w4lib.main('zcdrop', sys.argv[1:])
