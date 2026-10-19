// CCreatureAction_TrollThrowProjectile::FinishAction() -- if the troll still carries the live shot, kill the shot; clear the combat component's word +0x7c; then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C6A50, length 207 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x46 =
// CTCCarrying, then 0x30 = the combat component), CTCCarrying::IsCarryingThing(CThing*) @ 0x006A1060, CThing::Kill(bool)
// @ 0x004C9B80, CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: two CHECKED out-parameter lookups in the function body sharing the one key word; the shot is tested present / not
// dead (thing byte +0x91 bit 0) before the bool carrying query; the combat word +0x7c is zeroed through the component in eax.
// Layout: Ego_r.pdb PShot +0x12c, retail +0x114 (CCombatActionBase is 0x114 in retail).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool silent);                     // 0x004C9B80
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCCarrying {
    bool IsCarryingThing(CThing* thing);        // 0x006A1060
};
struct CTCCombat {
    unsigned char _pad[0x7c];
    long          m_word7c;                     // +0x7c
};

struct C3DVector { float X, Y, Z; };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCombatActionBase {
    virtual void FinishAction();                // 0x00859A20
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

struct CCreatureAction_TrollThrowProjectile : CCombatActionBase {
    CBaseIntelligentPointer PShot;              // +0x114 (PDB +0x12c)
    C3DVector               TargetPos;          // +0x11c
    bool                    UseTargetPos;       // +0x128
    long                    ShotAlpha;          // +0x12c
    virtual void FinishAction();
};

void CCreatureAction_TrollThrowProjectile::FinishAction()
{
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetInterface(&carrying, 0x46)) {
        if (PShot.GetPItem() && !(PShot.GetPItem()->m_flags91 & 1) && carrying->IsCarryingThing(PShot.GetPItem()))
            PShot.GetPItem()->Kill(true);
    }
    CTCCombat* combat;
    if (PCreature.GetPItem()->GetInterface(&combat, 0x30))
        combat->m_word7c = 0;
    CCombatActionBase::FinishAction();
}