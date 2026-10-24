// IsInMeleeWithCombatWheelAttackers(const CThingCreatureBase& creature) -- true when the creature is attacking in melee, or is being attacked and at least one of its attackers is using the combat wheel
// Leaf: ?IsInMeleeWithCombatWheelAttackers@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00979910, 119 bytes, plain `ret`. Anonymous-namespace __fastcall (Ego_r.pdb
//   ?IsInMeleeWithCombatWheelAttackers@?A0xeaff0e08@@YI_NABVCThingCreatureBase@@@Z, the CAIStateGroup_CreatureTakeCover
//   TU; callers 0x00979D80 IsRunnableLeaf and 0x00979E90 IsStillRunnable): creature in ecx.
// Callees: CThingCreatureBase::IsInMeleeAsAttacker @ 0x00662410 (the combat manager's attacker test: the combatant's
//   AttackerInfo.Attacked != 0) / IsInMeleeAsAttacked @ 0x00662430 (its DefenderInfo.Attackers count != 0),
//   CThingCreatureBase::GetCombatManager const @ 0x00661EC0, CCombatManager::GetCombatantInfo const @ 0x006E9360, the
//   attacker map iterator's operator++ @ 0x006EF790 (out of line, on the iterator's frame word).
// Shape: an early `return true` for the attacker case (its own `mov al,1` epilogue before `push edi`); then the
//   creature's combatant info is a named reference held in edi, the attacker map is walked with end() re-read every
//   pass, and each attacker's combatant info is fetched afresh (GetCombatManager again, its argument pushed first);
//   `UsingCombatWheel` (+0x20) set returns true.
// Layout: the retail CCombatantInfo of the landed rows (0x006E97F0, 0x008E7040): Parent +0, AttackerInfo +4
//   (UsingCombatWheel at +0x20), DefenderInfo.Attackers (STLport map<unsigned __int64, CThingCreatureBase*>, the header
//   node pointer) at +0x34; map node value at +0x10, the attacker pointer at +0x18. Ego_r.pdb's debug layout is 4 bytes
//   longer (a 16-byte CombatSequences vector).
// Label: catalog name confirmed; 0x00662410 / 0x00662430 are IsInMeleeAsAttacker / IsInMeleeAsAttacked (the manifest's
//   "C3DAnimationBlendState::AddCombinationBlend" is propagated; landed sources call them IsRegisteredCombatant /
//   IsCombatant).
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
class CThingCreatureBase;
struct CAttackerNode {                          // STLport node of map<unsigned __int64, CThingCreatureBase*>
    long _M_color;                              // +0x00
    CAttackerNode* _M_parent;                   // +0x04
    CAttackerNode* _M_left;                     // +0x08
    CAttackerNode* _M_right;                    // +0x0c
    unsigned __int64 first;                     // +0x10 the attacker's unique id
    CThingCreatureBase* second;                 // +0x18 the attacker
};
struct CAttackerIterator {
    CAttackerNode* _M_node;
    CAttackerIterator(CAttackerNode* node) : _M_node(node) { }
    bool operator!=(const CAttackerIterator& other) const { return _M_node != other._M_node; }
    CAttackerIterator& operator++();            // 0x006EF790
    CAttackerNode* operator->() const { return _M_node; }
};
struct CAttackerMap {
    CAttackerNode* _M_header;                   // +0x00 header node (left = leftmost)
    unsigned long _M_node_count;                // +0x04
    CAttackerIterator begin() const { return CAttackerIterator(_M_header->_M_left); }
    CAttackerIterator end() const { return CAttackerIterator(_M_header); }
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                 // +0x00
    CThingCreatureBase* Attacked;               // +0x04 AttackerInfo.Attacked
    long AttackerNumber;                        // +0x08
    long CombatWheelRingIndex;                  // +0x0c
    bool PreferredAttackDollySet;               // +0x10
    long PreferredAttackDollyTimer;             // +0x14
    float PreferredAttackDollyVector[2];        // +0x18 C2DVector
    bool UsingCombatWheel;                      // +0x20
    long CurrentSequence;                       // +0x24
    void* CombatSequences[3];                   // +0x28 STLport vector<CCountedPointer<CCombatSequenceBase> >
    CAttackerMap Attackers;                     // +0x34 DefenderInfo.Attackers
};
struct CCombatManager {
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const;   // 0x006E9360
};
class CThingCreatureBase {
public:
    bool IsInMeleeAsAttacker() const;           // 0x00662410
    bool IsInMeleeAsAttacked() const;           // 0x00662430
    const CCombatManager& GetCombatManager() const;   // 0x00661EC0
};

namespace {
bool __fastcall IsInMeleeWithCombatWheelAttackers(const CThingCreatureBase& creature);
}
// Keeps the internal function emitted (VC7.1 discards an unreferenced one).
bool __fastcall IsInMeleeWithCombatWheelAttackers_Use(const CThingCreatureBase& creature)
{
    return IsInMeleeWithCombatWheelAttackers(creature);
}
namespace {
bool __fastcall IsInMeleeWithCombatWheelAttackers(const CThingCreatureBase& creature)
{
    if (creature.IsInMeleeAsAttacker())
        return true;
    if (creature.IsInMeleeAsAttacked()) {
        const CCombatantInfo& info = creature.GetCombatManager().GetCombatantInfo(creature);
        for (CAttackerIterator it = info.Attackers.begin(); it != info.Attackers.end(); ++it) {
            if (creature.GetCombatManager().GetCombatantInfo(*it->second).UsingCombatWheel)
                return true;
        }
    }
    return false;
}
}