# h24_wsop.py: naming wave 24, family wsop -- callers of CWideString::operator=(const wchar_t*) 0x0099B800 under stand-ins, bound now as
# ??4CWideString@@QAEABV0@PBG@Z (the spelling the landed 0x00A39010 binds; the lane's wchar_t is unsigned short): both assign the empty
# wide literal 0x0122D70C to a CWideString member.
#  0x009E90A0 CMemoryFile::Open() (Ego_r / FableWin ?Open@CMemoryFile@@QAEXXZ): was a free __fastcall Open(CMemoryFile*) with a junk
#    Sub::init(const char*) member at +0x20 -- that member is Ego_r's CWideString Pathname (engine/CMemoryFile.h +0x20).
#  0x00495FE0 CFrontendGameComponent::SetMode: the "mode pointer" at +0x1E40 assigned from 0x0122D70C is Ego_r's CWideString WorldToLoad
#    (PDB +0x1E4C less the three 4-byte vector shrinks of ProfileNames / SaveNames / WorldNames; the landed GetWorldNames 0x00495890 already
#    models +0x1E40 as WorldToLoad).
# 0x009994A0 / 0x00999620 keep their in-TU AFileSupport / ExternalRetry string models (DEFERRED: per-row re-model).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.put('009e90a0', 'src', '''// CMemoryFile::Open() -- open the memory file empty: clear the path name, rewind, zero the length, mark it open
// Leaf: ?Open@CMemoryFile@@QAEXXZ
// Retail: Fable.exe @ 0x009E90A0, 30 bytes, thiscall plain `ret`.
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?Open@CMemoryFile@@QAEXXZ (the parameterless overload; catalog CMemoryFile::Open); the
//   member at +0x20 is Ego_r's CWideString Pathname (engine/CMemoryFile.h), assigned the empty wide literal 0x0122D70C through
//   CWideString::operator=(const wchar_t*) 0x0099B800. The row had landed as a free __fastcall Open(CMemoryFile*) calling a junk
//   Sub::init(const char*) on a raw address.
// Callees: CWideString::operator=(const wchar_t*) 0x0099B800.
// Layout: engine/CMemoryFile.h (Position +0x08, Length +0x0c, Opened +0x1c, Pathname +0x20).
class CWideString {
public:
    const CWideString& operator=(const unsigned short* text);   // 0x0099B800
};
#define CMemoryFile CMemoryFileLayout
#include "engine/CMemoryFile.h"
#undef CMemoryFile
class CMemoryFile : public CMemoryFileLayout {
public:
    CWideString& GetPathname() { return *(CWideString*)&Pathname_PStringData; }
    void Open();
};

void CMemoryFile::Open()
{
    GetPathname() = L"";
    Position = 0;
    Length = 0;
    Opened = true;
}''')
he.put('009e90a0', 'tst', '''// Behaviour test for 0x009E90A0 CMemoryFile::Open(): the path name is set empty, position and length are zeroed, the file is open.
#include <cstdio>
class CWideString { public: const unsigned short* Text; int Assigned; const CWideString& operator=(const unsigned short* t) { Text = t; ++Assigned; return *this; } };
class CMemoryFile { public:
    void* vtbl; long AccessType; long Position; long Length; char DataBuffer[0xc]; bool Opened; char pad[3]; CWideString Pathname;
    void Open();
};
void CMemoryFile::Open() { Pathname = (const unsigned short*)L""; Position = 0; Length = 0; Opened = true; }
int main() {
    CMemoryFile f;
    f.Position = 99; f.Length = 99; f.Opened = false; f.Pathname.Text = 0; f.Pathname.Assigned = 0;
    f.Open();
    if (f.Pathname.Assigned == 1 && f.Pathname.Text && f.Pathname.Text[0] == 0 && f.Position == 0 && f.Length == 0 && f.Opened) {
        printf("PARITY_OK\\n");
    } else {
        printf("FAIL\\n");
    }
    return 0;
}''')
A = '00495fe0'
he.edit(A, 'src', '''class CFrontendModePointer_00495fe0
{
    void* m_Data;

public:
    void Assign(const CFrontendModePointer_00495fe0*);
};

extern CFrontendModePointer_00495fe0 g_EmptyFrontendModePointer_00495fe0;
''', '''// Naming (naming wave 24): the CWideString at +0x1E40 is Ego_r's WorldToLoad (CFrontendGameComponent PDB +0x1E4C less the three 4-byte
// vector shrinks; the landed GetWorldNames 0x00495890 models it so), cleared
// to the empty wide literal 0x0122D70C through CWideString::operator=(const wchar_t*) 0x0099B800 (??4CWideString@@QAEABV0@PBG@Z); it
// was a CFrontendModePointer_00495fe0 stand-in with an Assign member and a g_EmptyFrontendModePointer_00495fe0 object.
class CWideString
{
    void* PStringData;

public:
    const CWideString& operator=(const unsigned short* text);   // 0x0099B800
};
''')
he.edit(A, 'src', '    CFrontendModePointer_00495fe0 m_ModePointer;\n', '    CWideString WorldToLoad;\n')
he.edit(A, 'src', '        m_ModePointer.Assign(&g_EmptyFrontendModePointer_00495fe0);\n', '        WorldToLoad = L"";\n')
