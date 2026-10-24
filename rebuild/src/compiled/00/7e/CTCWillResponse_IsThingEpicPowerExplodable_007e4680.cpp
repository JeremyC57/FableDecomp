// CTCWillResponse::IsThingEpicPowerExplodable(const CThing* thing) -- true when the thing may be blown up by the epic power (the will-response component's IsEpicPowerExplodable bit); a thing without the component is explodable
// Leaf: ?IsThingEpicPowerExplodable@CTCWillResponse@@SI_NPBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E4680, 69 bytes (+ int3 padding). FableWin.pdb
//   `?IsThingEpicPowerExplodable@CTCWillResponse@@SI_NPBVCThing@@@Z` = public static bool __fastcall (const CThing*);
//   catalog label `IsInterfaceVisibleWithBit` is a placeholder: bit 5 is IsEpicPowerExplodable (PDB field order), default true as in the
//   ctor 0x007E3C70; it follows IsThingAbleToBeLifted (0x007E4620, bit 4) in the PDB static order.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (the const copy; key by address).
// Layout: CTCWillResponse (Ego_r.pdb, 0x14 bytes): CTCBase {vptr, ParentThing +4, flags +8}, PDef +0x0c, the seven
//   response bits at +0x10 in PDB field order (IsForcePushable bit 0 .. IsEpicSpellable bit 6). Interface type 0x9c ->
//   presence word 4 of the thing's bitset (+0x30), bit 28. No CThing / CTCWillResponse engine header exists, so the
//   thing is the retail local model used by landed 0x00664D00 (bitset +0x20, component map +0x44).
// Shape: the PDB's `CThing::PeekTC<const CTCWillResponse>(const T** out) const` inlined with the map address in a named
//   local (`lea esi,[ecx+44h]`), then the private inline `IsThingEpicPowerExplodable() const` (return IsEpicPowerExplodable, a one-bit bool field read as
//   `shr al,5 ; and al,1`); the absent-component default `return true;` is the fall-out tail. Same TU shape as 0x007E44E0.
class CTCBase;

struct STCEntry {                               // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {                                 // the thing's sorted component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0
};

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
            const STCEntry* it = map->LowerBound((unsigned long)T::INTERFACE_TYPE);
            if (it == map->m_pLast || T::INTERFACE_TYPE < it->m_kind)
                it = map->m_pLast;
            *out = (const T*)it->m_pTC;
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
    bool IsThingEpicPowerExplodable() const
    {
        return IsEpicPowerExplodable;
    }
    static bool __fastcall IsThingEpicPowerExplodable(const CThing* thing);
};

bool __fastcall CTCWillResponse::IsThingEpicPowerExplodable(const CThing* thing)
{
    const CTCWillResponse* tc;
    if (thing->PeekTC(&tc))
        return tc->IsThingEpicPowerExplodable();
    return true;
}