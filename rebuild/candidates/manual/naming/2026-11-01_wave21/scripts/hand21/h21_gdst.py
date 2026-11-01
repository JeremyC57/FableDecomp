# h21_gdst.py: naming wave 21, family gdst, the hand rows the fam_gdst21 driver cannot do: the NUISystem-namespaced declaration (0054ed90),
# the class-inheriting model (008dd6e0), the stand-in member rows 009ae6b0 (GetName -> the PDB ?GetStringAsCharArray@CDefStringTable@@QBEPBDJ@Z
# 0x009D49F0), 009d5060 (the catalog CDefString::SetString: TablePos = GDefStringTable.AddString(s), PDB ?AddString@CDefStringTable@@QAEJABVCCharString@@@Z
# 0x009D4F90) and 00415d70 (the PDB ??BCDefString@@QBE?AVCCharString@@XZ: return GDefStringTable.GetString(TablePos)).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_gdst.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
def E(a, k, old, new):
    if a not in _X: he.edit(a, k, old, new)
def R(a, k, pat, rep, n=None):
    if a not in _X: he.resub(a, k, pat, rep, n)
def P(a, txt):
    if a not in _X: he.put(a, 'src', txt)
E('0054ed90', 'src', '''struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
struct CDefString {
    long Index;
    CCharString GetString() const { return g_DefStringTable.GetString(Index); }
};''', '''}
class CDefStringTable { public:                         // global scope (naming wave 21): the PDB class, not NUISystem's
    NUISystem::CCharString GetString(long id) const;    // 0x009D49B0 ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z
};
extern CDefStringTable GDefStringTable;                 // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)
namespace NUISystem {
struct CDefString {
    long Index;
    CCharString GetString() const { return GDefStringTable.GetString(Index); }
};''')
R('0054ed90', 'src', r'\bg_DefStringTable\b', 'GDefStringTable')
E('008dd6e0', 'src', 'struct CDefStringTable:CDefStringTablePdb {\n    CCharString GetString(long)const; //009d49b0\n};\nextern CDefStringTable g_DefStringTable;', 'class CDefStringTable : public CDefStringTablePdb { public:   // naming wave 21: class key V (PDB)\n    CCharString GetString(long)const; //009d49b0\n};\nextern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)')
R('008dd6e0', 'src', r'\bg_DefStringTable\b', 'GDefStringTable')
E('009ae6b0', 'src', 'struct CDefTypeNames { const char* GetName(long type); };\nextern CDefTypeNames g_DefTypeNames;                      // 0x13CA828', 'class CDefStringTable { public: const char* GetStringAsCharArray(long id) const; };   // 0x009D49F0 ?GetStringAsCharArray@CDefStringTable@@QBEPBDJ@Z (PDB; naming wave 21: was a CDefTypeNames::GetName stand-in)\nextern CDefStringTable GDefStringTable;                   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)')
E('009ae6b0', 'src', 'g_DefTypeNames.GetName(def->Type)', 'GDefStringTable.GetStringAsCharArray(def->Type)')
P('009d5060', '''// CDefString::SetString(const CCharString& string) -- store the table position of the string added to the global def string table (CDefStringTable::AddString 0x009D4F90 on GDefStringTable 0x013CA828)
// Leaf: ?SetString@CDefString@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D5060, 24 bytes, thiscall `ret 4`: `mov eax,[esp+4] ; push esi ; mov esi,ecx ; push eax ; mov ecx,offset GDefStringTable ; call AddString ; mov [esi],eax ; pop esi ; ret 4`.
// Naming (naming wave 21): the catalog's CDefString::SetString (no `mov eax,esi` epilogue: not the CCharString constructor ??0CDefString@@QAE@ABVCCharString@@@Z, which
//   returns `this`); the callee is the PDB ?AddString@CDefStringTable@@QAEJABVCCharString@@@Z (returns the position) on the established ?GDefStringTable@@3VCDefStringTable@@A.
//   Was a `Singleton::method` stand-in over g_singleton_13ca828 (the CRegion::SetMiniMapRegionExitTextOffsetX label is propagated).
class CCharString;
class CDefStringTable { public:
    long AddString(const CCharString& string);           // 0x009D4F90
};
extern CDefStringTable GDefStringTable;                 // 0x013CA828

class CDefString { public:
    long TablePos;                                      // +0x00
    void SetString(const CCharString& string);
};

void CDefString::SetString(const CCharString& string)
{
    TablePos = GDefStringTable.AddString(string);
}
''')
P('00415d70', '''// CDefString::operator CCharString() const -- the def string's text: CDefStringTable::GetString(TablePos) (0x009D49B0) on GDefStringTable 0x013CA828, returned through the hidden result pointer
// Leaf: ??BCDefString@@QBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415D70, 25 bytes, thiscall `ret 4` (hidden result): `mov eax,[ecx] ; push esi ; mov esi,[esp+8] ; push eax ; push esi ; mov ecx,offset GDefStringTable ; call GetString ; mov eax,esi ; pop esi ; ret 4`.
// Naming (naming wave 21): the PDB ??BCDefString@@QBE?AVCCharString@@XZ (Ego_r / FableWin), the callee the PDB ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z on the
//   established ?GDefStringTable@@3VCDefStringTable@@A. Was a `CCharString::AssignTo` / `Pool::Assign` stand-in over g_pool.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30 (not called here: the callee's result lands in the caller's slot)
    ~CCharString();                                     // 0x0099EAE0
};
class CDefStringTable { public:
    CCharString GetString(long tablePos) const;        // 0x009D49B0
};
extern CDefStringTable GDefStringTable;                 // 0x013CA828

class CDefString { public:
    long TablePos;                                      // +0x00
    operator CCharString() const;
};

CDefString::operator CCharString() const
{
    return GDefStringTable.GetString(TablePos);
}
''')
# the tests that define the table global themselves follow the established spelling (they LINK_FAILed against the respelled sources)
for a in ('00415dd0', '007a0400'):
    R(a, 'tst', r'\bg_DefStringTable\b', 'GDefStringTable')
R('007a0400', 'tst', r'\bstruct CDefStringTable \{', 'class CDefStringTable { public:', 1)
# 0x009D5060's test drove the old free-function stand-in: a self-contained test of CDefString::SetString
if '009d5060' not in _X: he.put('009d5060', 'tst', '''// Behaviour test for 0x009D5060 CDefString::SetString(const CCharString&): TablePos becomes the position GDefStringTable.AddString returns for the string
#include <stdio.h>
class CCharString { public: void* PStringData; };
class CDefStringTable { public:
    const CCharString* lastString; int calls;
    long AddString(const CCharString& string);
};
CDefStringTable GDefStringTable;
long CDefStringTable::AddString(const CCharString& string) { lastString = &string; ++calls; return 0x1234; }
class CDefString { public:
    long TablePos;
    void SetString(const CCharString& string);
};
int main()
{
    CCharString s; s.PStringData = 0;
    CDefString d; d.TablePos = -1;
    d.SetString(s);
    if (d.TablePos == 0x1234 && GDefStringTable.lastString == &s && GDefStringTable.calls == 1) { printf("009d5060_TEST PASS\\n"); return 0; }
    printf("009d5060_TEST FAIL\\n"); return 1;
}
''')
# 0054ed90: CCharString is the global PDB class (the table's GetString returns ?AVCCharString@@): hoisted out of NUISystem with a using-declaration,
# so GetString binds the PDB ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z (already bound at 0x009D49B0) and ~CCharString / GetFont follow
E('0054ed90', 'src', '''namespace NUISystem {
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString''', '''class CCharString {                                     // global scope (naming wave 21): the PDB class
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
namespace NUISystem {
using ::CCharString;
class CWideString''')
E('0054ed90', 'src', '    NUISystem::CCharString GetString(long id) const;    // 0x009D49B0', '    CCharString GetString(long id) const;               // 0x009D49B0')
