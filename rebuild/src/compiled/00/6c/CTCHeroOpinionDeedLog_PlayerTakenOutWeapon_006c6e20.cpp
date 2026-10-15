// CTCHeroOpinionDeedLog::PlayerTakenOutWeapon() -- retires any previous weapon-out deed, then (if weapon-out crimes are enabled) posts a fresh one at the hero's position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C6E20, 92 bytes (+4 int3 padding).
// Callees: CTCHeroOpinionDeedLog::RemoveDeedStillSearchingForWitnesses(long) @ 0x006C5920,
//          CTCHeroOpinionDeedLog::RemoveDeed(long) @ 0x006C59E0 (find + erase in the opinion-deed map),
//          CTCHeroOpinionDeedLog::RemoveCrimeFromVillage(long) @ 0x006C5F80,
//          CThing::GetPosition() @ 0x004C73D0 (`&m_pPlace->Position`, manifest label "DoSizeof" is propagated),
//          CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType, const C3DVector&, CThing*, CThingAICreature*,
//              EOpinionPostDeedType, bool, bool, CFaction*, bool, long) @ 0x006C6990.
//
// Shape: the handle re-read from the member before each of the three removal calls (each call may change it),
// the first call reusing ecx = this; -1 stored back; then the WeaponOutCrimeEnabled byte gates PostDeed(1, pos,
// 0, 0, 2, true, true, 0, false, 0) -- the eight constant arguments pushed before the position call, whose
// result is pushed with the deed type -- and the new handle stored.
// Layout: WeaponOutDeedHandle +0x28 / WeaponOutCrimeEnabled +0x2C in retail (PDB +0x34/+0x38: the deed map is
// 12 bytes, not 16, and the witnesses list a 4-byte head, not 12); a local model.

struct C3DVector { float x, y, z; };
struct CThingAICreature;
struct CFaction;

struct CThing {
    const C3DVector& GetPosition();           // 0x004C73D0
};

struct CTCHeroOpinionDeedLog_Retail {
    void*         __vftable;                  // +0x00
    CThing*       ParentThing;                // +0x04 CTCBase
    unsigned char _pad_0x08[0x20];            // +0x08 CTCBase flags, OpinionDeedMap, witnesses list, NextHandle
    long          WeaponOutDeedHandle;        // +0x28 (PDB +0x34)
    bool          WeaponOutCrimeEnabled;      // +0x2C (PDB +0x38)

    void RemoveDeedStillSearchingForWitnesses(long handle);   // 0x006C5920
    void RemoveDeed(long handle);                             // 0x006C59E0
    void RemoveCrimeFromVillage(long handle);                 // 0x006C5F80
    long PostDeed(long type, const C3DVector& position, CThing* target, CThingAICreature* victim,
                  long postType, bool a, bool b, CFaction* faction, bool c, long extra);   // 0x006C6990
    void PlayerTakenOutWeapon();
};

void CTCHeroOpinionDeedLog_Retail::PlayerTakenOutWeapon()
{
    if (WeaponOutDeedHandle != -1) {
        RemoveDeedStillSearchingForWitnesses(WeaponOutDeedHandle);
        RemoveDeed(WeaponOutDeedHandle);
        RemoveCrimeFromVillage(WeaponOutDeedHandle);
        WeaponOutDeedHandle = -1;
    }
    if (WeaponOutCrimeEnabled)
        WeaponOutDeedHandle = PostDeed(1, ParentThing->GetPosition(), 0, 0, 2, true, true, 0, false, 0);
}