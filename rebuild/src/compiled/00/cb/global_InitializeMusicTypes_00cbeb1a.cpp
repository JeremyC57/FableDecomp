// InitializeMusicTypes() -- build the global music/atmos type symbol map on first use and load it from "Data\Defs\atmos_types.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?InitializeMusicTypes@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBEB1A, 100 bytes, `__fastcall` global with no arguments, plain `ret`
//   (PDB ?InitializeMusicTypes@@YIXXZ).
// Callees: scalar operator new @ 0x00BFEA1A (0x24 bytes), CSymbolMap::CSymbolMap() @ 0x009E5200, GetDefsDirectory() @
//   0x0041A080 (L"Data\Defs\", __fastcall with the hidden result in ecx; catalogued as the propagated
//   CUserProfileManager::GetAutoSaveFileName), operator+(const CWideString&, const unsigned short*) @ 0x0099BF30
//   (__fastcall: hidden result in ecx, left operand in edx, the literal on the stack), CSymbolMap::LoadFromFile @
//   0x009E5C90, ~CWideString @ 0x0099B510 (x2).
// Data: the symbol-map pointer @ 0x0143E900, L"atmos_types.h" @ 0x012C1778.
// Shape: frame-pointer size TU (the quest-helper globals' module: `optimize("y",off)` + `("s",on)`); the null test
//   is the guard, `new` with an inline null test (`and [g],0` is the size form of the zero store), then the
//   right-to-left argument evaluation of GetDefsDirectory() + literal (literal pushed first, directory temporary in
//   frame word -8, the sum in -4), temporaries destroyed sum first.
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};

CWideString __fastcall GetDefsDirectory();             // 0x0041A080
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30

class CSymbolMap {
public:
    CSymbolMap();                                       // 0x009E5200
    bool LoadFromFile(const CWideString& filename);     // 0x009E5C90
    unsigned char _body[0x24];
};
extern CSymbolMap* g_pMusicTypeSymbolMap;               // 0x0143E900

void __fastcall InitializeMusicTypes()
{
    if (g_pMusicTypeSymbolMap == 0) {
        g_pMusicTypeSymbolMap = new CSymbolMap;
        g_pMusicTypeSymbolMap->LoadFromFile(GetDefsDirectory() + L"atmos_types.h");
    }
}