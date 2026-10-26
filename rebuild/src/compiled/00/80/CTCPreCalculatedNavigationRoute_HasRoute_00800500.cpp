// CTCPreCalculatedNavigationRoute::HasRoute() -- TRUE when the target thing is alive (non-null, dead bit clear) and the route is not empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00800500, length 55 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50 (twice, on the +0x0c pointer addressed once in esi).
// Shape: the inlined IS_THING_DEAD intelligent-pointer predicate (the landed 0x00836BE0 shape: GetPItem null test, GetPItem
// again and `test byte [eax+91h],1`) negated, then the vector's empty test (`mov eax,[edi+20h] ; cmp eax,[edi+24h] ; je`);
// a `long` (BOOL) result: `mov eax,1` / `xor eax,eax` on two duplicated exits.
// Layout = Ego_r.pdb CTCPreCalculatedNavigationRoute with the 12-byte retail vector: PThingToCalculateRouteTo +0x0c,
// ThingToCalculateRouteToUID +0x18, Route (vector<pair<C2DVector, unsigned char>>) +0x20. CThing's dead flag is bit 0 of +0x91
// (as the landed IS_THING_DEAD bodies model it; the PDB CThing layout does not match retail there).
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags;                                  // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
};

#pragma pack(push, 1)
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

inline bool IS_THING_DEAD(const CIntelligentPointer<CThing>& pointer)
{
    if (pointer.GetPItem() != 0) {
        if (!pointer.GetPItem()->IsDead())
            return false;
    }
    return true;
}

struct SRouteNode;
struct CRetailVector_RouteNode {                            // retail std::vector, 12 bytes
    SRouteNode* _Myfirst;
    SRouteNode* _Mylast;
    SRouteNode* _Myend;
    bool empty() const { return _Myfirst == _Mylast; }
};

struct CTCBase_Layout {                                     // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCPreCalculatedNavigationRoute : public CTCBase_Layout { public:
    CIntelligentPointer<CThing> PThingToCalculateRouteTo;       // +0x0c CIntelligentPointer<const CThing>
    unsigned __int64        ThingToCalculateRouteToUID;     // +0x18
    CRetailVector_RouteNode Route;                          // +0x20
    long HasRoute() const;
};

long CTCPreCalculatedNavigationRoute::HasRoute() const
{
    if (!IS_THING_DEAD(PThingToCalculateRouteTo) && !Route.empty())
        return 1;
    return 0;
}