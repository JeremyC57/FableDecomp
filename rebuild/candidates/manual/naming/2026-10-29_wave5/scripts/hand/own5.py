# wave 5 own-identity re-matches (whole-file replacements from naming/w5/own/; HE_OV=<overlay>). Each row's own
# function becomes the retail one; the files keep their paths here (RENAMES.tsv proposes the new names):
#   0x00404280 std::operator==(const basic_string<wchar_t>&, const wchar_t*)    own/src404.cpp + own/test404.cpp
#   0x0040F0A0 std::operator==(const basic_string<wchar_t>&, const basic_string<wchar_t>&)   own/src40f.cpp (the test
#              is a self-contained reimplementation and does not link the row; unchanged)
#   0x00993F00 CDataOutputStream::WriteNullTerminatedWString(const CWideString&)  own/src993.cpp (needs fable_string.h.w5;
#              the test is self-contained; unchanged)
import os, sys, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import path
O = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'own')
for a, k, f in (('00404280', 'src', 'src404.cpp'), ('00404280', 'tst', 'test404.cpp'), ('0040f0a0', 'src', 'src40f.cpp'),
                ('00993f00', 'src', 'src993.cpp')):
    p, rel = path(a, k)
    new = open(os.path.join(O, f), 'rb').read()
    if open(p, 'rb').read() == new: print('already', a, k); continue
    open(p, 'wb').write(new); print('ok', a, k)
