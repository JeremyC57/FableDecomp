// CTCPreCalculatedNavigationRoute::~CTCPreCalculatedNavigationRoute() -- free the Route vector, release the target pointer, then ~CTCBase
// Retail 0x004E2BC7, 33 bytes. Called only from the deleting destructor 0x004E2BAB (vtable 0x01242444
// slot 0); the manifest label "CTCInventoryItem::OnDie" is propagated.
// Members die in reverse order: Route (STLport vector<pair<C2DVector, unsigned char>>, +0x20) frees its
// block with `free` 0x00BFEA14 under a null test (`pop ecx`, /Os); PThingToCalculateRouteTo
// (CIntelligentPointer<const CThing>, +0x0C) runs ~CBaseIntelligentPointer 0x00A01C10; then
// CTCBase::~CTCBase 0x00686830 is tail-jumped. No vtable store at all: the class is
// `__declspec(novtable)` over CTCBase whose destructor is out of line.
// Offsets as Ego_r.pdb (PThingToCalculateRouteTo +0x0C, ThingToCalculateRouteToUID +0x18, Route +0x20).
// Leaf: ??1CTCPreCalculatedNavigationRoute
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CThing;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    virtual ~CTCBase();                                    // 0x00686830
};

class CBaseObject;

struct CBaseIntelligentPointer {                   // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x4
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // no destructor of its own
    virtual void v00();
};

struct SRouteNode {                                // pair<C2DVector, unsigned char>
    float x, y;
    unsigned char flags;
};

struct SRouteVector {
    SRouteNode* _M_start;
    SRouteNode* _M_finish;
    SRouteNode* _M_end_of_storage;
    ~SRouteVector() { if (_M_start) free(_M_start); }
};

struct __declspec(novtable) CTCPreCalculatedNavigationRoute : CTCBase {
    CIntelligentPointer PThingToCalculateRouteTo;      // +0x0C
    unsigned __int64 ThingToCalculateRouteToUID;       // +0x18
    SRouteVector Route;                                // +0x20
    virtual void v00();
    ~CTCPreCalculatedNavigationRoute();
};

CTCPreCalculatedNavigationRoute::~CTCPreCalculatedNavigationRoute()
{
}