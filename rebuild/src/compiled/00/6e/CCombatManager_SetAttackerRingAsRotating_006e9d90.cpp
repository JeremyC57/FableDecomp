// CCombatManager::SetAttackerRingAsRotating(const CThingCreatureBase& attacker, float degreesPerSecond) -- when the attacker is eligible and its victim's combat type rotates rings, start the victim's first ring turning (on an idle pattern entry, for the inner-ring attacker)
// Leaf: SetAttackerRingAsRotating
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9D90, 171 bytes (176 with padding), `ret 8`,
//   `?SetAttackerRingAsRotating@CCombatManager@@QAEXABVCThingCreatureBase@@M@Z` (FableWin.pdb export list).
// Callees: the creature test 0x00662410 (manifest "C3DAnimationBlendState::AddCombinationBlend" is propagated),
//   the combatant map's find 0x006F0250 (twice), CThingCreatureBase::GetCombatTypeDef 0x00666D10,
//   CCombatWheel::GetRing(long) 0x006E84E0 (out of line in this TU).
// Globals: 1/360 @ 0x01238E00, the frames-per-second long 0x01375550.
// Layout: CCombatManager / CCombatantInfo / the UID map as the landed 0x006E9E40 (Combatants +0x10, node value +0x18,
//   AttackerInfo +4: Attacked +0, CombatWheelRingIndex +8; PAttackPattern +0x64, AttackPatternIndex +0x68,
//   CombatWheel +0x40); the pattern def's vector at +0x3c (landed 0x006E8600); CCombatTypeDef flag byte +0x4c.
// Shape: the inline GetCombatantInfo takes the creature by reference, so each UID is a GetUID() temporary sharing one
//   8-byte slot; GetRing's body sits in the TU (auto_inline off) so the x87 rotation is computed
//   before the GetRing call and stored after it, without a null test.
extern long g_framesPerSecond;                  // 0x01375550

template<class T> struct CRetailVector {        // retail std::vector: {first, last, end_of_storage}, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
    T& operator[](unsigned long n) const { return first[n]; }
};
struct CCombatRing;

class CCombatTypeDef { public:
    unsigned char _pad[0x4c];
    bool RotatesCombatRings;                    // +0x4c
};

class CThingCreatureBase { public:
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                  // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
    bool IsCombatEligible() const;              // 0x00662410
    CCombatTypeDef* GetCombatTypeDef() const;   // 0x00666D10
};

struct CCombatRing {                            // 28 bytes in retail
    float RotationAngle;                        // +0x00
    float DeltaRotation;                        // +0x04
    unsigned char _rest[0x14];
};


struct CCombatWheel {
    CRetailVector<CCombatRing> CombatRings;     // +0x00
    CCombatRing* GetRing(long index);           // 0x006E84E0
};

#pragma auto_inline(off)
CCombatRing* CCombatWheel::GetRing(long index)  // same-TU body (landed 0x006E84E0): lets the x87 value live across the call
{
    if (index >= 0 && (unsigned long)index < (unsigned long)(CombatRings.last - CombatRings.first))
        return &CombatRings.first[index];
    return 0;
}
#pragma auto_inline(on)

class CAttackPatternDef { public:
    unsigned char _pad00[0x3c];
    CRetailVector<long> AttackPattern;          // +0x3c
};

class CAttackingCombatantInfo { public:
    const CThingCreatureBase* Attacked;         // +0x00
    long AttackerNumber;                        // +0x04
    long CombatWheelRingIndex;                  // +0x08
    unsigned char _rest[0x30 - 0x0c];
};

struct CCombatantInfo {                         // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    const CThingCreatureBase* Parent;           // +0x00
    CAttackingCombatantInfo AttackerInfo;       // +0x04
    unsigned char Attackers[0x0c];              // +0x34
    CCombatWheel CombatWheel;                   // +0x40
    unsigned char _pad4c[0x64 - 0x4c];
    CAttackPatternDef* PAttackPattern;          // +0x64 CDefPointer<const CAttackPatternDef>
    long AttackPatternIndex;                    // +0x68
    long FrameForNextInstruction;               // +0x6c
};

struct CRetailRbMap_UID_CombatantInfo {         // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                   // +0x10
        CCombatantInfo value;                   // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);    // 0x006F0250
};
#pragma auto_inline(off)
// Same-TU body of the STLport map::find instantiation 0x006F0250 (template bodies are compiled in the caller's TU): the
// lower_bound walk reading the 64-bit key through its const reference, then the end() test.
__declspec(noinline) CRetailRbMap_UID_CombatantInfo::node* CRetailRbMap_UID_CombatantInfo::find(const unsigned __int64& key)
{
    node* y = header;
    node* x = header->parent;
    while (x != 0) {
        if (!(x->key < key)) {
            y = x;
            x = x->left;
        } else
            x = x->right;
    }
    if (y == header || key < y->key)
        return header;
    return y;
}
#pragma auto_inline(on)

class CCombatManager { public:
    virtual void v00();
    unsigned char _sequences[0xc];              // +0x04
    CRetailRbMap_UID_CombatantInfo Combatants;  // +0x10
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature)
    {
        return Combatants.find(creature.GetUID())->value;
    }
    void SetAttackerRingAsRotating(const CThingCreatureBase& attacker, float degreesPerSecond);
};

void CCombatManager::SetAttackerRingAsRotating(const CThingCreatureBase& attacker, float degreesPerSecond)
{
    if (!attacker.IsCombatEligible())
        return;
    CCombatantInfo& info = GetCombatantInfo(attacker);
    const CThingCreatureBase* victim = info.AttackerInfo.Attacked;
    if (victim->GetCombatTypeDef()->RotatesCombatRings) {
        CCombatantInfo& victimInfo = GetCombatantInfo(*victim);
        if (victimInfo.PAttackPattern->AttackPattern[victimInfo.AttackPatternIndex] == 0 && info.AttackerInfo.CombatWheelRingIndex == 0)
            victimInfo.CombatWheel.GetRing(0)->DeltaRotation = degreesPerSecond * (1.0f / 360.0f) / g_framesPerSecond;
    }
}