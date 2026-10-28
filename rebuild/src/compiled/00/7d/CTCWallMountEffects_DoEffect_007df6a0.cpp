// CTCWallMountEffects::DoEffect() -- stamp the use frame, then a teleport mount starts its CTCTeleporter on the living hero and a healing mount heals the living creature using it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DF6A0, 201 bytes (+ int3 padding).
// Callees: CWorld::GetWorldFrame() @ 0x0049D870 (static); CTCBase::GetPlayerManager() @ 0x00686D40;
//          CPlayerManager::GetMainPlayer() @ 0x00449970; CPlayer::GetPControlledCreature() @ 0x00487DC0;
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0xdd = CTCTeleporter, bit 29 of the
//          bitset dword at +0x38); CTCTeleporter::StartTeleporting(CThingPlayerCreature*) @ 0x007DFF90;
//          CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (x3); CThingPhysical::ModifyHealth(float, bool) @ 0x006A68F0 (virtual slot 59, +0xec), called with
//          the def's HealingAmount and true.
// Layout (Ego_r.pdb, retail offsets equal): CTCWallMountEffects PDef +0x0c, CreaturePerformingAction +0x14,
//          FrameEffectLastUsed +0x1c; CWallMountEffectsDef Effect +0x28 (1 = teleport, 2 = heal), HealingAmount +0x2c;
//          CThing dead bit +0x91 bit 0.
// Shape: a switch (`dec eax ; je` / `dec eax ; jne`); the teleporter is the `T* p = 0` preload then `if (HasTC) p = GetTC` (component in
//          ecx, `xor ecx,ecx` preload); the healing creature is the landed IS_THING_ALIVE idiom re-fetching GetPItem.
class CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CThingPlayerCreature : public CThing { };
class CThingPlayerCreature;
class CThingPhysical : public CThing { public:          // CThingCreatureBase derives from it
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void ModifyHealth(float amount, bool showEffect);   // slot 59 (+0xec) 0x006A68F0 (CThingPhysical)
};

class CTCTeleporter { public:                          // interface 0xdd
    void StartTeleporting(CThingPlayerCreature* hero);   // 0x007DFF90
};

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

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};

enum EWallMountEffects { WALL_MOUNT_EFFECT_NONE = 0, WALL_MOUNT_EFFECT_TELEPORT = 1, WALL_MOUNT_EFFECT_HEAL = 2 };
class CWallMountEffectsDef { public:
    unsigned char     _pad_00[0x28];
    EWallMountEffects Effect;                   // +0x28
    float             HealingAmount;            // +0x2c
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[4];
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCWallMountEffects : public CTCBase { public:
    const CWallMountEffectsDef* PDef;           // +0x0c CDefPointer<CWallMountEffectsDef const>
    long                    WaitingForConfirmation;     // +0x10
    CIntelligentPointer<CThing> CreaturePerformingAction;   // +0x14 CIntelligentPointer<CThingCreatureBase>
    long                    FrameEffectLastUsed;        // +0x1c
    bool                    ObjectWasGamePersistant;    // +0x20
    void DoEffect();
};

void CTCWallMountEffects::DoEffect()
{
    FrameEffectLastUsed = CWorld::GetWorldFrame();
    switch (PDef->Effect) {
    case WALL_MOUNT_EFFECT_TELEPORT: {
        CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (hero && !hero->IsDead()) {
            CThing* thing = ParentThing;
            CTCTeleporter* teleporter = 0;
            if (thing->HasTC(0xdd))
                teleporter = (CTCTeleporter*)thing->GetTC(0xdd);
            teleporter->StartTeleporting((CThingPlayerCreature*)hero);
        }
        break;
    }
    case WALL_MOUNT_EFFECT_HEAL:
        if (IS_THING_ALIVE(CreaturePerformingAction))
            ((CThingPhysical*)CreaturePerformingAction.GetPItem())->ModifyHealth(PDef->HealingAmount, true);
        break;
    }
}