// _Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo> >, pair<...> >
// Leaf: CSymbolInfo>_>
// Retail 0x00927be0. Destroys the two internal sub-objects of the pair
// (at +0x14 and +0xc) then tail-calls the pair's own destructor.

// The pair<CCharString, CSymbolInfo> object. CSymbolInfo carries two
// destructible members (e.g. CCharStrings) at +0xc and +0x14 relative to
// the pair base; the CCharString 'first' begins the pair. The three engine
// destructors live at fixed addresses.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
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