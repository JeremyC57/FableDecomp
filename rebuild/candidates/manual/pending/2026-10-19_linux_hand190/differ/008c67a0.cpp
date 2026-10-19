// CCreatureAction_TrollThrowProjectile::FrameUpdate() -- fade the carried shot in (ShotAlpha += 16 up to 255, straight to 255 once the troll's primary slot is empty), apply the alpha to the live shot's graphic appearance, then the base FrameUpdate
// Grade: DIFFER(190v190) -- scheduling only: retail fetches the shot (third GetPItem) before loading/pushing the alpha byte, ours pushes the alpha first; a shot local, operator-> or an inline CThing::SetAlpha fix the order but swap esi/edi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C67A0, length 190 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x46 =
// CTCCarrying, bit 6 of +0x28), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCGraphicAppearance::SetAlpha(unsigned char)
// @ 0x004BFAC0, CCombatActionBase::FrameUpdate @ 0x00859D80.
// Shape: the increment keeps the loaded value in eax, the clamp re-reads the member; the checked carrying lookup feeds the
// empty-slot test; the shot is tested present / not dead (thing byte +0x91 bit 0) and its cached appearance (+0x64) gets the
// low byte of ShotAlpha (`xor ecx,ecx ; mov cl` for the unsigned char parameter).
// Layout: Ego_r.pdb PShot +0x12c, TargetPos +0x134, UseTargetPos +0x140, ShotAlpha +0x144; retail +0x114 / +0x11c / +0x128 /
// +0x12c (CCombatActionBase is 0x114 in retail).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);         // 0x004BFAC0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* m_pGraphicAppearance; // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
    CThing* GetThingInPrimarySlot();            // 0x006A1440
};

struct C3DVector { float X, Y, Z; };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCombatActionBase {
    virtual void FrameUpdate();                 // 0x00859D80
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

struct CCreatureAction_TrollThrowProjectile : CCombatActionBase {
    CBaseIntelligentPointer PShot;              // +0x114 (PDB +0x12c)
    C3DVector               TargetPos;          // +0x11c
    bool                    UseTargetPos;       // +0x128
    long                    ShotAlpha;          // +0x12c
    virtual void FrameUpdate();
};

void CCreatureAction_TrollThrowProjectile::FrameUpdate()
{
    if (ShotAlpha < 255)
        ShotAlpha += 16;
    if (ShotAlpha > 255)
        ShotAlpha = 255;
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetInterface(&carrying, 0x46) && !carrying->GetThingInPrimarySlot())
        ShotAlpha = 255;
    if (PShot.GetPItem() && !(PShot.GetPItem()->m_flags91 & 1))
        PShot.GetPItem()->m_pGraphicAppearance->SetAlpha((unsigned char)ShotAlpha);
    CCombatActionBase::FrameUpdate();
}
