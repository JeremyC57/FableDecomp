// CAIBrain::IsReadyToRunStateGroupWithHigherScriptPriorityThan(long priority, long zone) -- selects the update zone; on this brain's update frame finds the first enabled, runnable state group with a higher script priority and, if none is active, activates it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088BAC0, length 155 bytes.
// Leaf: IsReadyToRunStateGroupWithHigherScriptPriorityThan
// Callees: CAIBrain::GetUpdateZone @ 0x0088B910 (propagated label CEngineWorldMap::BuildAndSaveSea),
// CMainGameComponent::GetFrameNumber @ 0x004167C8, __aullrem @ 0x00BFEBB0, CAIStateGroupBase::IsEnabledAndRunnable
// @ 0x008FCF10, CAIStateGroupBase::Activate @ 0x008FCF50, the new group's virtual slot 2 (+0x08).
// Shape: the zone's period defaults to 1; the frame test is the landed IsUpdateZoneActiveThisFrame (0x0088B810)
// arithmetic (`(offset + frame) % period`, 64-bit unsigned, the creature offset read into a local before the call);
// the list walk re-reads its end every iteration; the activated group is called through the member re-read.
// Layout: the retail CAIBrain is the PDB layout (engine/CAIBrain.h) shifted down by one dword from +0x28 on (the
// retail CVectorMap is 0x10 bytes), as the landed 0x0088B810 / 0x0088B870 found -- modelled locally because the
// header disagrees: Component +0x14, Creature +0x18, StateGroupList +0x28 (8-byte {key, group} pairs),
// PActiveStateGroup +0x38, PLastStateGroup +0x3c, PPendingStateGroup +0x40, Output +0x44 (CBrainOutput, 0x48),
// OutputValid +0x8c, UpdatingStateGroup +0x8d, PCurrentUpdateZone +0xa0, OnKillCalled +0xa4.
// CAIStateGroupBase (PDB): ScriptAIPriority +8, Terminated +0x10, Priority +0x11, Enabled +0x12.
class CBrainOutput { public:                                    // 0x48 bytes
    unsigned char _body[0x48];
    CBrainOutput& operator=(const CBrainOutput& other);  // 0x0088C840
};
class CAIStateGroupBase { public:
    virtual void v00(); virtual void v01();
    virtual void OnStart();                              // slot 2 (+0x08)
    void*         PBrain;                                // +0x04
    long          ScriptAIPriority;                      // +0x08
    void*         Name;                                  // +0x0c CCharString
    bool          Terminated;                            // +0x10
    unsigned char Priority;                              // +0x11
    bool          Enabled;                               // +0x12
    void Terminate();                                    // 0x008FCF40 (Terminated = true)
    void CleanupMain();                                  // 0x008FD050
    bool IsEnabledAndRunnable();                         // 0x008FCF10
    void Activate();                                     // 0x008FCF50
};
struct CStateGroupEntry {
    long               Key;                              // +0x00
    CAIStateGroupBase* Group;                            // +0x04
};
class CBrainUpdateZone { public:
    unsigned char _pad[8];
    unsigned long UpdatePeriod;                          // +0x08 frames between updates
};
class CMainGameComponent { public:
    unsigned long GetFrameNumber();                      // 0x004167C8
};
struct CThingAICreatureCalls {
    unsigned char    _pad[0x18];
    unsigned __int64 m_updateOffset;                     // +0x18 per-creature frame offset (64-bit)
};

class CAIBrain { public:
    virtual void v00();
    virtual void UpdateStateGroup();                     // slot 1 (+0x04)
    unsigned char           _pad04[0x10];
    CMainGameComponent*     Component;                   // +0x14
    CThingAICreatureCalls*  Creature;                    // +0x18
    unsigned char           _pad1c[0x0c];
    CStateGroupEntry*       StateGroupBegin;             // +0x28 StateGroupList (retail CVectorMap)
    CStateGroupEntry*       StateGroupEnd;               // +0x2c
    unsigned char           _pad30[0x08];
    CAIStateGroupBase*      PActiveStateGroup;           // +0x38
    CAIStateGroupBase*      PLastStateGroup;             // +0x3c
    CAIStateGroupBase*      PPendingStateGroup;          // +0x40
    CBrainOutput            Output;                      // +0x44
    bool                    OutputValid;                 // +0x8c
    bool                    UpdatingStateGroup;          // +0x8d
    bool                    StateGroupUpdated;           // +0x8e
    bool                    InitialisedCreaturePosition; // +0x8f
    void*                   PBrainDef;                   // +0x90
    unsigned char           UpdateZones[0x0c];           // +0x94 retail vector
    CBrainUpdateZone*       PCurrentUpdateZone;          // +0xa0
    bool                    OnKillCalled;                // +0xa4
    CBrainUpdateZone* GetUpdateZone(long zone);          // 0x0088B910
    void OnKill();
    bool ForceReset(CBrainOutput* output);
    bool IsReadyToRunStateGroupWithHigherScriptPriorityThan(long priority, long zone);
};

bool CAIBrain::IsReadyToRunStateGroupWithHigherScriptPriorityThan(long priority, long zone)
{
    CBrainUpdateZone* updateZone = GetUpdateZone(zone);
    PCurrentUpdateZone = updateZone;
    unsigned long period = 1;
    if (updateZone)
        period = updateZone->UpdatePeriod;
    unsigned __int64 offset = Creature->m_updateOffset;
    if ((offset + Component->GetFrameNumber()) % period != 0)
        return false;
    for (CStateGroupEntry* it = StateGroupBegin; it != StateGroupEnd; ++it) {
        CAIStateGroupBase* group = it->Group;
        if (group->ScriptAIPriority > priority && group->IsEnabledAndRunnable()) {
            if (PActiveStateGroup == 0) {
                PActiveStateGroup = group;
                group->Activate();
                PActiveStateGroup->OnStart();
            }
            return true;
        }
    }
    return false;
}