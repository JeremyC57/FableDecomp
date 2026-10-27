// CAIStateGroup_MinionGuardFollowPlayer::IsStillValid() const -- the full validity check (IsStillValidInternal(true))
// Leaf: ?IsStillValid@CAIStateGroup_MinionGuardFollowPlayer@@UBE_NXZ
// Retail: Fable.exe @ 0x0090C190, 8 bytes, thiscall plain `ret`, bool in al.
// Identity (proven): vtable slot 5 (+0x14) of CAIStateGroup_MinionGuardFollowPlayer 0x01288DCC, the slot of the landed
// CAIStateGroupBase::IsStillValid (0x008FD276) in the base vtable 0x01288C60; Ego_r.pdb has
// ?IsStillValid@CAIStateGroup_MinionGuardFollowPlayer@@UBE_NXZ and the protected virtual
// ?IsStillValidInternal@CAIStateGroup_MinionGuardFollowPlayer@@MBE_N_N@Z, which is this class's first own slot, 14 (+0x38,
// 0x0090C2D0, landed under that name).
// Callees: virtual slot 14 (+0x38) IsStillValidInternal(bool).
// Layout: engine/CAIStateGroupBase.h carries the vptr as a data word, so the virtual call cannot go through it: local
// polymorphic model with the base vtable's slot names (0..13, read from Fable.exe and the catalog). Header debt (NOTES.md).
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();                           // slot 0
    virtual void Initialise();                              // slot 1 (real signature not needed here)
    virtual void OnActivate();                              // slot 2
    virtual void InitCreaturePosition();                    // slot 3
    virtual bool AllowPriorityOverlap() const;              // slot 4
    virtual bool IsStillValid() const;                      // slot 5 (+0x14)
    virtual void GetDebugText();                            // slot 6
    virtual bool IsTimingCritical() const;                  // slot 7
    virtual long GetStateGroupType() const;                 // slot 8
    virtual void Cleanup();                                 // slot 9
    virtual void OnReplace();                               // slot 10
protected:
    virtual bool IsRunnable();                              // slot 11
public:
    virtual void Update() = 0;                              // slot 12 (__purecall in the base)
    virtual void InitStateGroup();                          // slot 13
};

class CAIStateGroup_MinionGuardFollowPlayer : public CAIStateGroupBase {
public:
    virtual bool IsStillValid() const;
protected:
    virtual bool IsStillValidInternal(bool full) const;     // slot 14 (+0x38)
};

bool CAIStateGroup_MinionGuardFollowPlayer::IsStillValid() const
{
    return IsStillValidInternal(true);
}