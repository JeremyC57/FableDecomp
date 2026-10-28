// CTCCreatureGenerator::UpdateUndead() -- for every triggerer and every created undead: outside the trigger radius a
// surfaced undead returns to the earth and a buried one (when free to act) is moved to a fresh generation point near the
// triggerer and rises with a strike; inside the radius a buried undead rises with a strike
// Leaf: UpdateUndead
//
// Retail 0x0074ACE0, 624 bytes, plain `ret` (0x498-byte frame: four 0x114-byte action temporaries).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPos @ 0x004C73D0, NCreatureMode::IsUnderground
// @ 0x0085F120 (__fastcall), CCreatureAction_RiseUp::CCreatureAction_RiseUp(CThingCreatureBase*, CThing*) @ 0x0074DAC0,
// CCharString @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCombatActionBase::SetCombatAnimation @ 0x008574D0,
// CThingCreatureBase::IsFreeToPerformAction @ 0x00662210 / SetCurrentAction @ 0x006644F0, ~CCreatureActionBase @ 0x0062E900,
// GetUndeadGenerationPoint @ 0x0074A260, physics slot 31 (+0x7c, set position), CCreatureAction_UndeadReturnToEarth
// @ 0x008EC6A0. RiseUpStrike vtable 0x01268F44, name "RISE_UP_STRIKE" @ 0x01268F30.
// Layout (retail, not the PDB header's): CreatedCreatures list head +0x18, CreatureTriggerers start/finish +0x1c/+0x20,
// trigger radius +0x2c, UndeadTriggered +0x88.
class CThing;
class CThingCreatureBase;
class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
namespace N_0062e900 { struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
    ~CCreatureActionBase();                                  // 0x0062E900
}; } using N_0062e900::CCreatureActionBase;
class CCombatActionBase : public CCreatureActionBase { public:
    void SetCombatAnimation(const CCharString& name);          // 0x008574D0
};
class CCreatureAction_RiseUp : public CCombatActionBase { public:          // vtable 0x01268C9C
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
            SetCombatAnimation(name);
        }
        m_strike = true;
    }
};
class CCreatureAction_UndeadReturnToEarth : public CCreatureActionBase { public:
    CCreatureAction_UndeadReturnToEarth(CThingCreatureBase* creature);   // 0x008EC6A0
};
#define VF(n) virtual void v##n();
class CTCPhysicsBase { public:
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18)
    VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30)
    virtual void SetPosition(const C3DVector& pos);          // slot 31 (+0x7c)
};
#undef VF
class CThing { public:
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    const C3DVector& GetPos() const;                    // 0x004C73D0
};
class CThingCreatureBase : public CThing { public:
    bool IsFreeToPerformAction(const CCreatureActionBase& action);   // 0x00662210
    void SetCurrentAction(const CCreatureActionBase& action);        // 0x006644F0
};
namespace NCreatureMode { bool __fastcall IsUnderground(const CThingCreatureBase& creature); }   // 0x0085F120
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }

    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct SListNode {
    SListNode* next;
    SListNode* prev;
    CIntelligentPointer<CThingCreatureBase> data;            // +0x08
};
class CTCCreatureGenerator { public:
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
            const C3DVector& creaturePos = creature->GetPos();
            const C3DVector& triggererPos = triggerer->GetPos();
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