#!/usr/bin/env python3
"""w4a family FILESCAN: retail 0x00999760 / 0x00999850 are static CAFile::StartFileScan / ScanNextFile(const CWideString&, CAFile::CFileScanInfo&, wchar_t*) (__fastcall; PDB ...PA_W@Z, G form ...PAG@Z). The free FindFirstFile / FindNextFile stand-ins become the static members, the handle local becomes CAFile::CFileScanInfo (8 bytes, the 0x00999C50 model) and the file-data local passes its FileName buffer (same stack address).
Exact blocks: filescan.reps.json (keyed by repo-relative path; derived from the verified hand-edited copies by tools/mkreps.py).
usage: fam_filescan.py [--check] <file>...   (idempotent; a file whose block is in neither form is reported and left alone)
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import w4lib
if __name__ == '__main__':
    w4lib.main('filescan', sys.argv[1:])
