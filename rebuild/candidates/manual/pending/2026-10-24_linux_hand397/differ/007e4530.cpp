// CTCWillResponse::IsThingLightningable(const CThing* thing) -- true when the thing may be hit by the lightning power: decided by its will-response component when present (IsLightningable unless IsNotBloodyWillable); otherwise true for smashable barriers and for targetables with targeting flags 0x18, else the thing-type flag bit 3
// Grade: DIFFER(159v159) -- the merged `return true` block sits after the LAST test (retail lays it after the 0x4E test and the 0x18 test jumps back); the final ThingTypeFlags bit is read as a dword (`mov eax,[edi+6Ch] ; shr eax,3`) where retail loads the byte (`mov al,[edi+6Ch] ; shr al,3 ; and al,1`)
// Leaf: ?IsThingLightningable@CTCWillResponse@@SI_NPBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E4530, 159 bytes (+ int3). FableWin.pdb `?IsThingLightningable@CTCWillResponse@@SI_NPBVCThing@@@Z`
//   = public static bool __fastcall (const CThing*); catalog module `_global` is the propagated default.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0.
// Interface types (GetInterfaceType, vtable slot 21): CTCWillResponse 0x9c (+0x30 bit 28), CTCSmashableBarrier 0x4e (+0x28
//   bit 14, `test ah,40h`), CTCTargeted 0x09 (+0x20 bit 9; Ego_r.pdb TargetingFlags +0x0c). CThing::ThingTypeFlags is retail
//   +0x6c (Ego_r +0x7c). Six grades used (budget exceeded on single-variant probes; parked).
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
    unsigned char  _pad50[0x1c];
    unsigned long  ThingTypeFlags;              // +0x6c
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline const T* GetTC() const
    {
        const CTCMap* map = &m_tcs;
        const STCEntry* it = map->LowerBound((unsigned long)T::INTERFACE_TYPE);
        if (it == map->m_pLast || T::INTERFACE_TYPE < it->m_kind)
            it = map->m_pLast;
        return (const T*)it->m_pTC;
    }
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

class CTCSmashableBarrier {
public:
    enum { INTERFACE_TYPE = 0x4e };
};

class CTCTargeted {
public:
    enum { INTERFACE_TYPE = 0x09 };
    void*         _vfptr;
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[4];
    unsigned long TargetingFlags;               // +0x0c
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
    bool IsThingLightningable() const
    {
        if (IsNotBloodyWillable)
            return false;
        return IsLightningable;
    }
    static bool __fastcall IsThingLightningable(const CThing* thing);
};

bool __fastcall CTCWillResponse::IsThingLightningable(const CThing* thing)
{
    const CTCWillResponse* tc;
    if (thing->PeekTC(&tc))
        return tc->IsThingLightningable();
    if (thing->HasTC(CTCSmashableBarrier::INTERFACE_TYPE))
        return true;
    if (thing->HasTC(CTCTargeted::INTERFACE_TYPE) && (thing->GetTC<CTCTargeted>()->TargetingFlags & 0x18))
        return true;
    return (thing->ThingTypeFlags & 8) ? true : false;
}
