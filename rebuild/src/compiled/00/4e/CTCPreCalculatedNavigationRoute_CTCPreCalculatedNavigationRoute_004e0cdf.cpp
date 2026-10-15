// CTCPreCalculatedNavigationRoute::CTCPreCalculatedNavigationRoute(CThing* thing) -- CTCBase(thing), own vtable, an empty target intelligent pointer, target UID 0, an empty route
// Leaf: ??0CTCPreCalculatedNavigationRoute
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0CDF, length 59 bytes; size TU (unaligned entry, pass-through
// `push dword ptr [esp+0Ch]` before `mov esi,ecx`): `#pragma optimize("s", on)`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite", propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10. Vtables 0x01242444 = CTCPreCalculatedNavigationRoute,
// 0x01237F1C = CIntelligentPointer (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCPreCalculatedNavigationRoute (0x30) with the 12-byte retail vector (0x2c): PThingToCalculateRouteTo
// (CIntelligentPointer<const CThing>, 8 bytes) +0x0c, ThingToCalculateRouteToUID (ulong64, 8-aligned) +0x18, Route
// (vector<pair<C2DVector, unsigned char>>) +0x20.
// Shape: the intelligent pointer is the out-of-line base constructor plus its inline vtable store (`lea edi,[esi+0Ch]`
// addressed once); the UID's two words and the vector's three pointers follow as five stores of one `xor eax,eax` register
// (init-list scalar + member object with an inline zeroing constructor, all after the derived vtable store).
#pragma optimize("s", on)
struct CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() { }
};
#pragma pack(pop)

struct SRouteNode;                                          // pair<C2DVector, unsigned char>
struct CRetailVector_RouteNode {                            // retail std::vector, 12 bytes
    SRouteNode* _Myfirst;
    SRouteNode* _Mylast;
    SRouteNode* _Myend;
    CRetailVector_RouteNode() : _Myfirst(0), _Mylast(0), _Myend(0) { }
};

struct CTCPreCalculatedNavigationRoute : CTCBase_Calls {    // vtable 0x01242444
    CIntelligentPointer     PThingToCalculateRouteTo;       // +0x0c
    unsigned __int64        ThingToCalculateRouteToUID;     // +0x18
    CRetailVector_RouteNode Route;                          // +0x20
    virtual void v00();
    CTCPreCalculatedNavigationRoute(CThing* thing);
};

CTCPreCalculatedNavigationRoute::CTCPreCalculatedNavigationRoute(CThing* thing)
    : CTCBase_Calls(thing)
    , PThingToCalculateRouteTo()
    , ThingToCalculateRouteToUID(0)
    , Route()
{
}