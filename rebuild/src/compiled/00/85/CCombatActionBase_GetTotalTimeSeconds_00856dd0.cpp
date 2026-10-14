// CCombatActionBase::GetTotalTimeSeconds() const -- total time of the animation entry's animation from the creature's animation bank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00856DD0, length 31 bytes.
// Callees: CCreatureActionBase::GetPCreatureAnimationOwner @ 0x00692950 (`add ecx,8 ; call GetPItem ; jmp
// 0x004C7AB0`: the creature's animation-owning object; the manifest's "_Dest_val<...>" is a propagated label),
// CAnimationBank::GetPAnimation(long) @ 0x00A242C0 (`[this+380h][index]`; the manifest's "GetAbilityLevel" is
// propagated), CAnimation::GetTotalTimeSeconds @ 0x00A23CE0 (`fld [this+14h]`; "DrawGetSeaLevel" is propagated).
// The index is the first long of the entry PAnimEntry (+0x74, Ego_r.pdb name) points at; it is read into esi
// BEFORE the owner call and pushed only after it: named locals (index, bank); the float result goes through a
// named local too, which keeps the last getter a `call ; pop esi ; ret` instead of a tail `jmp`.
struct CAnimation {
    float GetTotalTimeSeconds() const;          // 0x00A23CE0
};

struct CAnimationBank {
    CAnimation* GetPAnimation(long index);      // 0x00A242C0
};

struct CAnimationOwner {
    unsigned char _pad[0x3c];
    CAnimationBank* m_pBank;                    // +0x3c
};

struct CAnimationEntry {
    long AnimationIndex;                        // +0x00
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[0x74 - 4];             // +0x04
    CAnimationEntry* PAnimEntry;                // +0x74
    unsigned char _pad78[0x114 - 0x78];         // +0x78
    CAnimationOwner* GetPCreatureAnimationOwner() const;   // 0x00692950 (CCreatureActionBase)
    float GetTotalTimeSeconds() const;
};

float CCombatActionBase::GetTotalTimeSeconds() const
{
    long index = PAnimEntry->AnimationIndex;
    CAnimationBank* bank = GetPCreatureAnimationOwner()->m_pBank;
    float totalTime = bank->GetPAnimation(index)->GetTotalTimeSeconds();   // a named float local blocks the tail jmp
    return totalTime;
}