// CCombatSequence_WillForcePushLongRange::OnCreate() -- queue the force-push "into" combat animation and the force-push release cast, then the base OnCreate
// Leaf: ?OnCreate@CCombatSequence_WillForcePushLongRange
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00909520, 288 bytes (incl. int3 padding).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//          CCreatureAction_PlayCombatAnimation(owner, target, const CCharString&, bool) 0x008B7F80 (0x118 bytes),
//          CCreatureAction_CastWillForceSpell(owner, long, bool, const CCharString&) 0x008BB200 (0x11c bytes),
//          CCountedPointer::Reset 0x00905FD0, AddAction 0x008B0B10, base OnCreate 0x008B0650,
//          operator delete 0x00BFE9BC.
// Shape: same as 0x009079C0 TrollProjectileAttack: the CCharString argument is an implicit conversion inside
// the new-expression, constructed only when operator new succeeded, so VC7.1 tracks it with flag bits in
// ebx (`mov ebx,1` doubles as the `true` argument: `push ebx`). Owner/target are read through inline const
// getters. Strings: "ST_FORCE_PUSH_INTO" (0x012801A0), "ST_FORCE_PUSH_RELEASE" (0x01280B8C).
class CThingCreatureBase;
class CThing;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                              // 0x0099EAE0
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x110];
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:
    unsigned char _body2[4];                // sizeof 0x118
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* owner, CThingCreatureBase* target, const CCharString& anim, bool flag); // 0x008B7F80
};
class CCreatureAction_CastWillForceSpell : public CCombatActionBase { public:
    unsigned char _body2[8];                // sizeof 0x11c
    CCreatureAction_CastWillForceSpell(CThingCreatureBase* owner, long level, bool flag, const CCharString& anim); // 0x008BB200
};

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CCountedPointer {
    CCombatActionBase* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(CCombatActionBase* object) { Reset(object); return *this; }
    void Reset(CCombatActionBase* object);  // 0x00905FD0
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

class CCombatSequenceBase { public:
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pCreature;        // +0x1c
    CThingCreatureBase* m_pTarget;          // +0x20
    CThingCreatureBase* GetCreature() const { return m_pCreature; }
    CThingCreatureBase* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};
struct CCombatSequence_WillForcePushLongRange : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequence_WillForcePushLongRange::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_PlayCombatAnimation(GetCreature(), GetTarget(), "ST_FORCE_PUSH_INTO", true);
    AddAction(action);
    action = new CCreatureAction_CastWillForceSpell(GetCreature(), 1, true, "ST_FORCE_PUSH_RELEASE");
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}