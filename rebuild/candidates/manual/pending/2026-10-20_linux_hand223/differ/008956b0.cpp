// Grade: DIFFER(250v262) -- register allocation (retail: this in edi, thing in ebx, hero reusing edi, ebx pushed after the IsAlive test; ours: this/hero in ebx, thing in edi) and the XY delta: retail loads the thing's X/Y onto the x87 stack first, stores dx into the dead `entity` slot and squares it from memory; ours keeps both deltas on the stack (fsub [mem])
// CGameScriptInterface::EntityTeleportToHeroPosition(const CScriptThing& entity) const -- when the entity is alive and more than 2 units (in XY) from the hero's controlled creature, find a wander position near the hero (radius 0.8) through the entity's navigation component and teleport the entity there
// Leaf: ?EntityTeleportToHeroPosition@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008956B0, length 262 bytes (+ int3 padding), `ret 4`.
// Signature from Ego_r.pdb: ?EntityTeleportToHeroPosition@CGameScriptInterface@@UBEXABVCScriptThing@@@Z.
// Callees: CScriptThing slots 75 IsAlive and 11 GetThing, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::
//   GetControlledCreature @ 0x00487DD0 (both labels propagated in the manifest), CThing::GetPosition @ 0x004C73D0, the
//   const TC map lower bound @ 0x004365B0, CTCCreatureNavigation::GetNearbyWanderPosition(const C3DVector&, float,
//   C3DVector*, bool) @ 0x006CB6E0, slot 31 (+0x7c) of the thing's placement object (+0x60). Float pool: 0x0123088C = 4.0f.
// Layout: CGameScriptInterface PlayerManager +0x14 (Ego_r.pdb); retail CThing bitset +0x20 (TC 7 = bit 7 of +0x20), map
//   +0x44, placement +0x60, dead bit +0x91.
// Shape: the target is a 12-byte C3DVector local of which only X/Y are copied from the hero's position (dword moves);
//   the X delta is homed in the dead `entity` slot; the squared XY distance is written `dx*dx + dy*dy` (VC7.1 emits the
//   Y term first); the navigation lookup is the const HasTC + one-ternary GetTC form.
struct C3DVector { float X, Y, Z; };
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThingPlacement {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void TeleportTo(const C3DVector& position);   // slot 31 (+0x7c)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPlacement* Placement;                         // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
    void* PImp[2];
};
struct CTCCreatureNavigation {
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* out, bool flag);   // 0x006CB6E0
};
struct CPlayer {
    CThing* GetControlledCreature();                    // 0x00487DD0
};
struct CPlayerManager {
    CPlayer& GetMainPlayer();                           // 0x00449970
};
struct CGameScriptInterface {
    void*           _vptr;                              // +0x00
    void*           World;                              // +0x04
    void*           Component;                          // +0x08
    void*           DisplayEngine;                      // +0x0c
    void*           DefinitionManager;                  // +0x10
    CPlayerManager* PlayerManager;                      // +0x14
    void EntityTeleportToHeroPosition(const CScriptThing& entity) const;
};

void CGameScriptInterface::EntityTeleportToHeroPosition(const CScriptThing& entity) const
{
    if (!entity.IsAlive())
        return;
    CThing* thing = entity.GetThing();
    CThing* hero = PlayerManager->GetMainPlayer().GetControlledCreature();
    if (!hero || (hero->m_flags91 & 1))
        return;
    C3DVector target;
    const C3DVector& heroPos = hero->GetPosition();
    target.X = heroPos.X;
    target.Y = heroPos.Y;
    const C3DVector& thingPos = thing->GetPosition();
    float dx = target.X - thingPos.X;
    float dy = target.Y - thingPos.Y;
    if (dx * dx + dy * dy > 4.0f && thing->HasTC(7)) {
        CTCCreatureNavigation* navigation = thing->GetTC<CTCCreatureNavigation>(7);
        if (navigation->GetNearbyWanderPosition(hero->GetPosition(), 0.8f, &target, false) && thing->Placement)
            thing->Placement->TeleportTo(target);
    }
}
