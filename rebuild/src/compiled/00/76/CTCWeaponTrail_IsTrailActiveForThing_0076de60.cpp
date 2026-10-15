// CTCWeaponTrail::IsTrailActiveForThing(long, const CThing*, const CThingCreatureBase*) const -- a trail is active for a struck thing given directly, or when the creature's current action reports this component's trail at the same index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076DE60, length 68 bytes (+ int3 padding).
// Callees: CCreatureActionBase::GetWeaponTrailUsedInStrikeAction(long&) const, virtual slot 43 (+0xac).
// Ego_r.pdb: ?IsTrailActiveForThing@CTCWeaponTrail@@ABE_NJPBVCThing@@PBVCThingCreatureBase@@@Z and
// ?GetWeaponTrailUsedInStrikeAction@CCreatureActionBase@@UBEPAVCTCWeaponTrail@@AAJ@Z.
// Layout: the creature's current action (CCountedPointer<CCreatureActionBase> PCurrentAction, object word) sits at
// retail CThingCreatureBase +0x100 (Ego_r.pdb +0x11c).
// Shape: the out-parameter local is homed in the dead `thing` argument slot (`lea edx,[esp+0Ch]`, known null on that
// path); `trail == this` compares the call result with esi, then `index == trailIndex`; shared `mov al,1` /
// `xor al,al` tails with `ret 0Ch`.
struct CThing;
struct CTCWeaponTrail;

struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long& trailIndex) const;   // slot 43 (+0xac)
};

struct CThingCreatureBase {
    unsigned char        _pad_00[0x100];
    CCreatureActionBase* PCurrentAction;                    // +0x100 CCountedPointer<CCreatureActionBase> (object)
};

struct CTCWeaponTrail {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    bool IsTrailActiveForThing(long trailIndex, const CThing* thing, const CThingCreatureBase* creature) const;
};

bool CTCWeaponTrail::IsTrailActiveForThing(long trailIndex, const CThing* thing, const CThingCreatureBase* creature) const
{
    if (thing)
        return true;
    if (creature && creature->PCurrentAction) {
        long index;
        if (creature->PCurrentAction->GetWeaponTrailUsedInStrikeAction(index) == this && index == trailIndex)
            return true;
    }
    return false;
}