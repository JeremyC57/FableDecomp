#!/usr/bin/env python3
"""w4a family COMPARE: retail 0x00403C60 is STLport 4.6 __char_traits_base<wchar_t, wint_t>::compare (static __cdecl, inherited by char_traits<wchar_t>; wchar_t == wint_t == unsigned short) = ?compare@?$__char_traits_base@GG@std@@SAHPBG0I@Z; MemCmp_Unsigned16 / char_traits_w / __char_traits_ushort / char_traits<wchar_t> stand-ins respelled. The 0x00403C60 row itself becomes the explicit specialization template <> int __char_traits_base<unsigned short, unsigned short>::compare (its test calls it). Also the 0x00404280 callers: STLport char_traits is a class template (V).
Exact blocks: compare.reps.json (keyed by repo-relative path; derived from the verified hand-edited copies by tools/mkreps.py).
usage: fam_compare.py [--check] <file>...   (idempotent; a file whose block is in neither form is reported and left alone)
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import w4lib
if __name__ == '__main__':
    w4lib.main('compare', sys.argv[1:])
