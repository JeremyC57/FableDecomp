// CAIStateGroup_MazeRangedCombat::IsStillRunnable() -- keep the maze ranged-combat state while the target is alive, the group is not stopping and the target is between 8 and 50 units away in XY
// Grade: DIFFER(140v142) -- register allocation only: retail keeps this in edi and &PTarget in esi, reuses edi for the creature after IsStopping and holds the target in ebx (`push ebx` shrink-wrapped around the GetPosition pair, 2 bytes more); ours keeps this in esi and &PTarget in edi and never uses ebx (tried: base [kept], self/target declaration order swapped, GetPosition calls swapped, the IS_THING_ALIVE helper spelt out as a direct condition)
// Leaf: ?IsStillRunnable@CAIStateGroup_MazeRangedCombat
// Retail: Fable.exe (TLC, VC7.1) @ 0x00954220, 142 bytes (+ int3 padding to 144), plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, PTarget at +0x14), CAIStateGroupBase::IsStopping
//   @ 0x008FD010, CThing::GetPosition @ 0x004C73D0 (creature first, then the target); 50.0f @ 0x012944BC, 8.0f
//   @ 0x012944C4.
// Shape: IS_THING_ALIVE(PTarget) (GetPItem null test, then the dead bit +0x91) && !IsStopping(), then the XY distance
//   `sqrt(dx*dx + dy*dy)` (fsqrt) tested `< 50 && > 8`.
// Label: catalog "AIState_CheckCanReach" -> Ego_r.pdb ?IsStillRunnable@CAIStateGroup_MazeRangedCombat@@IAE_NXZ.
#include <math.h>

struct C3DVector { float X, Y, Z; };

struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    bool IsStopping();                          // 0x008FD010
};

class CAIStateGroup_MazeRangedCombat : public CAIStateGroupBase {
public:
    CBaseIntelligentPointer PTarget;            // +0x14
    unsigned char _pad1c[4];                    // +0x1c a base-class word Ego_r.pdb does not name
    long LastCompletedPhase;                    // +0x20
    bool IsStillRunnable();
};

bool CAIStateGroup_MazeRangedCombat::IsStillRunnable()
{
    if (IS_THING_ALIVE(PTarget) && !IsStopping()) {
        const CThing* self = PBrain->PCreature;
        const CThing* target = PTarget.GetPItem();
        const C3DVector& me = self->GetPosition();
        const C3DVector& him = target->GetPosition();
        float dx = him.X - me.X;
        float dy = him.Y - me.Y;
        float dist = (float)sqrt(dx * dx + dy * dy);
        if (dist < 50.0f && dist > 8.0f)
            return true;
    }
    return false;
}
