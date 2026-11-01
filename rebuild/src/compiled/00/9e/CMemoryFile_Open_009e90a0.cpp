// CMemoryFile::Open() -- open the memory file empty: clear the path name, rewind, zero the length, mark it open
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
}