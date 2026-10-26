// CCombatManager::ResetDefenderAttackPattern(CCombatantInfo& info, CDefPointer<const CAttackPatternDef> pattern) -- install a new attack pattern on a defender, rewind its index and instruction frame, and stop every combat ring's rotation
// Leaf: ResetDefenderAttackPattern
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8C30, 164 bytes (176 with padding), `ret 8` (the def pointer is by value and
//   released here); `this` is not used.
// Callees: none direct -- the def's destroy is vtable slot 1 (`call [eax+4]`) when its count (+4) reaches zero.
// Globals: the frames-per-second long 0x01375550, the float 0.0f 0x0122DEDC.
// Layout: CCombatantInfo / CCombatWheel / CCombatRing as the landed IncrementDefenderAttackPattern 0x006E8600
//   (CombatWheel +0x40, PAttackPattern +0x64, AttackPatternIndex +0x68, FrameForNextInstruction +0x6c; 28-byte rings).
// Shape: the CDefPointer assignment is inlined (compare, release + null the old def, store, add-ref); the ring walk is
//   `while ((ring = GetRing(i++)) != 0)` with the inlined GetRing (sign test strength-reduced onto the byte offset,
//   unsigned size compare by the /28 reciprocal); DeltaRotation = 0.0f / fps.
extern long g_framesPerSecond;                  // 0x01375550

template<class T> struct CRetailVector {        // retail std::vector: {first, last, end_of_storage}, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};

struct CCombatRing {                            // CDefendingCombatantInfo::CCombatRing, 28 bytes in retail
    float RotationAngle;                        // +0x00
    float DeltaRotation;                        // +0x04
    long  Index;                                // +0x08
    float RingThickness;                        // +0x0c
    unsigned char Prongs[12];                   // +0x10 vector<CProng>
};

struct CCombatWheel {
    CRetailVector<CCombatRing> CombatRings;     // +0x00
    CCombatRing* GetRing(long index)            // 0x006E84E0, inlined here
    {
        if (index >= 0 && (unsigned long)index < CombatRings.size())
            return &CombatRings.first[index];
        return 0;
    }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};

struct CDefBase : CDefVirtual {
    long m_count;                               // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

class CAttackPatternDef : public CDefBase { public: };

template<class T> class CDefPointer { public:
    T* m_p;
    CDefPointer(const CDefPointer& other);
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};

struct CCombatantInfo {                         // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    void* Parent;                               // +0x00
    unsigned char AttackerInfo[0x30];           // +0x04 CAttackingCombatantInfo
    unsigned char Attackers[0x0c];              // +0x34 DefenderInfo.Attackers (map)
    CCombatWheel CombatWheel;                   // +0x40
    float LastVictimWheelUpdatePos[3];          // +0x4c
    long LastWheelUpdateWorldFrame;             // +0x58
    unsigned char PLastAttacker[8];             // +0x5c CIntelligentPointer<CThing>
    CDefPointer<CAttackPatternDef> PAttackPattern;   // +0x64 CDefPointer<const CAttackPatternDef>
    long AttackPatternIndex;                    // +0x68
    long FrameForNextInstruction;               // +0x6c
};

class CCombatManager { public:
    void ResetDefenderAttackPattern(CCombatantInfo& info, CDefPointer<CAttackPatternDef> pattern);
};

void CCombatManager::ResetDefenderAttackPattern(CCombatantInfo& info, CDefPointer<CAttackPatternDef> pattern)
{
    info.PAttackPattern = pattern;
    info.AttackPatternIndex = 0;
    info.FrameForNextInstruction = 0;
    long i = 0;
    CCombatRing* ring;
    while ((ring = info.CombatWheel.GetRing(i++)) != 0)
        ring->DeltaRotation = 0.0f / g_framesPerSecond;
}