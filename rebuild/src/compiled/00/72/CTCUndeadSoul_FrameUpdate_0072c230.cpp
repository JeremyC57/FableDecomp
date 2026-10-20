// CTCUndeadSoul::FrameUpdate() -- an undead soul is visible (drawable and targetable) only while the controlled hero is alive and able to use the soul-seeing ability (7); toggles drawable/targetable and, for a player-type thing, the animation suspension when that changes
// Leaf: ?FrameUpdate@CTCUndeadSoul
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072C230, 318 bytes (plain `ret`); CTCUndeadSoul vtable 0x0123A364 slot 9
// (Ego_r.pdb `?FrameUpdate@CTCUndeadSoul@@UAEXXZ`; the manifest says "vfunc_9").
// Callees: CTCBase::GetPlayerManager() 0x00686D40; CPlayerManager::GetMainPlayer() 0x00449970; CPlayer::
//          GetControlledCreature() 0x00487DD0; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0
//          (interface 0x6F, CTCInventoryAbilities, through the const GetTC) and 0x0040F020 (interface 9, CTCTargeted);
//          CTCInventoryAbilities::IsAbleToUseAbility(EHeroAbility, bool) 0x005EE49A; CThing::DrawIsDrawable (slot 27, +0x6C)
//          and SetAsDrawable(bool) (slot 28, +0x70); CThing::SetSuspendAnimation(bool, long) 0x00833A10.
// Layout (retail): CThing m_type +0x10 (1 = the player creature), bitset +0x20 (0x6F = bit 15 of word +0x2c, 9 = bit 9 of
//          word +0x20), interface map +0x44, dead bit +0x91; CTCTargeted::Targetable +0x10 (PDB offset holds).
// Shape: the constant 1 lives in ebx (dead-bit mask, the ability flag, the drawable flag, the type compare); the
//        targeted component uses the `T* p = 0; if (HasTC) p = GetTC;` zero preload; the two arms are written out in
//        full (each re-reads ParentThing).
class CThing;
struct CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026();
    virtual bool DrawIsDrawable();              // slot 27 (+0x6c)
    virtual void SetAsDrawable(bool drawable);  // slot 28 (+0x70)
    unsigned char _pad04[0xc];
    unsigned char m_type;                       // +0x10 (1 = the player creature)
    unsigned char _pad11[0xf];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

struct CTCInventoryAbilities {
    bool IsAbleToUseAbility(long ability, bool checkLevel) const;   // 0x005EE49A (EHeroAbility)
};
struct CTCTargeted {
    unsigned char _pad_0x00[0x10];
    bool          Targetable;                   // +0x10
};
struct CPlayer {
    CThing* GetControlledCreature();            // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};

struct CTCBaseModel {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    unsigned char _pad0a[2];
    CPlayerManager* GetPlayerManager();         // 0x00686D40
};
struct CTCUndeadSoul : CTCBaseModel {
    virtual void FrameUpdate();
};

void CTCUndeadSoul::FrameUpdate()
{
    const CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
    if (hero && !(hero->m_flags & 1) && hero->HasTC(0x6f)
        && ((const CTCInventoryAbilities*)hero->GetTC(0x6f))->IsAbleToUseAbility(7, true)) {
        if (!ParentThing->DrawIsDrawable()) {
            ParentThing->SetAsDrawable(true);
            CTCTargeted* targeted = 0;
            if (ParentThing->HasTC(9))
                targeted = (CTCTargeted*)ParentThing->GetTC(9);
            targeted->Targetable = true;
            if (ParentThing->m_type == 1)
                ParentThing->SetSuspendAnimation(false, 0);
        }
    } else if (ParentThing->DrawIsDrawable()) {
        ParentThing->SetAsDrawable(false);
        CTCTargeted* targeted = 0;
        if (ParentThing->HasTC(9))
            targeted = (CTCTargeted*)ParentThing->GetTC(9);
        targeted->Targetable = false;
        if (ParentThing->m_type == 1)
            ParentThing->SetSuspendAnimation(true, 0);
    }
}