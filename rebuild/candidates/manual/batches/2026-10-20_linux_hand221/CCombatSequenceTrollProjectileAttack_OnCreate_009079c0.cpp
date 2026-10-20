// CCombatSequence_TrollProjectileAttack::OnCreate() -- queue the two troll projectile attacks (PROJECTILE_ATTACK_FIRST / _SECOND), then the base OnCreate
// Leaf: ?OnCreate@CCombatSequence_TrollProjectileAttack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009079C0, 288 bytes (incl. int3 padding).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//          CCreatureAction_TrollProjectileAttack(owner, target, const CCharString&) 0x009080B0 (0x11c bytes),
//          CCountedPointer::Reset 0x00905FD0, AddAction 0x008B0B10, base OnCreate 0x008B0650,
//          operator delete 0x00BFE9BC.
// Shape: the landed OnCreate family with a string temporary inside the new-expression: the CCharString
// argument is only constructed when operator new succeeded, so VC7.1 tracks it with flag bits in ebx
// (`mov ebx,1` / `or ebx,2`, `test bl,N ; je` around the destructor after the full expression). The string is an
// IMPLICIT conversion of the literal (re-addressed with `lea edx` for the push); an explicit temporary pushes eax.
// Owner/target are read through inline const getters GetCreature()/GetTarget(): direct member reads give
// a different eax/ecx/edx rotation for the argument loads and the AddAction address temporaries.
struct CThingCreatureBase;
struct CThing;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                              // 0x0099EAE0
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x110];
};
struct CCreatureAction_TrollProjectileAttack : CCombatActionBase {
    unsigned char OverriddenAnim;           // +0x114
    CCharString   AnimName;                 // +0x118
    CCreatureAction_TrollProjectileAttack(CThingCreatureBase* owner, CThing* target, const CCharString& animName); // 0x009080B0
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

struct CCombatSequenceBase {
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pCreature;        // +0x1c
    CThing* m_pTarget;                      // +0x20
    CThingCreatureBase* GetCreature() const { return m_pCreature; }
    CThing* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};
struct CCombatSequence_TrollProjectileAttack : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequence_TrollProjectileAttack::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_TrollProjectileAttack(GetCreature(), GetTarget(), "PROJECTILE_ATTACK_FIRST");
    AddAction(action);
    action = new CCreatureAction_TrollProjectileAttack(GetCreature(), GetTarget(), "PROJECTILE_ATTACK_SECOND");
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}
