// CTCFireballSpell::FrameUpdate() -- adopts the combat target (enemies, or anything when the player auto-targets), then unless in one-anim cast mode starts the finish-fireball action (resetting the will timer if refused), else makes a noise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C9180, 549 bytes, `ret`.
// Callees: CVectorMap::LowerBound const @ 0x004365B0, CTCEnemy::AreThingsEnemies @ 0x0076D2F0, CThing::GetPlayer @ 0x004C7A30,
//          CIntelligentPointer ctor(T*) @ 0x004A96F0 / operator= @ 0x004A9760 / template ctor @ 0x004A9730 (the out-of-line copies),
//          CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10,
//          CThingCreatureBase::FinishCurrentAction @ 0x00662650, IsValidRangedWeaponTargetForShooter @ 0x0085F8D0,
//          CCreatureAction_FinishFireballSpell ctors @ 0x008BCE50 / 0x008BCE10, its by-value pointer setter @ 0x0072D360,
//          CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCreatureActionBase @ 0x0062E900 (folded label),
//          CCharString(const char*, long) @ 0x0099EBF0, CThingCreatureBase::MakeNoise @ 0x00662110, ~CCharString @ 0x0099EAE0.
//          CIntelligentPointer vtable 0x01238C6C; "SIMPLE_NOISE" @ 0x01268A5C.
// Layout: Ego_r.pdb CTCFireballSpell minus 4 after the CWillButtonHelperTimer (12-byte vector): Timer +0x0C (CurrentState
//         +0x20), PDef +0x40, OneAnimCastMode +0x51, PTarget +0x52, PItemRemoved +0x5A (2-byte packed pointers);
//         retail CSpecialAbilitiesFireballSpellDef TargettingArcAngle +0x50.
// Shape: the intelligent-pointer conversions are plain inline members; VC7.1 inlines the later call sites and calls the
//        out-of-line copies for the first ones, as retail. The validity call goes through an inline helper that names the
//        def first, which is what loads PDef into ebx before the target's GetPItem call.
#pragma pack(push, 2)
class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* PItem;
};
class CThing;
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    inline CIntelligentPointer() {}
    inline CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    template<class U> inline CIntelligentPointer(CIntelligentPointer<U>& other) { SetPItem(other.GetPItem()); }   // 0x004A9730 out of line
    inline CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
    T* Get() const { return (T*)GetPItem(); }
};
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type) const;         // 0x004365B0
};
class CPlayer {
public:
    unsigned char _pad00[0x20a];
    bool AutoTargetCreatures;                           // +0x20A
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char Flags6C;                              // +0x6C
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char Flags91;                              // +0x91
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    CPlayer& GetPlayer();                               // 0x004C7A30
};
namespace N_0062e900 { class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();                     // 0x0062E900
    unsigned char _pad04[0x2c - 0x04];
    CIntelligentPointer<const CThing> PLookAtThing;     // +0x2C
    unsigned char _pad34[0x114 - 0x34];
}; } using N_0062e900::CCreatureActionBase;
class CCreatureAction_FinishFireballSpell : public CCreatureActionBase {
public:
    CCreatureAction_FinishFireballSpell(class CThingCreatureBase& creature, CThing& target);   // 0x008BCE50
    CCreatureAction_FinishFireballSpell(class CThingCreatureBase& creature);                   // 0x008BCE10
    void SetLookAtThing(CIntelligentPointer<const CThing> thing);                              // 0x0072D360
};
class CThingCreatureBase : public CThing {
public:
    void FinishCurrentAction();                         // 0x00662650
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
    void MakeNoise(const CCharString& noise);           // 0x00662110
};
class CTCBase {
public:
    virtual void v00();
    CThingCreatureBase* Thing;                          // +0x04
    unsigned char _pad08[4];
};
class CTCTargeting : public CTCBase {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32();
    virtual CThing* GetTarget();                        // slot 33 (+0x84)
};
class CTCEnemy {
public:
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);   // 0x0076D2F0
};
bool __fastcall IsValidRangedWeaponTargetForShooter(const CThing* shooter, const CThing* target, float range);   // 0x0085F8D0
class CSpecialAbilitiesFireballSpellDef { public:
    unsigned char _pad00[0x50];
    float TargettingArcAngle;                           // +0x50
};
struct CWillButtonHelperTimer {
    long ButtonHeldDownTimer;
    long WillAccumulatedLevel;
    CIntelligentPointer<CThing> PCreature;
    long MyAbility;
    long CurrentState;                                  // +0x14
    unsigned char _pad18[0x34 - 0x18];
};
class CTCFireballSpell : public CTCBase {
public:
    CWillButtonHelperTimer Timer;                       // +0x0C
    const CSpecialAbilitiesFireballSpellDef* PDef;      // +0x40
    unsigned long InitLoopingSound;                     // +0x44
    CIntelligentPointer<CThing> FireballBuildUpEffect;  // +0x48
    bool ImmediateCastMode;                             // +0x50
    bool OneAnimCastMode;                               // +0x51
    CIntelligentPointer<CThing> PTarget;                // +0x52
    CIntelligentPointer<CThing> PItemRemoved;           // +0x5A
    __forceinline bool IsValidTarget(const CThing* thing) const
    {
        const CSpecialAbilitiesFireballSpellDef* def = PDef;
        return IsValidRangedWeaponTargetForShooter(thing, PTarget.Get(), def->TargettingArcAngle);
    }
    void FrameUpdate();
};
#pragma pack(pop)

void CTCFireballSpell::FrameUpdate()
{
    CThingCreatureBase* thing = Thing;
    if (thing->HasTC(8)) {
        CThing* target = ((CTCTargeting*)thing->GetTC(8))->GetTarget();
        if (target && !(target->Flags91 & 1)) {
            if (!(target->Flags6C & 8) || CTCEnemy::AreThingsEnemies(*thing, *target))
                PTarget = target;
            else if (thing->GetPlayer().AutoTargetCreatures)
                PTarget = target;
        }
    }
    if (!OneAnimCastMode) {
        thing->FinishCurrentAction();
        bool started;
        if (PTarget.GetPItem() && !(PTarget.Get()->Flags91 & 1) && IsValidTarget(thing)) {
            CCreatureAction_FinishFireballSpell action(*thing, *(CThing*)PTarget.Get());
            if (PItemRemoved.GetPItem() && !(PItemRemoved.Get()->Flags91 & 1))
                action.SetLookAtThing(CIntelligentPointer<const CThing>(PItemRemoved));
            started = thing->SetCurrentAction(action);
        } else {
            CCreatureAction_FinishFireballSpell action(*thing);
            if (PItemRemoved.GetPItem() && !(PItemRemoved.Get()->Flags91 & 1))
                action.PLookAtThing = PItemRemoved;
            started = thing->SetCurrentAction(action);
        }
        if (!started) {
            Timer.CurrentState = 0;
            return;
        }
    }
    {
        CCharString noise("SIMPLE_NOISE", -1);
        thing->MakeNoise(noise);
    }
}