// CTCTargetingPlayer::UpdateNearestInterestingThing() -- the player's nearest interesting thing is its targeted thing; without one, the base scan runs
// Leaf: ?UpdateNearestInterestingThing@CTCTargetingPlayer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008023E0, length 50 bytes. Vtable 0x0124280C (CTCTargetingPlayer) slot 39
// (catalogued "vfunc_39"; CTCTargetingAI's slot 39 is CTCTargetingBase::UpdateNearestInterestingThing 0x007F5AE0).
// Callees: CTCTargetingBase::SetPNearestInterestingThing(CThing*) @ 0x007F5810 (manifest "CVillageTaskCarryCrate::
// AssignCreatureToTask" is propagated; the body stores into PNearestInterestingThing +0x0d), the virtual
// GetPTargetedThing() (slot 33, +0x84; 0x00802610), CTCTargetingBase::UpdateNearestInterestingThing @ 0x007F5AE0
// (called non-virtually as the tail jump).
// Shape: clear first, then `if (GetPTargetedThing()) Set(GetPTargetedThing()); else Base::Update();` -- the virtual getter
// is called twice (no local), `mov ecx,esi` is hoisted above the `je`, the base call becomes a sibling jump.
struct CThing;

struct CTCTargetingBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTargetedThing();     // slot 33 (+0x84)
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void UpdateNearestInterestingThing();   // slot 39 (+0x9c), base body 0x007F5AE0
    void SetPNearestInterestingThing(CThing* thing);   // 0x007F5810
};

struct CTCTargetingPlayer : CTCTargetingBase {
    virtual void UpdateNearestInterestingThing();
};

void CTCTargetingPlayer::UpdateNearestInterestingThing()
{
    SetPNearestInterestingThing(0);
    if (GetPTargetedThing())
        SetPNearestInterestingThing(GetPTargetedThing());
    else
        CTCTargetingBase::UpdateNearestInterestingThing();
}
