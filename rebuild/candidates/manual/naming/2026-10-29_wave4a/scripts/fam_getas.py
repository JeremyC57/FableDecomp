#!/usr/bin/env python3
"""w4a family GETAS: retail 0x0099AF10 is CWideString::GetAsCharArray() const (Ego_r/FableWin `?GetAsCharArray@CWideString@@QBEPB_WXZ`,
G form `?GetAsCharArray@CWideString@@QBEPBGXZ`, owner decision 2026-10-29). These rows modelled it as `operator const wchar_t*() const`
(`??BCWideString@@QBEPBGXZ`, the spelling of its twin 0x0099AF00) and converted implicitly. The edit renames the local model's
member (and the test fakes' out-of-line body) and makes each call explicit: `x.GetAsCharArray()` for an implicit conversion or a
`(const wchar_t*)x` cast. The call target and argument order are unchanged, so the code bytes are unchanged.
Exact blocks: fam_getas.reps.json (keyed by repo-relative path). usage: fam_getas.py [--check] <file>...
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import w4lib
if __name__ == '__main__':
    w4lib.main('getas', sys.argv[1:])
