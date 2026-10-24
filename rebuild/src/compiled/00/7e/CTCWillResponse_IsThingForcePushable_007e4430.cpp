// CTCWillResponse::IsThingForcePushable(const CThing* thing) -- true when the thing may be force-pushed: not when its will-response component forbids it (IsNotBloodyWillable or no IsForcePushable), not while it holds a physical-shield spell component, and not while its creature-mode manager has mode 0x36 active
// Leaf: ?IsThingForcePushable@CTCWillResponse@@SI_NPBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E4430, 165 bytes (+ int3 padding). FableWin.pdb
//   `?IsThingForcePushable@CTCWillResponse@@SI_NPBVCThing@@@Z` = public static bool __fastcall (const CThing*); the catalog
//   module `_global` is the propagated default, the class comes from the PDB symbol.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (key by address; same-TU stand-in below),
//   CTCCreatureModeManager::IsModeActive(unsigned long) const @ 0x0073A8A0 (landed name; Ego_r.pdb `QBE_NK`).
// Interface types (GetInterfaceType, vtable slot 21): CTCWillResponse 0x9c (word +0x30 bit 28), CTCPhysicalShieldSpell 0x93
//   (word +0x30 bit 19), CTCCreatureModeManager 0x31 (word +0x24 bit 17). Mode 0x36 is kept numeric (enum not recovered).
// Layout: CTCWillResponse (Ego_r.pdb, 0x14 bytes) response bits at +0x10 in PDB field order; the thing is the retail local
//   model used by landed 0x00664D00 (bitset +0x20, component map +0x44); no CThing / CTCWillResponse engine header exists.
// Shape: retail loads presence word 4 once (`mov ebx,[edi+30h]`) and tests it again for 0x93 after the LowerBound call:
//   VC7.1 does that only with the const LowerBound body visible in the TU (it is a COMDAT template instantiation there), so a
//   read-only same-TU stand-in carries it. The will-response verdict is stored to a dead frame byte (`mov [esp+0Fh],al`):
//   the inline `IsThingForcePushable() const` result bound to a `const bool&`. Both lookups are the PDB's
//   `PeekTC<const T>(const T** out) const`, which must be `__forceinline` (the second instance is otherwise emitted out of
//   line and esi is shrink-wrapped).
class CTCBase;

struct STCEntry {                               // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {                                 // the thing's sorted component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    const STCEntry* LowerBound(const unsigned long& kind) const;   // 0x004365B0
};

// Same-TU stand-in for the const CVectorMap::LowerBound 0x004365B0 (read-only binary search, as the retail COMDAT).
#pragma auto_inline(off)
__declspec(noinline) const STCEntry* CTCMap::LowerBound(const unsigned long& kind) const
{
    const STCEntry* first = m_pFirst;
    long count = m_pLast - first;
    while (count > 0) {
        long half = count / 2;
        if (first[half].m_kind < (long)kind) {
            first += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return first;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool PeekTC(const T** out) const
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

class CTCPhysicalShieldSpell {
public:
    enum { INTERFACE_TYPE = 0x93 };
};

class CTCCreatureModeManager {
public:
    enum { INTERFACE_TYPE = 0x31 };
    bool IsModeActive(unsigned long type) const;   // 0x0073A8A0
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
    bool IsThingForcePushable() const
    {
        if (IsNotBloodyWillable)
            return false;
        return IsForcePushable;
    }
    static bool __fastcall IsThingForcePushable(const CThing* thing);
};

bool __fastcall CTCWillResponse::IsThingForcePushable(const CThing* thing)
{
    const CTCWillResponse* tc;
    if (thing->PeekTC(&tc)) {
        const bool& pushable = tc->IsThingForcePushable();
        if (!pushable)
            return false;
    }
    if (thing->HasTC(CTCPhysicalShieldSpell::INTERFACE_TYPE))
        return false;
    const CTCCreatureModeManager* modes;
    if (thing->PeekTC(&modes) && modes->IsModeActive(0x36))
        return false;
    return true;
}