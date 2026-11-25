// Leaf: ?GetTotalTimeSeconds@CCombatActionBase@@UBEMXZ
// CCombatActionBase::GetTotalTimeSeconds() const -- total time of the animation entry's animation from the creature's animation bank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00856DD0, length 31 bytes.
// Callees: CCreatureActionBase::GetPCreatureAnimationOwner @ 0x00692950 (`add ecx,8 ; call GetPItem ; jmp
// 0x004C7AB0`: the creature's animation-owning object; the manifest's "_Dest_val<...>" is a propagated label),
// CMeshDataBank::PeekAnimation(long) @ 0x00A242C0 (`[this+380h][index]`; the manifest's "GetAbilityLevel" is
// propagated), CAnimation::GetTotalTimeSeconds @ 0x00A23CE0 (`fld [this+14h]`; "DrawGetSeaLevel" is propagated).
// The index is the first long of the entry PAnimEntry (+0x74, Ego_r.pdb name) points at; it is read into esi
// BEFORE the owner call and pushed only after it: named locals (index, bank); the float result goes through a
// named local too, which keeps the last getter a `call ; pop esi ; ret` instead of a tail `jmp`.
struct CAnimation {
    float GetTotalTimeSeconds() const;          // 0x00A23CE0
};

class C3DAnimationInfo;
class CMeshDataBank { public:
    C3DAnimationInfo const* PeekAnimation(unsigned long a0) const;   // 0x00A242C0 (Ego_r.pdb ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z)
};
struct CAnimationBank {
};

struct CAnimationOwner {
    unsigned char _pad[0x3c];
    CAnimationBank* m_pBank;                    // +0x3c
};

class CAnimationEntry { public:
    long AnimationIndex;                        // +0x00
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[0x74 - 4];             // +0x04
    CAnimationEntry* PAnimEntry;                // +0x74
    unsigned char _pad78[0x114 - 0x78];         // +0x78
    virtual float GetTotalTimeSeconds() const;
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00692950 ?PeekWorld@CCreatureActionBase@@IBEABVCWorld@@XZ (was ?GetPCreatureAnimationOwner@CCombatActionBase@@QBEPAUCAnimationOwner@@XZ)
class CWorld;
class CCombatActionBase;
class CCreatureActionBase { public: protected: CWorld const& PeekWorld() const; public: friend class ::CCombatActionBase; };
float CCombatActionBase::GetTotalTimeSeconds() const
{
    long index = PAnimEntry->AnimationIndex;
    CAnimationBank* bank = ((CAnimationOwner*)(&reinterpret_cast<const CCreatureActionBase*>(this)->PeekWorld()))->m_pBank;
    float totalTime = ((CAnimation*)((CMeshDataBank*)bank)->PeekAnimation(index))->GetTotalTimeSeconds();   // a named float local blocks the tail jmp
    return totalTime;
}