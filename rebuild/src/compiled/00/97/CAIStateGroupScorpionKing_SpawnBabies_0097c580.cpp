// CAIStateGroup_ScorpionKing::SpawnBabies() -- the scorpion king performs its summon action against the current enemy and waits for it; the state terminates if it is no longer runnable afterwards
// Leaf: ?SpawnBabies@CAIStateGroup_ScorpionKing
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097C580, 81 bytes (+ int3 padding to 96), plain `ret`; a 0x114-byte frame
//   holds the action temporary.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy at +0x30), CCreatureAction_ScorpionSummon::
//   CCreatureAction_ScorpionSummon(CThingCreatureBase&, CThingCreatureBase&) @ 0x008E46D0 (vtable 0x012865DC; its
//   strings "SWORD_SWING" / "SUMMON"), CAIStateGroupBase::AIAction_PerformActionAndWait @ 0x0097DB70,
//   ~CCombatActionBase @ 0x0062E900 (the temporary's destructor), CAIStateGroup_ScorpionKing::IsStillRunnable
//   @ 0x0097B870, CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: `AIAction_PerformActionAndWait(&CCreatureAction_ScorpionSummon(*creature, *PEnemy.GetPItem()), false);` -- the
//   creature is a named local read first (edi), the action is a full-expression temporary (no EH frame: the TU has no
//   /EHsc, the destructor runs straight after the call), then `if (!IsStillRunnable()) Terminate();`.
// Layout: the retail CAIStateGroup_ScorpionKing: PArenaCentre +0x14, TrapThings (12-byte STLport vector) +0x1c,
//   bIsRangeAttack +0x28, ChargeCount +0x2c, PEnemy +0x30 (Ego_r.pdb's debug vector is 16 bytes, so it lists PEnemy at
//   +0x34); the summon action is a 0x114-byte CCombatActionBase.
// Label: catalog "CAIStateGroup_ScorpionKing_ExecuteBriarRoseAbility" -> Ego_r.pdb
//   ?SpawnBabies@CAIStateGroup_ScorpionKing@@IAEXXZ; 0x008E46D0 is the CCreatureAction_ScorpionSummon constructor (not
//   BriarRoseShotFarInto) and 0x0062E900 is ~CCombatActionBase (not ~CTCLightningOrb).
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
class CThingCreatureBase;

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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                  // +0x04..+0xa7
};

class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];                // +0xa8..+0x113
    virtual ~CCombatActionBase();                       // 0x0062E900
};

class CCreatureAction_ScorpionSummon : public CCombatActionBase { public:
    CCreatureAction_ScorpionSummon(CThingCreatureBase& creature, CThingCreatureBase& enemy);   // 0x008E46D0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    protected: EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
};

class CAIStateGroup_ScorpionKing : public CAIStateGroupBase {
public:
    CIntelligentPointer<CThingCreatureBase> PArenaCentre;       // +0x14 CIntelligentPointer<CThing>
    unsigned char TrapThings[0x0c];             // +0x1c STLport vector<CIntelligentPointer<CThing> >
    bool bIsRangeAttack;                        // +0x28
    long ChargeCount;                           // +0x2c
    CIntelligentPointer<CThingCreatureBase> PEnemy;             // +0x30 CIntelligentPointer<CThingCreatureBase>
    long EnemyKnockDownFrame;                   // +0x38
    float HealthAtLastTailStrike;               protected: // +0x3c
    bool IsStillRunnable(); public:                     // 0x0097B870
protected:
    void SpawnBabies();
};

void CAIStateGroup_ScorpionKing::SpawnBabies()
{
    CThingCreatureBase* creature = GetCreature();
    AIAction_PerformActionAndWait(CCreatureAction_ScorpionSummon(*creature, *PEnemy.GetPItem()), false);
    if (!IsStillRunnable())
        Terminate();
}