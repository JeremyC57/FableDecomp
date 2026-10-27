// CWorld::EAExpressionWait(const CGameEvent*) -- game-event action: the event player's live controlled creature starts a CCreatureAction_ActivateQuest on the "Expression_Wait" script (no object, resources not loaded)
// Leaf: ?EAExpressionWait@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006299C0, 138 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?EAExpressionWait@CWorld@@QAEXPBVCGameEvent@@@Z; the row is referenced only
//   from the CWorld event-action table initialiser 0x0121B9B0 (store at 0x0121CE80), sits among the landed CWorld::EA*
//   handlers (EAMoveCreature 0x006295C0 before, EAChargeQuickAccessItem 0x00629B10 after, the donor order's
//   ExpressionFollow / ExpressionWait / SpiritAttack slot between them) and builds its action on the literal
//   "Expression_Wait" (0x01259184).
// Callees: CWorld::GetPlayer(unsigned long) 0x0049D850 (protected, Ego_r.pdb), CPlayer::GetPControlledCreature() const
//   0x00487DC0, CCharString::CCharString(const char*, long) 0x0099EBF0, CCreatureAction_ActivateQuest::
//   CCreatureAction_ActivateQuest(CThingCreatureBase&, CThing*, const CCharString&, bool) 0x00843F50 (Ego_r.pdb; vtable
//   0x012752C4 RTTI), CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0, then the temporaries'
//   destructors: ~CBaseIntelligentPointer 0x00A01C10 (PObjectToUse), ~CCharString 0x0099EAE0 (ScriptName),
//   ~CCreatureActionBase 0x00693EF0, ~CCharString (the script-name temporary).
// Layout (retail): CCreatureActionBase 0xA8 bytes; CCreatureAction_ActivateQuest ScriptName +0xA8, LoadResources +0xAC,
//   PObjectToUse +0xAD (CIntelligentPointer is 1-aligned in retail: Ego_r +0xB4/+0xB8/+0xB9, 12 lower); 0xB5 bytes.
//   CGameEvent Player +0x04 (signed char); CThing dead bit +0x91 bit 0.
// Shape: the action's destructor is compiler-generated (no vptr store, members in reverse, the base destructor called
//   directly); the SetCurrentAction result is dropped; the script name is a named local (built before the bool push,
//   passed by address, destroyed last).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CBaseObject;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void* PData;                                        // CBaseObjectPointer*
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)

class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                             // +0x91 bit 0
    bool IsDead() const { return Dead; }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CCreatureActionBase;
class CThingCreatureBase : public CThingGameObject {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CThingPlayerCreature : public CThingCreatureBase { };

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();                     // 0x00693EF0
    unsigned char _pad04[0xa8 - 4];
};
class CCreatureAction_ActivateQuest : public CCreatureActionBase {
public:
    CCreatureAction_ActivateQuest(CThingCreatureBase& creature, CThing* objectToUse, const CCharString& scriptName,
                                  bool loadResources);  // 0x00843F50
    CCharString                  ScriptName;            // +0xA8
    bool                         LoadResources;         // +0xAC
    CIntelligentPointer<CThing>  PObjectToUse;          // +0xAD
};

class CGameEvent {
public:
    long        Type;                                   // +0x00
    signed char Player;                                 // +0x04
};

class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;     // 0x00487DC0 (Ego_r order: GetPControlledCreature, then PeekPControlledCreature at 0x00487DD0)
};

class CWorld {
public:
    void EAExpressionWait(const CGameEvent* event);
protected:
    CPlayer* GetPlayer(unsigned long number);           // 0x0049D850
};

void CWorld::EAExpressionWait(const CGameEvent* event)
{
    CThingPlayerCreature* creature = GetPlayer(event->Player)->GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CCharString scriptName("Expression_Wait");
        creature->SetCurrentAction(CCreatureAction_ActivateQuest(*creature, 0, scriptName, false));
    }
}