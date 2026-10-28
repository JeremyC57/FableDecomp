// CAIStateGroup_WhisperPotionCombat::IsRunnableLeaf() -- always runnable in boss phase 1; in phase 2 only once 10 s have passed since the last run: restamp the frame (plus a random 0..5 s delay), and run when the creature is within the combat-type range of the target and a valid jump position is found
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096AAD0, length 330 bytes (plain `ret`, virtual, bool).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 173 = bit 13 of the bitset dword at +0x34),
//          CTCBoss::GetBossPhase @ 0x007AED10 (manifest "CWorld::DrawGetEnvironment" is propagated), CAIStateGroupBase::
//          GetPWorld @ 0x008FCFC0 (manifest "CPlayer::GetCurrentMode" propagated), GetWorldFrame @ 0x0049D870 (static,
//          manifest "GFGetPreMainMemoryUsed"), CWorld::GetPRandomSeed @ 0x0049D810 (manifest "CVertexBufferWin32::DoSizeof"),
//          GFROR13 @ 0x00497890, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0,
//          CThingCreatureBase::GetCombatTypeDef @ 0x00666D10, the jump-position finder @ 0x0096A3C0 (manifest
//          "FindValidCombatPositions"); data: the frame rate @ 0x01375550 (long), 10.0f @ 0x01237F0C.
// Shape: the phase is a three-way switch (`dec eax ; je` / `dec eax ; jne`); the elapsed-frame test is `(float)delta >
//          (float)rate * 10.0f` (`fcompp ; test ah,5 ; jp` reject); the restamp + random delay + planar distance test sit in
//          a __forceinline helper so VC7.1 keeps that region's `push ebx` / `pop ebx` inside the phase-2 branch (ebx holds
//          the frame rate, scaled by 5 with `lea`, then the creature); the zero-guarded world random is the landed
//          WorldRandomRange shape (`test ebx,ebx ; je` around the div); `dx*dx + dy*dy` squares dy (top of stack) first;
//          the distance is spilled across GetCombatTypeDef and compared `< def+0x6c` (`fcomp ; test ah,5 ; jp`).
// Layout: engine/CAIStateGroup_WhisperPotionCombat.h is 4 bytes high after TrapThings (PDB vector 16 vs retail 12):
//          retail PTarget +0x14 (CIntelligentPointer<CThing>, PDB `_pad`), TrapThings +0x1c, FrameLastRun +0x28,
//          Jump_to_pos +0x2c. CCombatTypeDef +0x6c is a float range (PDB offsets are vector-shifted; descriptive name).
#include <math.h>
class CTCBase;
class CThing;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class C3DVector { public: float X, Y, Z; };

class CCombatTypeDef { public:
    unsigned char _pad[0x6c];
    float PotionCombatRange;                    // +0x6c (descriptive)
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPos() const;    // 0x004C73D0
    CCombatTypeDef* GetCombatTypeDef();         // 0x00666D10 (CThingCreatureBase)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

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
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCBoss { public:                                // interface 173
    long GetBossPhase();                        // 0x007AED10
};

class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
extern long g_FrameRate;                        // 0x01375550

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range != 0)
        return seed % range;
    return 0;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
};

struct CAIStateGroup_WhisperPotionCombat : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;            // +0x14 CIntelligentPointer<CThing>
    unsigned char TrapThings[0xc];              // +0x1c vector<CIntelligentPointer<CThing> > (retail STLport, 12 bytes)
    long FrameLastRun;                          // +0x28
    C3DVector Jump_to_pos;                      // +0x2c
    bool FindValidCombatPositions(C3DVector* jumpPos);   // 0x0096A3C0
    __forceinline bool RestampAndCheckRange();
    bool IsRunnableLeaf();
};

// Restamp the last-run frame with a random extra delay of up to 5 s, then test the planar distance to the target
// against the creature's combat-type range.
__forceinline bool CAIStateGroup_WhisperPotionCombat::RestampAndCheckRange()
{
    FrameLastRun = GetPWorld()->GetWorldFrame();
    long rate = g_FrameRate;
    unsigned long* seed = GetPWorld()->GetPRandomSeed();
    FrameLastRun += WorldRandomRange(*seed, rate * 5);
    CThing* creature = PBrain->PCreature;
    const C3DVector* targetPos = &PTarget.GetPItem()->GetPos();
    const C3DVector* pos = &creature->GetPos();
    float dx = pos->X - targetPos->X;
    float dy = pos->Y - targetPos->Y;
    float distance = (float)sqrt(dx * dx + dy * dy);
    return distance < PBrain->PCreature->GetCombatTypeDef()->PotionCombatRange;
}

bool CAIStateGroup_WhisperPotionCombat::IsRunnableLeaf()
{
    CTCBoss* boss;
    PBrain->PCreature->GetInterface(&boss, 173);
    switch (boss->GetBossPhase()) {
    case 1:
        return true;
    case 2:
        break;
    default:
        return false;
    }
    long delta = GetPWorld()->GetWorldFrame() - FrameLastRun;
    if ((float)delta > (float)g_FrameRate * 10.0f) {
        if (RestampAndCheckRange()) {
            if (FindValidCombatPositions(&Jump_to_pos))
                return true;
        }
    }
    return false;
}