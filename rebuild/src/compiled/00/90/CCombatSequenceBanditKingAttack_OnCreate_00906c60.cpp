// CCombatSequence_BanditKingAttack::OnCreate() -- queue a CCreatureAction_BanditKingAttack on the "STRIKE_MEDIUM_FRONT" animation (flags 1, 0), then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00906C60, 177 bytes (192 with padding), virtual `?OnCreate@CCombatSequence_BanditKingAttack@@UAEXXZ`.
// Callees: operator new 0x00BFEA1A (0x118 bytes), CCharString::CCharString(const char*, long) 0x0099EBF0 ("STRIKE_MEDIUM_FRONT"
//   @ 0x012778B0), CCreatureAction_BanditKingAttack::CCreatureAction_BanditKingAttack 0x008C9410, CCountedPointer::Reset
//   0x00905FD0, ~CCharString 0x0099EAE0, AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
// Layout: sequence owner +0x1c, target +0x20 (landed sibling OnCreate model); engine header CCombatSequence_BanditKingAttack.h
//   is the PDB size only.
// Shape: the string temporary lives only on the allocated branch of the new-expression, so its destruction is guarded by
//   the construction flag (`test bl,1`) -- the same register that carries the constant 1 argument. The literal is
//   converted implicitly (default length -1), which builds the temporary before the other arguments are pushed.
// Shape: owner/target reach the action constructor through inline accessors (GetCreature/GetTarget); reading the
//   members directly rotates the owner/target/argument registers (eax/ecx/edx) in the VC7.1 allocator.
// Leaf: OnCreate
class CThingCreatureBase;
class CThing;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

class CSoundPair { public:                         // the zeroed two-dword sound argument of CCombatActionBase
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase { public:
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

class CCombatSequenceBase { public:
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pOwner;           // +0x1c
    CThing* m_pTarget;                      // +0x20
    CThingCreatureBase* GetCreature() const { return m_pOwner; }
    CThing* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
};

class CCreatureAction_BanditKingAttack : public CCombatActionBase { public:
    unsigned char _body[0x114];                   // sizeof = 0x118
    CCreatureAction_BanditKingAttack(CThingCreatureBase* owner, CThing* thing, const CCharString& animation, long first, long second);   // 0x008C9410
};

struct CCombatSequence_BanditKingAttack : CCombatSequenceBase {
    virtual void OnCreate();
};

void CCombatSequence_BanditKingAttack::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_BanditKingAttack(GetCreature(), GetTarget(), "STRIKE_MEDIUM_FRONT", 1, 0);
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}