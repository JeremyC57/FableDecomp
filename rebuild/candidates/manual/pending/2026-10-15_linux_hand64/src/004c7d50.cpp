// CThing::OnDie() -- forbids adding thing components, tells every component of the thing it has died (for_each over the component map, each TC's virtual OnDie), then permits adding components again
// Leaf: ?OnDie@CThing@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7D50, 50 bytes. CThing vtable 0x01239ABC slot 11 (rtti-vtables.tsv).
// Callees: for_each<pair<ETCInterfaceType, CTCBase*>*, CCallOnDie> @ 0x004CBA30 (SGI algorithm built `/Gr`:
// first in ecx, last in edx, the 1-byte functor by value on the stack, the functor returned in al; its loop calls
// CTCBase virtual slot 13, which is OnDie in the TC vtables, e.g. CTCMapwho / CTCShopKeeper).
// Label: the chunk label "CParticleSystem::Kill" is a BSim guess (bsim_port_audit 0.61); the vtable slot and the
// body make it CThing's own OnDie (slot 12 at 0x004C7CF0 is CThing::OnKill with the same guard and the OnKill
// functor 0x004CB9F0).
// Shape: the retail CThing is 16 bytes narrower than the PDB before +0x93 (CVectorMap 16 bytes, STLport list
// 4 bytes, bitset 36): Components first/last at +0x44/+0x48 (PDB +0x48), the flag byte +0x93 (PDB +0xa3) with
// AddingTCsPermitted at bit 6 (`and dl,0BFh` / `or byte ptr [..],40h`). The functor is VALUE-initialised
// (`CCallOnDie()` with no user constructor), which is the `mov byte ptr [esp+4],0` store into the `push ecx`
// slot before the dword push.
struct CTCBase;

struct SComponentPair {                              // pair<ETCInterfaceType, CTCBase*>, 8 bytes
    long     first;
    CTCBase* second;
};

struct CCallOnDie {                                  // empty functor: p.second->OnDie()
    void operator()(SComponentPair& p) const;
};

CCallOnDie __fastcall for_each(SComponentPair* first, SComponentPair* last, CCallOnDie f);   // 0x004CBA30

struct CThing {
    unsigned char   m_pad00[0x44];
    SComponentPair* m_componentsFirst;               // +0x44  Components (CVectorMap)
    SComponentPair* m_componentsLast;                // +0x48
    SComponentPair* m_componentsEnd;                 // +0x4c
    unsigned char   m_pad50[0x43];
    unsigned char   NotPausedDuringCutscenes : 1;    // +0x93 bit 0
    unsigned char   DeathPersistent : 1;
    unsigned char   KillOnLevelUnload : 1;
    unsigned char   ActivatedScript : 1;
    unsigned char   LevelIsUnloading : 1;
    unsigned char   IsAffectedByBulletTimeOverride : 1;
    unsigned char   AddingTCsPermitted : 1;          // bit 6
    void OnDie();
};

void CThing::OnDie()
{
    AddingTCsPermitted = false;
    for_each(m_componentsFirst, m_componentsLast, CCallOnDie());
    AddingTCsPermitted = true;
}
