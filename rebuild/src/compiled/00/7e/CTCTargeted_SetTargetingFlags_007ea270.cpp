// CTCTargeted::SetTargetingFlags(unsigned long flags) -- replace the thing's targeting flags word (+0xC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EA270, length 10 bytes, `ret 4`.
// Leaf: ?SetTargetingFlags@CTCTargeted@@QAEXK@Z
// Naming: Ego_r.pdb ?SetTargetingFlags@CTCTargeted@@QAEXK@Z (TU order AddTargetingFlag, RemoveTargetingFlag,
//   SetTargetingFlags, IsTargetable, ...; retail 0x007EA280 next is the IsTargetable shape `TargetingFlags ? Targetable :
//   false`). Ego_r.pdb CTCTargeted: CTCBase (12 bytes) then ulong TargetingFlags +0x0C, bool Targetable +0x10 (as
//   engine/CTCTargeted.h). Callers: the targeting-flag save / restore pair (0x0085F869 GetOldTargetFlagsAndReplaceWith,
//   0x0085F8C5 RevertOldTargetFlags), CGameScriptInterface::EntitySetTargetingType, CThingDeadCreature::OnCreate,
//   CModeStunned / CModeOutOfActionBase. FINDINGS chunk 402. Was landed as the stand-in "SetMember_dword0xc_007ea270"
//   (manifest "CObservable::AddExclusiveObserver" propagated). A local model because engine/CTCTargeted.h is data-only.
class CThing;
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                    // +0x04
    bool    ToBeRemoved;                    // +0x08
    bool    IsInGlobalUpdateTCs;            // +0x09
};

class CTCTargeted : public CTCBase {
public:
    void SetTargetingFlags(unsigned long flags);
    unsigned long TargetingFlags;           // +0x0c
    bool          Targetable;               // +0x10
    bool          UseTargetGlow;            // +0x11
    bool          ValidTargetWithoutLOS;    // +0x12
};

void CTCTargeted::SetTargetingFlags(unsigned long flags)
{
    TargetingFlags = flags;
}
