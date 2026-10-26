// CAnimSet::Update(const CAnimSetUpdateInfo& info) -- advance the animation set by the driver its UpdateMode selects (time, distance, or a directly supplied percentage), then roll the internal blend alphas
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070D030, 96 bytes of code + a 4-entry jump table (112), __fastcall, ret 4.
// Callees: CAnimSet::UpdateTimePassed(float) 0x0070C330, CAnimSet::UpdateDistanceMoved(float) 0x0070CE10,
// CAnimSet::UpdateInternalAlphas() 0x0070C2C0 (landed).
// Layout: as the landed CAnimSet rows (0x0070C2C0): UpdateMode at +0, retail 12-byte vectors, so
// CurrentPercentageComplete / LastPercentageComplete sit at +0x1C / +0x20 (PDB +0x24 / +0x28).
// Shape: a dense `switch` on UpdateMode (1..4: `dec eax ; cmp eax,3 ; ja` + jump table, the two
// percentage modes share one case) with the UpdateInternalAlphas call tail-duplicated into every arm.
// The update record carries the three per-mode drivers (time at +0, distance at +4, percentage at +8);
// a float argument is pushed through an integer register. The percentage arm is an inline setter fed
// by a float-returning inline getter: the new value travels through x87 (`fld [info+8]` ... `fstp`)
// while `Last = Current` is a plain integer-register copy stored last. Writing the two assignments
// directly copies both through integer registers; a `last` local in the setter puts the OLD value on
// x87 instead. Embedded jump table: grade/land with the jump-table-aware path if the lander objects.
struct CAnimSetUpdateInfo {
    float TimePassed;                                    // +0
    float DistanceMoved;                                 // +4
    float PercentageComplete;                            // +8
    float GetPercentageComplete() const { return PercentageComplete; }
};

template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
};

class CTransitionBlendAlpha { public:
    float Current;
    float Last;
};

enum EAnimUpdateMode {
    ANIM_UPDATE_NONE = 0,
    ANIM_UPDATE_TIME = 1,
    ANIM_UPDATE_DISTANCE = 2,
    ANIM_UPDATE_PERCENTAGE = 3,
    ANIM_UPDATE_PERCENTAGE_2 = 4
};

class CAnimSet { public:
    EAnimUpdateMode UpdateMode;                          // +0
    CRetailVector<short> Anims;                          // +4
    CRetailVector<CTransitionBlendAlpha> InternalAlphas; // +0x10
    float CurrentPercentageComplete;                     // +0x1c
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    void UpdateTimePassed(float time);                   // 0x0070C330
    void UpdateDistanceMoved(float distance);            // 0x0070CE10
    void UpdateInternalAlphas();                         // 0x0070C2C0
    void Update(const CAnimSetUpdateInfo& info);
    void SetPercentageComplete(float percentage)
    {
        LastPercentageComplete = CurrentPercentageComplete;
        CurrentPercentageComplete = percentage;
    }
};

void CAnimSet::Update(const CAnimSetUpdateInfo& info)
{
    switch (UpdateMode) {
    case ANIM_UPDATE_TIME:
        UpdateTimePassed(info.TimePassed);
        break;
    case ANIM_UPDATE_DISTANCE:
        UpdateDistanceMoved(info.DistanceMoved);
        break;
    case ANIM_UPDATE_PERCENTAGE:
    case ANIM_UPDATE_PERCENTAGE_2:
        SetPercentageComplete(info.GetPercentageComplete());
        break;
    }
    UpdateInternalAlphas();
}