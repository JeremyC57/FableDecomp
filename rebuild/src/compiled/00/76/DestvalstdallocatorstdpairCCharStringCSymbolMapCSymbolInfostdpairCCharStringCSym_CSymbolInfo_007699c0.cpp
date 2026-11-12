// Faithful VC7.1 reconstruction of std::_Dest_val for
// Leaf: ?Dest_val_CSymbolInfo___@@YIXPAUSymPair@@@Z
// Plan phase 3 (2026-11-04): the Leaf was `CSymbolInfo>_>`, the tail of this row's old display name std::_Dest_val<..CSymbolMap::CSymbolInfo>_> (renamed 2026-10-24); it named no symbol, so recheck graded the largest function (a 30/48-byte ??_G) and reported DIFFER. It now names the 27-byte stand-in helper the bytes were landed on.
//   std::pair<CCharString, CSymbolMap::CSymbolInfo>
// Retail 0x007699c0. Pair + CSymbolInfo destructors inline; leaf CCharString /
// CBaseIntelligentPointer destructors are out-of-line -> call/jmp.
// Name fix (agentNF23): the member's destructor binds the PDB spelling ??1CBaseIntelligentPointer@@UAE@XZ (virtual; Ego_r.pdb / FableWin.pdb) through engine/CIntelligentPointer.h, replacing the local non-virtual model; the explicit call is qualified so it stays direct.

class CCharString {
public:
    ~CCharString();          // 0x99eae0
    char* m_p;               // sizeof == 4
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer: vptr + PData, virtual ~ @ 0x00A01C10

struct CSymbolInfo {
    CCharString name;             // +0
    int a;                        // +4
    int b;                        // +8
    CBaseIntelligentPointer ptr;  // +0xC
};

struct SymPair {
    CCharString first;    // +0
    CSymbolInfo second;   // +4  -> name +4, ptr +0x10
};

// std::_Dest_val<Alloc, pair>(Alloc&, pair* _Ptr) { _Ptr->~pair(); }
// ecx carries _Ptr in retail.
void __fastcall Dest_val_CSymbolInfo___(SymPair* _Ptr)
{
    _Ptr->second.ptr.CBaseIntelligentPointer::~CBaseIntelligentPointer();   // qualified: direct call 0x00A01C10, as retail
    _Ptr->second.name.~CCharString();
    _Ptr->first.~CCharString();
}