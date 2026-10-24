// CDefendingCombatantInfo::CCombatWheel::SetAttackerProngAsBlocked(const CThingCreatureBase& attacker, CAttackingCombatantInfo& attackerInfo) -- block the prong the attacker holds on its wheel ring (when the ring index is valid and the attacker owns a prong there), then detach the attacker from the wheel (ring index -1)
// Leaf: ?SetAttackerProngAsBlocked@CCombatWheel@CDefendingCombatantInfo@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9200, 101 bytes (112 with int3 padding), thiscall, `ret 8`.
// Identity: Ego_r.pdb name set `?SetAttackerProngAsBlocked@CCombatWheel@CDefendingCombatantInfo@@QAEXABVCThingCreatureBase@@
//   AAVCAttackingCombatantInfo@@@Z`; the landed CCombatManager::GetPreferredPos 0x006EA770 calls this address as
//   "CCombatWheel::BlockAttackerProng" with the creature and its AttackerInfo. The catalog's
//   "CDefendingCombatantInfo_BlockAttackerProng" is descriptive.
// Callees: CCombatRing::GetAttackerProng(const CThingCreatureBase&) @ 0x006E8790, CProng::SetBlocked(bool) @ 0x006E9140
//   (both landed). CCombatWheel::GetRing(long) (landed out of line @ 0x006E84E0) is inlined here.
// Layout (retail 12-byte STLport vectors, as the landed GetRing / AddRing): CCombatWheel = {vector<CCombatRing>
//   CombatRings}; CCombatRing 28 bytes; CAttackingCombatantInfo (Ego_r.pdb) CombatWheelRingIndex +8.
// Shape: the inline GetRing (`index >= 0` first, then the unsigned compare against size(), `&first[index]` null-tested);
//   VC duplicates the trailing `CombatWheelRingIndex = -1` into the negative-index exit (no edi push there).
class CThingCreatureBase;

class CAttackingCombatantInfo {
public:
    CThingCreatureBase* Attacked;                           // +0x00
    long                AttackerNumber;                     // +0x04
    long                CombatWheelRingIndex;               // +0x08
};

template <class T> struct CRetailVector {                   // retail std::vector: {first, last, end_of_storage}
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};

class CDefendingCombatantInfo {
public:
    class CProng {                                          // 28 bytes
    public:
        bool          Blocked;                              // +0x00
        float         Prong[3];                             // +0x04 C3DVector
        long          ProngBlockageTimer;                   // +0x10
        unsigned char POccupier[8];                         // +0x14 CIntelligentPointer<CThingCreatureBase>
        void SetBlocked(bool blocked);                      // 0x006E9140
    };
    class CCombatRing {                                     // 28 bytes in retail (PDB 32)
    public:
        float                 RotationAngle;                // +0x00
        float                 DeltaRotation;                // +0x04
        long                  Index;                        // +0x08
        float                 RingThickness;                // +0x0C
        CRetailVector<CProng> Prongs;                       // +0x10
        CProng* GetAttackerProng(const CThingCreatureBase& attacker);   // 0x006E8790
    };
    class CCombatWheel {
    public:
        CRetailVector<CCombatRing> CombatRings;             // +0x00
        CCombatRing* GetRing(long index)                    // 0x006E84E0 (inlined here)
        {
            if (index >= 0 && (unsigned long)index < CombatRings.size())
                return &CombatRings.first[index];
            return 0;
        }
        void SetAttackerProngAsBlocked(const CThingCreatureBase& attacker, CAttackingCombatantInfo& attackerInfo);
    };
};

void CDefendingCombatantInfo::CCombatWheel::SetAttackerProngAsBlocked(const CThingCreatureBase& attacker,
                                                                      CAttackingCombatantInfo& attackerInfo)
{
    CCombatRing* ring = GetRing(attackerInfo.CombatWheelRingIndex);
    if (ring) {
        CProng* prong = ring->GetAttackerProng(attacker);
        if (prong)
            prong->SetBlocked(true);
    }
    attackerInfo.CombatWheelRingIndex = -1;
}