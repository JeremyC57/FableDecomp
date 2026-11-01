# h21_misc.py: naming wave 21, family misc -- 00cba8e0 (the wave-20 FastCompare -> ??MCCharString@@QBE_NABV0@@Z respell in the uninstantiated
# comparator of std::swap; name-only, no symbol), 00c62920 (wave 20 DEFERRED: the decoder-stream factory 0x00C73AF0 takes the name as
# const unsigned short* per chunk 687; the owner ctor and the opener's slot 0 pass it through -- PBD -> PBG in both spellings).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_misc.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
def R(a, k, pat, rep, n=None):
    if a not in _X: he.resub(a, k, pat, rep, n)
R('00cba8e0', 'src', r'a\.first\.FastCompare\(b\.first\)', 'a.first.operator<(b.first)')
R('00cba8e0', 'src', r'CCharString::FastCompare 0x009E6530', 'CCharString::operator< 0x009E6530 (??MCCharString@@QBE_NABV0@@Z, naming wave 20)')
R('00cba8e0', 'src', r'bool FastCompare\(const CCharString& other\) const;', 'bool operator<(const CCharString& other) const;')
a = '00c62920'
for k in ('src',):   # the standalone test keeps its char literals
    R(a, k, r'const char\* name', 'const unsigned short* name')
    R(a, k, r'Open\(const char\*\)', 'Open(const unsigned short*)')
    R(a, k, r'CreateDecoderStream\(Source\*, const char\*,', 'CreateDecoderStream(Source*, const unsigned short*,')
R(a, 'src', r'^(// Leaf: [^\n]*\n)', r'\1// Naming (naming wave 21, wave 20 DEFERRED): the name is a wide string (const unsigned short*): chunk 687 proved the decoder-stream factory 0x00C73AF0 classifies a 16-bit-character extension; the owner ctor and the opener slot 0 pass it through (the landed PBD spellings were wrong).\n', 1)
