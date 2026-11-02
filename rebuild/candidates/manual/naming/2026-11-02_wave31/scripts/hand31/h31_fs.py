# h31_fs.py: naming wave 31, family fs (scope 1, DECISIONS_W30 D2 APPROVED): rebuild/include/fable_string.h gains the PDB CCharString copy
#  constructor ??0CCharString@@QAE@ABV0@@Z (Ego_r.pdb / FableWin.pdb; retail 0x0099EC30, landed own row CCharString_CCharStringcopy_0099ec30).
#  Before this the header declared none, so every includer's CCharString copy was the implicit bitwise one; retail calls 0x0099EC30 out of line.
import os, sys
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REL = 'rebuild/include/fable_string.h'
dst = os.path.join(W, os.environ.get('HE_OV', 'ov_hand'), REL)
src = dst if os.path.exists(dst) else os.path.join('/home/user/FableDecomp', REL)
t = open(src, encoding='utf-8', newline='').read()
OLD = '    CCharString(const char* text, long length);\n'
NEW = OLD + '    CCharString(const CCharString& other);                     // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z (Ego_r / FableWin)\n'
if NEW in t: print('already', REL)
else:
    assert t.count(OLD) == 1
    t = t.replace(OLD, NEW)
    os.makedirs(os.path.dirname(dst), exist_ok=True); open(dst, 'w', encoding='utf-8', newline='').write(t); print('ok', REL)
# The two includer tests that copy a CCharString (0x00401C00: the test's operator+ returns its local by value; 0x00A45150: the test's
#  C3DSkeletalBoneMorph copy ctor copies BoneName) now reference the declared copy ctor; each test defines it with the bitwise copy
#  the implicit one did (test stubs; behaviour unchanged). The landed sources are untouched (their recheck is unchanged).
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.edit('00401c00', 'tst', '''CCharString::~CCharString()
{
}
''', '''CCharString::CCharString(const CCharString& other)   // test stub: the bitwise copy the implicit constructor made
{
    Storage(*this) = const_cast<CCharStringData*>(Storage(other));
}

CCharString::~CCharString()
{
}
''')
he.edit('00a45150', 'tst', 'CCharString::~CCharString() {}\n',
        'CCharString::~CCharString() {}\n#ifdef _MSC_VER\nCCharString::CCharString(const CCharString& x):storage_(x.storage_) {}   // test stub: the bitwise copy the implicit constructor made\n#endif\n')
