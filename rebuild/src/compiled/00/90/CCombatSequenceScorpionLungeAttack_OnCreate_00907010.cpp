// CCombatSequence_ScorpionLungeAttack::OnCreate() -- stamp the sequence with the current frame plus a world-random 0..(10 s of frames) offset, queue the lunge action, then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00907010, 200 bytes (208 with padding), virtual OnCreate.
// Callees: CWorld::GetPRandomSeed 0x0049D810 on the world at [0x013B86A0]+0x24 (manifest "CVertexBufferWin32::DoSizeof"
//   is propagated), GFROR13 0x00497890 (__fastcall, one step of the world random generator), GFGetCurrentFrame 0x0049D870
//   (__fastcall, no argument), operator new 0x00BFEA1A (0x114 bytes), the action ctor 0x008CCC70 (owner, target;
//   manifest "CCombatAction_GenericResponseToDrain", may be propagated), CCountedPointer::Reset 0x00905FD0, AddAction
//   0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
// Globals: the main game component 0x013B86A0 (world pointer +0x24), the logic frame rate 0x01375550 (15).
// Layout: owner +0x1c, target +0x20, m_frameOfNextUse +0x28 (retail offsets as the landed BattleCharge IsValid 0x00908DA0).
// Shape: the frame rate is read into a named local before the seed call; the range helper tests `range != 0` first
//   (retail places the `div` on the fall-through, unlike the landed 0x006E8730).
// Shape: owner/target reach the action constructor through inline accessors (GetCreature/GetTarget); reading the
//   members directly rotates the owner/target/argument registers (eax/ecx/edx) in the VC7.1 allocator.
// Leaf: OnCreate
long __fastcall GFROR13(unsigned long);   // 0x00497890
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                     // 0x0049D870

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range != 0)
        return seed % range;
    return 0;
}

struct CWorld_Calls {
    unsigned long* GetPRandomSeed();                            // 0x0049D810
};

struct CMainGameComponent_Retail {
    unsigned char _pad[0x24];
    CWorld_Calls* World;                                        // +0x24
};

extern CMainGameComponent_Retail* g_pMainGameComponent;        // 0x013B86A0
extern long g_logicFramesPerSecond;                             // 0x01375550 (15)
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

struct CCombatAction_008ccc70 : CCombatActionBase {
    unsigned char _body[0x110];                                 // sizeof = 0x114
    CCombatAction_008ccc70(CThingCreatureBase* owner, CThing* thing);   // 0x008CCC70
};

struct CCombatSequence_ScorpionLungeAttack : CCombatSequenceBase {
    unsigned char _pad24[4];                                    // +0x24
    long m_frameOfNextUse;                                      // +0x28
    virtual void OnCreate();
};

void CCombatSequence_ScorpionLungeAttack::OnCreate()
{
    long framesPerSecond = g_logicFramesPerSecond;
    unsigned long* seed = g_pMainGameComponent->World->GetPRandomSeed();
    unsigned long offset = WorldRandomRange(*seed, framesPerSecond * 10);
    m_frameOfNextUse = CWorld::GetFrame() + offset;
    CCountedPointer action;
    action = new CCombatAction_008ccc70(GetCreature(), GetTarget());
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}