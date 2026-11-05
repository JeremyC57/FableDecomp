// _Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo> >, pair<...> >
// Leaf: ?_Dest_val@@YIXPAUSymPair@@PAX@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail 0x00927be0. Destroys the two internal sub-objects of the pair
// (at +0x14 and +0xc) then tail-calls the pair's own destructor.

// The pair<CCharString, CSymbolInfo> object. CSymbolInfo carries two
// destructible members (e.g. CCharStrings) at +0xc and +0x14 relative to
// the pair base; the CCharString 'first' begins the pair. The three engine
// destructors live at fixed addresses.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct SymPair;

// engine dtor called on the member at +0x14  (retail 0x00a01c10)
// engine dtor called on the member at +0xc   (retail 0x0099eae0)
void __fastcall memb0c_dtor(void* self);
// engine dtor called on the whole pair        (retail 0x0099a2e0)

void __fastcall _Dest_val(SymPair* p, void* al)
{
    char* base = (char*)p;
    ((CBaseIntelligentPointer*)(base + 0x14))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    memb0c_dtor(base + 0xc);
    ((CBaseClass*)(p))->CBaseClass::~CBaseClass();
}