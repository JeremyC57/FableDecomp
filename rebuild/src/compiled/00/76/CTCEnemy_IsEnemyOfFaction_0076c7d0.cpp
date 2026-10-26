// CTCEnemy::IsEnemyOfFaction(const CFaction* faction) const -- false for a null faction or when this TC has no faction; otherwise its faction's CFaction::IsEnemyOfFaction(*faction) (a tail jump)
// Leaf: ?IsEnemyOfFaction@CTCEnemy@@QBE_NPBVCFaction@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076C7D0, 29 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: the tail jump goes to 0x006B94C0 = CFaction::IsEnemyOfFaction (EnemyFactions at +0x10; RENAMES 0x006B94C0),
//   with `this` = PFaction (+0x18); Ego_r.pdb name set ?IsEnemyOfFaction@CTCEnemy@@QBE_NPBVCFaction@@@Z (CTCEnemy's
//   IsAlliedWithFaction is the twin at 0x0076C7F0 that tail-jumps to CFaction::IsAlliedWithFaction 0x006B9460, as the
//   landed 0x006B9460 notes). Callers: CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed 0x006C6552,
//   CThingCreatureBase 0x006C8CE2 (tail jump), CThingFilter_IsEligibleWitness::operator() 0x006C8F60.
//   Was landed as CTCEnemy::IsAlliedWithFaction.
// Layout: Ego_r CTCEnemy: CTCBase (vptr, +4, +8), FriendsWithEverythingFlag +0x0C, EnableFollowersEnemyProxy +0x0D,
//   EnemyThings / AllyThings (STLport lists, 4 bytes each: +0x10 / +0x14), PFaction +0x18.
// Shape: the argument is re-stored into its own stack slot before the `jmp` (VC7.1 tail call of a thiscall with the same
//   argument), both null tests branch to one `xor al,al ; ret 4`.
class CFaction {
public:
    bool IsEnemyOfFaction(const CFaction& faction) const;      // 0x006B94C0
};

class CTCEnemy {
public:
    virtual ~CTCEnemy();
    unsigned char _base04[8];                           // CTCBase +0x04..+0x0B
    bool FriendsWithEverythingFlag;                     // +0x0C
    bool EnableFollowersEnemyProxy;                     // +0x0D
    void* EnemyThings;                                  // +0x10 list<CIntelligentPointer<const CThing> > (STLport, 4 bytes)
    void* AllyThings;                                   // +0x14
    CFaction* PFaction;                                 // +0x18
    bool IsEnemyOfFaction(const CFaction* faction) const;
};

bool CTCEnemy::IsEnemyOfFaction(const CFaction* faction) const
{
    if (faction && PFaction)
        return PFaction->IsEnemyOfFaction(*faction);
    return false;
}
