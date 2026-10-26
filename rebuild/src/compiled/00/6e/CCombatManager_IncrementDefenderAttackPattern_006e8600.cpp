// CCombatManager::IncrementDefenderAttackPattern(CCombatantInfo&) -- steps a defender to the next entry of its attack pattern (wrapping), schedules the next instruction and, on an attack entry, stops the first combat ring's rotation
// Leaf: IncrementDefenderAttackPattern
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8600, 148 bytes of code (`ret 4`).
// Callees: GetWorldFrame @ 0x0049D870, __ftol2 @ 0x00BFEA70.
// Globals: the frames-per-second long @ 0x01375550 (copied to the frame for `fild`), the float 0.0f @ 0x0122DEDC.
// Shape: the pattern vector's address (def +0x3c) is formed once (edi); the index pre-increment is compared unsigned
//          with size(); the world frame is spilled into the dead argument slot for `fild`; the first ring comes from the
//          inlined CCombatWheel::GetRing(0) (size test then null test of the data pointer, the /28 reciprocal
//          multiply). AttackPatternIdleStandardInterval is CCombatManager +0x30 (PDB +0x38: retail maps are 12 bytes).
extern long g_framesPerSecond;                  // 0x01375550
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                           // 0x0049D870 (returns the global @ 0x013B89BC; label "GFGetPreMainMemoryUsed" propagated)

template<class T> struct CRetailVector {        // retail std::vector: {first, last, end_of_storage}, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    T* begin() const { return first; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CCombatRing {                            // CDefendingCombatantInfo::CCombatRing, 28 bytes in retail
    float RotationAngle;                        // +0x00
    float DeltaRotation;                        // +0x04
    long  Index;                                // +0x08
    float RingThickness;                        // +0x0c
    unsigned char Prongs[12];                   // +0x10 vector<CProng>
};

struct CCombatWheel {                           // one vector of rings
    CRetailVector<CCombatRing> CombatRings;     // +0x00
    CCombatRing* GetRing(long index)            // 0x006E84E0, inlined here
    {
        if (index >= 0 && (unsigned long)index < CombatRings.size())
            return &CombatRings.first[index];
        return 0;
    }
};

class CAttackPatternDef { public:
    unsigned char _pad00[0x3c];
    CRetailVector<long> AttackPattern;          // +0x3c (PDB +0x40)
};

struct CCombatantInfo {                         // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    void* Parent;                               // +0x00
    unsigned char AttackerInfo[0x30];           // +0x04 CAttackingCombatantInfo
    unsigned char Attackers[0x0c];              // +0x34 DefenderInfo.Attackers (map)
    CCombatWheel CombatWheel;                   // +0x40
    float LastVictimWheelUpdatePos[3];          // +0x4c
    long LastWheelUpdateWorldFrame;             // +0x58
    unsigned char PLastAttacker[8];             // +0x5c CIntelligentPointer<CThing>
    CAttackPatternDef* PAttackPattern;          // +0x64 CDefPointer<const CAttackPatternDef>
    long AttackPatternIndex;                    // +0x68
    long FrameForNextInstruction;               // +0x6c
};

class CCombatManager { public:
    unsigned char _pad00[0x30];
    float AttackPatternIdleStandardInterval;    // +0x30
    void IncrementDefenderAttackPattern(CCombatantInfo& info);
};

void CCombatManager::IncrementDefenderAttackPattern(CCombatantInfo& info)
{
    CRetailVector<long>& pattern = info.PAttackPattern->AttackPattern;
    if ((unsigned long)++info.AttackPatternIndex >= pattern.size())
        info.AttackPatternIndex = 0;
    long framesPerSecond = g_framesPerSecond;
    info.FrameForNextInstruction = (long)(CWorld::GetFrame() + framesPerSecond * AttackPatternIdleStandardInterval);
    if (pattern[info.AttackPatternIndex] > 0) {
        CCombatRing* ring = info.CombatWheel.GetRing(0);
        if (ring)
            ring->DeltaRotation = 0.0f / g_framesPerSecond;
    }
}