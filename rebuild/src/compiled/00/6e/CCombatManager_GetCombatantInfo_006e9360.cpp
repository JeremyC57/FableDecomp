// CCombatManager::GetCombatantInfo(const CThingCreatureBase& creature) const -- the creature's combatant record: copy its
//   64-bit UniqueID (+0x18/+0x1c) into a local key, find it in the Combatants map at +0x10 and return the found node's
//   mapped CCombatantInfo (+0x18); the caller relies on the creature being registered.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9360, 41 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb names exactly two overloads, in this order: ?GetCombatantInfo@CCombatManager@@QBEABUCCombatantInfo@@
//   ABVCThingCreatureBase@@@Z (public const) then ?GetCombatantInfo@CCombatManager@@AAEAAUCCombatantInfo@@ABVCThingCreature
//   Base@@@Z (private); retail has two byte-identical bodies in the same order, 0x006E9360 then 0x006E9390. 0x006E9360 is
//   called from outside the class (CThingCreatureBase::GetShotAccuracyPercentage, CombatDialogueManager::GetBestSpeechForCreature,
//   the anonymous IsInMeleeWithCombatWheelAttackers(const CThingCreatureBase&), CMeleeCombatAbilityDef::IsAbilityValid, ...),
//   so it is the public const overload; 0x006E9390 (no direct callers) is the private one. Ego_r.pdb CCombatManager::
//   Combatants is map<unsigned __int64, CCombatantInfo> keyed by CThing::UniqueID (+0x18). The manifest's
//   CSurface::GetHeight and the catalog's CUIDMapOwner_* names are propagated / placeholders. FINDINGS chunks 407 + 408.
// Callees: the Combatants tree's node find @ 0x006F0250 (thiscall on the tree, key by reference, node pointer in eax).
// Shape: an 8-byte frame for the key copy (low word stored before the push, high word after `add ecx,10h`),
//   `lea edx,[esp] ; push edx`, then `add eax,18h`, `ret 4`.
// Leaf: ?GetCombatantInfo@CCombatManager@@QBEABUCCombatantInfo@@ABVCThingCreatureBase@@@Z
struct CCombatantInfo {                           // the mapped value (Ego_r.pdb struct CCombatantInfo; opaque here)
    long FirstWord;                                // +0x00
};

struct SCombatantNode {                            // STLport _Rb_tree_node<pair<const unsigned __int64, CCombatantInfo> >
    long             _M_color;                     // +0x00
    SCombatantNode*  _M_parent;                    // +0x04
    SCombatantNode*  _M_left;                      // +0x08
    SCombatantNode*  _M_right;                     // +0x0c
    unsigned __int64 first;                        // +0x10 the creature's UniqueID
    CCombatantInfo   second;                       // +0x18
};

struct SCombatantMap {                             // map<unsigned __int64, CCombatantInfo> (STLport _Rb_tree)
    SCombatantNode* _M_header;                     // +0x00
    SCombatantNode* FindNode(const unsigned __int64& key) const;   // 0x006F0250 (the tree's node find; stand-in name)
};

class CThing {
public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                     // +0x18 (Ego_r.pdb CThing::UniqueID)
};
class CThingCreatureBase : public CThing { };

class CCombatManager {
public:
    unsigned char  _pad00[0x10];                   // +0x00 vptr, +0x04 RegisteredCombatSequences (STLport map, 12 bytes)
    SCombatantMap  Combatants;                     // +0x10 map<unsigned __int64, CCombatantInfo>
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const;   // 0x006E9360
private:
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature);               // 0x006E9390
};

const CCombatantInfo& CCombatManager::GetCombatantInfo(const CThingCreatureBase& creature) const
{
    unsigned __int64 key = creature.UniqueID;
    return Combatants.FindNode(key)->second;
}
