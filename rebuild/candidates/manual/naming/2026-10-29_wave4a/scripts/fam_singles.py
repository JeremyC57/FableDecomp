#!/usr/bin/env python3
"""w4a family SINGLES: retail 0x0099B770 is CWideString::SetAsSubString(const wchar_t*, long) (PDB ?SetAsSubString@CWideString@@QAEXPB_WJ@Z, G form ...PBGJ@Z); the 0x0054F8E0 model called it Assign.
Also the word-wrap helpers to their FableWin.pdb types in G form: WordWrap_SetCallback 0x00A0FD90 (unsigned int (__fastcall*)(wchar_t), unsigned int (__fastcall*)()), WordWrap_FindNextLineW 0x00A10390 (const wchar_t*, unsigned int, const wchar_t**), and the 0x0054F8E0 callback MeasureText -> CText::GetCharWidthW 0x0054F4B0 (only its address is taken); 0x0048DC8B unsigned long width -> unsigned int.
Exact blocks: singles.reps.json (keyed by repo-relative path; derived from the verified hand-edited copies by tools/mkreps.py).
usage: fam_singles.py [--check] <file>...   (idempotent; a file whose block is in neither form is reported and left alone)
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import w4lib
if __name__ == '__main__':
    w4lib.main('singles', sys.argv[1:])
