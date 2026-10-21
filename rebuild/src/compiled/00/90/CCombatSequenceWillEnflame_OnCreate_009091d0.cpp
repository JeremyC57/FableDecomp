// CCombatSequence_WillEnflame::OnCreate() -- queue a CCreatureAction_PlayCombatAnimation on "ST_BUILDUP_ENFLAME" (flag 1) and a 0x11C-byte enflame action (owner, 2), then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x009091D0, 237 bytes (192 with padding), virtual `?OnCreate@CCombatSequence_WillEnflame@@UAEXXZ`.
// Callees: operator new 0x00BFEA1A (0x118 bytes), CCharString::CCharString(const char*, int) 0x0099EBF0 ("ST_BUILDUP_ENFLAME"
//   @ 0x012801D0), CCreatureAction_PlayCombatAnimation ctor 0x008B7F80, the 0x11C-byte action ctor 0x008BB450 (owner, 2), CCountedPointer::Reset
//   0x00905FD0, ~CCharString 0x0099EAE0, AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
// Layout: sequence owner +0x1c, target +0x20 (landed sibling OnCreate model); engine header CCombatSequence_WillEnflame.h
//   is the PDB size only.
// Shape: the string temporary lives only on the allocated branch of the new-expression, so its destruction is guarded by
//   the construction flag (`test bl,1`) -- the same register that carries the constant 1 argument. The literal is
//   converted implicitly (default length -1), which builds the temporary before the other arguments are pushed.
// Shape: owner/target reach the action constructor through inline accessors (GetCreature/GetTarget); reading the
//   members directly rotates the owner/target/argument registers (eax/ecx/edx) in the VC7.1 allocator.
// Leaf: OnCreate
struct CThingCreatureBase;
struct CThing;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CSoundPair {                         // the zeroed two-dword sound argument of CCombatActionBase
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    CCombatActionBase() { }
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCountedPointer {
    CCombatActionBase* m_pObject;           // +0
    SCountBlock* m_pCount;                  // +4
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
    CThingCreatureBase* m_pOwner;           // +0x1c
    CThing* m_pTarget;                      // +0x20
    CThingCreatureBase* GetCreature() const { return m_pOwner; }
    CThing* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
};

struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {
    unsigned char _body[0x114];                   // sizeof = 0x118
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* owner, CThing* thing, const CCharString& animation, long flag);   // 0x008B7F80
};

struct CCreatureAction_CastWillEnflameSpell : CCombatActionBase {   // 0x11C bytes; manifest label of 0x008BB450
    unsigned char _body[0x118];
    CCreatureAction_CastWillEnflameSpell(CThingCreatureBase* owner, long mode);   // 0x008BB450
};

struct CCombatSequence_WillEnflame : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequence_WillEnflame::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_PlayCombatAnimation(GetCreature(), GetTarget(), "ST_BUILDUP_ENFLAME", 1);
    AddAction(action);
    action = new CCreatureAction_CastWillEnflameSpell(GetCreature(), 2);
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}