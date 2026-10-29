// CTCWillResponse::IsThingLifeDrainable(const CThing* thing) -- true when the thing may be life-drained: the will-response component's IsLifeDrainable bit unless it is flagged IsNotBloodyWillable; a thing without the component is drainable
// Leaf: ?IsThingLifeDrainable@CTCWillResponse@@SI_NPBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E44E0, 76 bytes (+ int3 padding). FableWin.pdb
//   `?IsThingLifeDrainable@CTCWillResponse@@SI_NPBVCThing@@@Z` = public static bool __fastcall (const CThing*); the catalog
//   module `_global` is the propagated default, the class comes from the PDB symbol.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (the const copy; key by address).
// Layout: CTCWillResponse (Ego_r.pdb, 0x14 bytes): CTCBase {vptr, ParentThing +4, flags +8}, PDef +0x0c, the seven
//   response bits at +0x10 in PDB field order (IsForcePushable bit 0 .. IsEpicSpellable bit 6). Interface type 0x9c ->
//   presence word 4 of the thing's bitset (+0x20 + 0x10 = +0x30), bit 28. No CThing / CTCWillResponse engine header
//   exists, so the thing is the retail local model used by landed 0x00664D00 (bitset +0x20, component map +0x44).
// Shape: the PDB's `CThing::PeekTC<const CTCWillResponse>(const T** out) const` inlined with the map address in a named
//   local (`lea esi,[ecx+44h]`, re-read of `last` through it), then the private inline `IsThingLifeDrainable() const`
//   (`if (IsNotBloodyWillable) return false; return IsLifeDrainable;` = `test al,8 ; je ; shr al,1 ; and al,1`).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                 // the thing's sorted component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    const STCEntry* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> bool PeekTC(const T** out) const
    {
        if (HasTC(T::INTERFACE_TYPE)) {
            const CTCMap* map = &m_tcs;
            const STCEntry* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->m_pLast || T::INTERFACE_TYPE < it->first)
                it = map->m_pLast;
            *out = (const T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCWillResponse {
public:
    enum { INTERFACE_TYPE = 0x9c };
    void*         _vfptr;                       // CTCBase vptr
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[4];                    // CTCBase flags
    void*         PDef;                         // +0x0c CDefPointer<const CWillResponseDef>
    bool IsForcePushable : 1;                   // +0x10 bit 0
    bool IsLifeDrainable : 1;                   // bit 1
    bool IsLightningable : 1;                   // bit 2
    bool IsNotBloodyWillable : 1;               // bit 3
    bool IsAbleToBeLifted : 1;                  // bit 4
    bool IsEpicPowerExplodable : 1;             // bit 5
    bool IsEpicSpellable : 1;                   // bit 6
    bool IsThingLifeDrainable() const
    {
        if (IsNotBloodyWillable)
            return false;
        return IsLifeDrainable;
    }
    static bool __fastcall IsThingLifeDrainable(const CThing* thing);
};

bool __fastcall CTCWillResponse::IsThingLifeDrainable(const CThing* thing)
{
    const CTCWillResponse* tc;
    if (thing->PeekTC(&tc))
        return tc->IsThingLifeDrainable();
    return true;
}