// CTCCreatureGenerator::UpdateUndead() -- for every triggerer and every created undead: outside the trigger radius a
// surfaced undead returns to the earth and a buried one (when free to act) is moved to a fresh generation point near the
// triggerer and rises with a strike; inside the radius a buried undead rises with a strike
// Leaf: UpdateUndead
//
// Retail 0x0074ACE0, 624 bytes, plain `ret` (0x498-byte frame: four 0x114-byte action temporaries).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0, NCreatureMode::IsUnderground
// @ 0x0085F120 (__fastcall), CCreatureAction_RiseUp::CCreatureAction_RiseUp(CThingCreatureBase*, CThing*) @ 0x0074DAC0,
// CCharString @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCombatActionBase::SetAnimationName @ 0x008574D0,
// CThingCreatureBase::IsFreeToPerformAction @ 0x00662210 / SetCurrentAction @ 0x006644F0, ~CCreatureActionBase @ 0x0062E900,
// GetUndeadGenerationPoint @ 0x0074A260, physics slot 31 (+0x7c, set position), CCreatureAction_UndeadReturnToEarth
// @ 0x008EC6A0. RiseUpStrike vtable 0x01268F44, name "RISE_UP_STRIKE" @ 0x01268F30.
// Layout (retail, not the PDB header's): CreatedCreatures list head +0x18, CreatureTriggerers start/finish +0x1c/+0x20,
// trigger radius +0x2c, UndeadTriggered +0x88.
struct CThing;
struct CThingCreatureBase;
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
namespace N_0062e900 { struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
    ~CCreatureActionBase();                                  // 0x0062E900
}; } using N_0062e900::CCreatureActionBase;
struct CCombatActionBase : CCreatureActionBase {
    void SetAnimationName(const CCharString& name);          // 0x008574D0
};
struct CCreatureAction_RiseUp : CCombatActionBase {          // vtable 0x01268C9C
    bool m_hasTarget;                                        // +0x114
    CCreatureAction_RiseUp(CThingCreatureBase* owner, CThing* thing);   // 0x0074DAC0
};
struct CCreatureAction_RiseUpStrike : CCreatureAction_RiseUp {   // vtable 0x01268F44
    bool m_strike;                                           // +0x118
    virtual void v00();
    CCreatureAction_RiseUpStrike(CThingCreatureBase* owner, CThing* thing) : CCreatureAction_RiseUp(owner, thing)
    {
        {
            CCharString name("RISE_UP_STRIKE");
            SetAnimationName(name);
        }
        m_strike = true;
    }
};
struct CCreatureAction_UndeadReturnToEarth : CCreatureActionBase {
    CCreatureAction_UndeadReturnToEarth(CThingCreatureBase* creature);   // 0x008EC6A0
};
#define VF(n) virtual void v##n();
struct CTCPhysicsBase {
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18)
    VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30)
    virtual void SetPosition(const C3DVector& pos);          // slot 31 (+0x7c)
};
#undef VF
struct CThing {
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    const C3DVector& GetPosition() const;                    // 0x004C73D0
};
struct CThingCreatureBase : CThing {
    bool IsFreeToPerformAction(const CCreatureActionBase& action);   // 0x00662210
    void SetCurrentAction(const CCreatureActionBase& action);        // 0x006644F0
};
namespace NCreatureMode { bool __fastcall IsUnderground(const CThingCreatureBase& creature); }   // 0x0085F120
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct SListNode {
    SListNode* next;
    SListNode* prev;
    CIntelligentPointer<CThingCreatureBase> data;            // +0x08
};
struct CTCCreatureGenerator {
    virtual void v00();
    CThing*        ParentThing;                              // +0x04
    unsigned char  _pad08[0x18 - 0x08];
    SListNode*     CreatedCreatures;                         // +0x18 list head node
    struct TVec { CIntelligentPointer<CThingCreatureBase>* _M_start; CIntelligentPointer<CThingCreatureBase>* _M_finish; bool empty() const { return _M_start == _M_finish; } } Triggerers;   // +0x1c
    unsigned char  _pad24[0x2c - 0x24];
    float          TriggerRadius;                            // +0x2c
    unsigned char  _pad30[0x88 - 0x30];
    bool           UndeadTriggered;                          // +0x88
    bool GetUndeadGenerationPoint(CThing* creature, C3DVector* out);   // 0x0074A260
    void UpdateUndead();
};

void CTCCreatureGenerator::UpdateUndead()
{
    if (Triggerers.empty())
        UndeadTriggered = false;
    for (CIntelligentPointer<CThingCreatureBase>* it = Triggerers._M_start; it != Triggerers._M_finish; ++it) {
        CThingCreatureBase* triggerer = it->GetPItem();
        for (SListNode* node = CreatedCreatures->next; node != CreatedCreatures; node = node->next) {
            CThingCreatureBase* creature = node->data.GetPItem();
            const C3DVector& creaturePos = creature->GetPosition();
            const C3DVector& triggererPos = triggerer->GetPosition();
            C3DVector delta;
            delta.X = triggererPos.X - creaturePos.X;
            delta.Y = triggererPos.Y - creaturePos.Y;
            bool underground = NCreatureMode::IsUnderground(*creature);
            if (delta.Y * delta.Y + delta.X * delta.X > TriggerRadius * TriggerRadius) {
                if (underground) {
                    bool free;
                    {
                        CCreatureAction_RiseUpStrike action(creature, triggerer);
                        free = creature->IsFreeToPerformAction(action);
                    }
                    C3DVector pos;
                    if (free && GetUndeadGenerationPoint(triggerer, &pos)) {
                        creature->PhysicsTC->SetPosition(pos);
                        creature->SetCurrentAction(CCreatureAction_RiseUpStrike(creature, triggerer));
                    }
                } else {
                    creature->SetCurrentAction(CCreatureAction_UndeadReturnToEarth(creature));
                }
            } else if (underground) {
                creature->SetCurrentAction(CCreatureAction_RiseUpStrike(creature, triggerer));
            }
        }
    }
}