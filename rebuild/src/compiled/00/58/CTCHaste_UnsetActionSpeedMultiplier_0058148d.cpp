// CTCHaste::UnsetActionSpeedMultiplier(void* source) -- drop one source's speed multiplier and recompute the combined multiplier
// Leaf: ?UnsetActionSpeedMultiplier@CTCHaste@@QAEXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0058148D, length 27 bytes (a /Os module: unaligned entry, no
// padding).
// Callees: map<void*, float>::erase(const key_type&) @ 0x0058119B (equal_range, erase the range, return the
// count; the manifest's "CBankFile::CompareDataOffset::operator()" is a propagated name),
// CTCHaste::UpdateActionSpeedMultiplier @ 0x00800020 (landed; Ego_r.pdb ?UpdateActionSpeedMultiplier@CTCHaste@@AAEXXZ,
// private: it resets Multiplier to 1.0f and multiplies in every map value; manifest "CAnimation_RecalculateScale").
// Naming: Ego_r.pdb's CTCHaste name set is Alloc, FrameUpdate, GetActionSpeedMultiplier, GetClassTypeName,
// SetActionSpeedMultiplier(void*, float), SetAsToTerminate, StatRegisterClass, UnsetActionSpeedMultiplier(void*)
// (?UnsetActionSpeedMultiplier@CTCHaste@@QAEXPAX@Z) and UpdateActionSpeedMultiplier; this body takes the source key
// alone, erases it from the multiplier map and recomputes -- the Unset member. The callers are
// CCombatActionBase::ResetMeleeCombatAbilityEffects (0x008596FA / 0x0085974D, on the interface-0x8C component =
// CTCHaste) and CTCHeroStats::FrameUpdate (0x0057D8AD). Was landed as the descriptive "CTCHaste::RemoveMultiplier"
// (FINDINGS chunk 398 noted its callee binding RecalculateMultiplier).
// Identity: the manifest label "CTCActionUseBuyHouse::OnCreate" is propagated and wrong.
//
// Layout = Ego_r.pdb CTCHaste: Multiplier +0x0c, ToTerminate +0x10, PauseFrames +0x14,
// map<void*, float> Multipliers +0x18. Shape: the key is taken by value and erased by reference (its own
// stack slot's address, `lea eax,[esp+8]`); the erase count is dropped; then the recompute on `this`.
#pragma optimize("s", on)

struct CMultiplierMap {                         // retail map<void*, float>
    void* m_head;
    unsigned long erase(void* const& key);      // 0x0058119B
};

class CTCHaste {
public:
    void*          __vftable;                   // +0x00
    void*          ParentThing;                 // +0x04 (CTCBase)
    unsigned char  _pad_0x08[4];
    float          Multiplier;                  // +0x0c
    bool           ToTerminate;                 // +0x10
    long           PauseFrames;                 // +0x14
    CMultiplierMap Multipliers;                 // +0x18
    void UnsetActionSpeedMultiplier(void* source);
private:
    void UpdateActionSpeedMultiplier();         // 0x00800020
};

void CTCHaste::UnsetActionSpeedMultiplier(void* source)
{
    Multipliers.erase(source);
    UpdateActionSpeedMultiplier();
}
