// CAIStateGroup_CreatureRangedCombat2::Cleanup() -- on world shutdown just cleans up the ranged weapon; otherwise adds the "fire ranged weapon on leaving state group" component and (unless a strafe was added) triggers it; clears AddedStrafe
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091E990, length 114 bytes (+ int3 padding).
// Callees: the state group's world accessor @ 0x008FCFC0 (PBrain->+0x1c->+0x24; a twin of 0x008FCFA0), the
// world's virtual slot 12 (+0x30) whose result carries a flag byte at +0xd4, CleanupRangedWeapon(CThing*)
// @ 0x007E5B40 (__fastcall; landed caller 0x007E5CC0), CCharString::CCharString(const char*, long) @ 0x0099EBF0 /
// ~CCharString @ 0x0099EAE0 on "CTCFireRangedWeaponOnLeavingStateGroup" @ 0x0123E434, CThing::AddTC(const
// CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, the new component's virtual slot 28 (+0x70).
// Shape: an if/else whose shared trailing `AddedStrafe = false` is tail-duplicated into both arms (each arm has
// its own epilogue); the class name is an IMPLICIT conversion temporary of AddTC's const-reference argument
// (constructed into the `push ecx` frame slot before the creature is loaded, re-addressed with `lea` for the
// push, destroyed right after the call); the creature is fetched through an inline accessor so the chain lands
// in ecx before the pushes (a direct PBrain->Creature splits through edx); the component is kept in edi across
// the destructor.
// Model: local (engine/CAIStateGroup_CreatureRangedCombat2.h flattens the base; AddedStrafe +0x1c from it).
class CTCParamsBase;
class CThing;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

void __fastcall CleanupRangedWeapon(CThing* creature);     // 0x007E5B40

class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual void Trigger();                                 // slot 28 (+0x70)
};

class CThing
{
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};
class CThingPhysical : public CThing
{
public:
};
class CThingGameObject : public CThingPhysical
{
public:
};
class CThingCreatureBase : public CThingGameObject
{
public:
};

class CThingAICreature : public CThingCreatureBase { public:
};

struct CGameState {
    unsigned char _pad[0xd4];
    unsigned char m_bShuttingDown;                          // +0xd4
};

class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11();
    virtual CGameState* GetGameState();                     // slot 12 (+0x30)
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                             // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                       // +0x04
    long ScriptAIPriority;                                  // +0x08
    void* Name_PStringData;                                 // +0x0c
    unsigned char Terminated;                               // +0x10
    unsigned char Priority;                                 // +0x11
    unsigned char Enabled;                                  // +0x12
    unsigned char _pad_0x13;
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0
public:
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
};

struct CAIStateGroup_CreatureRangedCombat2 : CAIStateGroupBase {
    unsigned char PTargetThing[8];                          // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool AddedStrafe;                                       // +0x1c
    virtual void Cleanup();
};

void CAIStateGroup_CreatureRangedCombat2::Cleanup()
{
    if (GetWorld().GetGameState()->m_bShuttingDown) {
        CleanupRangedWeapon((CThing*)PBrain->Creature);
    } else {
        CTCBase* fireOnLeaving = GetCreature()->AddTC("CTCFireRangedWeaponOnLeavingStateGroup", false, 0);
        if (!AddedStrafe)
            fireOnLeaving->Trigger();
    }
    AddedStrafe = false;
}