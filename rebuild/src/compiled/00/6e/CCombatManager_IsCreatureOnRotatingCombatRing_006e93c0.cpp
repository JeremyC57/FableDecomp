// CCombatManager::IsCreatureOnRotatingCombatRing(const CThingCreatureBase& creature) const -- whether a combat-eligible attacker stands on a ring of its victim's combat wheel that is currently turning (|DeltaRotation| above the tolerance)
// Leaf: ?IsCreatureOnRotatingCombatRing@CCombatManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E93C0, 139 bytes (144 with int3 padding), thiscall, `ret 4`.
// Identity: Ego_r.pdb name set `?IsCreatureOnRotatingCombatRing@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z`; same
//   shape as the landed CCombatManager::SetAttackerRingAsRotating 0x006E9D90 (eligibility test, the attacker's combatant
//   info, its victim's info, the victim's wheel ring). The catalog's "C3DAnimationBlendState_ProcessCombatRing" is a
//   propagated label.
// Callees: the creature combat-eligibility test @ 0x00662410, the combatant map find(const unsigned __int64&) @ 0x006F0250
//   (twice), CCombatWheel::GetRing(long) const @ 0x006E8520 (landed const overload; no null test after it).
// Globals: the 1e-4 tolerance @ 0x0129BA3C.
// Layout: as the landed 0x006E9D90: CCombatManager Combatants +0x10 (STLport map, node value +0x18); CCombatantInfo
//   AttackerInfo +4 (Attacked +0, CombatWheelRingIndex +8), CombatWheel +0x40; CCombatRing DeltaRotation +4.
// Shape: the victim's UID key is taken (into its frame slot) before the ring index is tested, then GetRing's argument
//   is pushed before the victim's find runs (arguments before the object expression); CCombatRing::IsRotating is the
//   inline `(float)fabs(DeltaRotation) > tolerance` (`fcomp dword ; test ah,41h ; jne`; a double compare loads the
//   tolerance and reverses into `fcompp ; test ah,5`); the find has a same-TU noinline body (else the two key
//   temporaries take separate slots, frame 10h v 8).
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

extern const float g_Epsilon;                               // 0x0129BA3C (1e-4f)

class CThingCreatureBase {
public:
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
    bool IsCombatEligible() const;                          // 0x00662410
};

template <class T> struct CRetailVector {                   // retail std::vector: {first, last, end_of_storage}
    T* first;
    T* last;
    T* end_of_storage;
};

struct CCombatRing {                                        // CDefendingCombatantInfo::CCombatRing, 28 bytes in retail
    float RotationAngle;                                    // +0x00
    float DeltaRotation;                                    // +0x04
    unsigned char _rest[0x14];
    bool IsRotating() const { return (float)fabs(DeltaRotation) > g_Epsilon; }
};

struct CCombatWheel {                                       // CDefendingCombatantInfo::CCombatWheel
    CRetailVector<CCombatRing> CombatRings;                 // +0x00
    const CCombatRing* GetRing(long index) const;           // 0x006E8520
};

struct CAttackingCombatantInfo {
    const CThingCreatureBase* Attacked;                     // +0x00
    long AttackerNumber;                                    // +0x04
    long CombatWheelRingIndex;                              // +0x08
    unsigned char _rest[0x30 - 0x0c];
};

struct CCombatantInfo {                                     // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    const CThingCreatureBase* Parent;                       // +0x00
    CAttackingCombatantInfo AttackerInfo;                   // +0x04
    unsigned char Attackers[0x0c];                          // +0x34
    CCombatWheel CombatWheel;                               // +0x40
};

struct CRetailRbMap_UID_CombatantInfo {                     // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                               // +0x10
        CCombatantInfo value;                               // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const;          // 0x006F0250
};
#pragma auto_inline(off)
// Same-TU body of the STLport map::find instantiation 0x006F0250 (as the landed 0x006E9D90 / 0x006EA770): VC7.1 then
// knows the node result is not derived from the key it was handed, so both key temporaries share one frame slot.
__declspec(noinline) CRetailRbMap_UID_CombatantInfo::node* CRetailRbMap_UID_CombatantInfo::find(const unsigned __int64& key) const
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

class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0xc];                          // +0x04
    CRetailRbMap_UID_CombatantInfo Combatants;              // +0x10
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const
    {
        return Combatants.find(creature.GetUID())->value;
    }
    bool IsCreatureOnRotatingCombatRing(const CThingCreatureBase& creature) const;
};

bool CCombatManager::IsCreatureOnRotatingCombatRing(const CThingCreatureBase& creature) const
{
    if (creature.IsCombatEligible()) {
        const CCombatantInfo& info = GetCombatantInfo(creature);
        unsigned __int64 attackedUID = info.AttackerInfo.Attacked->GetUID();
        long ringIndex = info.AttackerInfo.CombatWheelRingIndex;
        if (ringIndex >= 0 && Combatants.find(attackedUID)->value.CombatWheel.GetRing(ringIndex)->IsRotating())
            return true;
    }
    return false;
}