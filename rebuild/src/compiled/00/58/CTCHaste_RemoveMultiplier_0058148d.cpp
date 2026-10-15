// CTCHaste::RemoveMultiplier(void*) -- drop one source's speed multiplier and recompute the combined multiplier
// Leaf: RemoveMultiplier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0058148D, length 27 bytes (a /Os module: unaligned entry, no
// padding).
// Callees: map<void*, float>::erase(const key_type&) @ 0x0058119B (equal_range, erase the range, return the
// count; the manifest's "CBankFile::CompareDataOffset::operator()" is a propagated name),
// CTCHaste::RecalculateMultiplier @ 0x00800020 (in the CTCHaste TU next to GetActionSpeedMultiplier: it resets
// Multiplier to 1.0f and multiplies in every map value; manifest "CAnimation_RecalculateScale").
// Identity: the manifest label "CTCActionUseBuyHouse::OnCreate" is propagated and wrong; the callers are
// CCombatActionBase::ResetMeleeCombatAbilityEffects (0x008596FA / 0x0085974D, on the interface-0x8C
// component = CTCHaste) and CTCHeroStats::FrameUpdate (0x0057D8AD). The method name is descriptive.
//
// Layout = Ego_r.pdb CTCHaste: Multiplier +0x0c, ToTerminate +0x10, PauseFrames +0x14,
// map<void*, float> Multipliers +0x18. Shape: the key is taken by value and erased by reference (its own
// stack slot's address, `lea eax,[esp+8]`); the erase count is dropped; then the recompute on `this`.
#pragma optimize("s", on)

struct CMultiplierMap {                         // retail map<void*, float>
    void* m_head;
    unsigned long erase(void* const& key);      // 0x0058119B
};

struct CTCHaste {
    void*          __vftable;                   // +0x00
    void*          ParentThing;                 // +0x04 (CTCBase)
    unsigned char  _pad_0x08[4];
    float          Multiplier;                  // +0x0c
    bool           ToTerminate;                 // +0x10
    long           PauseFrames;                 // +0x14
    CMultiplierMap Multipliers;                 // +0x18
    void RecalculateMultiplier();               // 0x00800020
    void RemoveMultiplier(void* source);
};

void CTCHaste::RemoveMultiplier(void* source)
{
    Multipliers.erase(source);
    RecalculateMultiplier();
}