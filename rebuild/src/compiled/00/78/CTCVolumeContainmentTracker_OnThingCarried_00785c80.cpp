// CTCVolumeContainmentTracker::OnThingCarried(CThing*) -- make sure the tracker has run once, then push its current lighting theme channel onto the carrier's graphic appearance and flag the appearance changed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00785C80, length 55 bytes (+ int3 padding).
// Callees: CTCVolumeContainmentTracker::Update @ 0x00785620, CTCGraphicAppearance::SetLightingChannel(const
// CEnginePrimitiveThemeChannelDesc&, bool, bool) @ 0x004BFB40, CTCGraphicAppearance::SetAsChanged(unsigned long)
// @ 0x004BF9E0 (manifest labels).
// Layout (Ego_r.pdb CTCVolumeContainmentTracker, 36 bytes): CTCBase 0xc, PThingInside +0x0c, CurrentThemeChannel
// +0x14, PreviouslyUpdated +0x18, PDef +0x1c, UpdateLightingChannels +0x20. The carrier's graphic-appearance TC
// pointer sits at CThing +0x64.
// Shape: the TC pointer is re-read from the thing after the first call (a plain member, no local); the channel's
// address is formed with `add esi,14h` because `this` is dead after it.
class CEnginePrimitiveThemeChannelDesc { public:
    long m_channel;
};

struct CTCGraphicAppearance {
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool immediate, bool recurse);   // 0x004BFB40
    void SetAsChanged(unsigned long flags);                 // 0x004BF9E0
};

class CThing { public:
    unsigned char         _pad_00[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance;             // +0x64
};

class CTCVolumeContainmentTracker { public:
    void*                            __vftable;             // +0x00
    CThing*                          ParentThing;           // +0x04
    bool                             ToBeRemoved;           // +0x08
    bool                             IsInGlobalUpdateTCs;   // +0x09
    unsigned char                    PThingInside[8];       // +0x0c CIntelligentPointer<CThing>
    CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;   // +0x14
    bool                             PreviouslyUpdated;     // +0x18
    const void*                      PDef;                  // +0x1c CDefPointer<CTCVolumeContainmentTrackerDef const>
    bool                             UpdateLightingChannels;// +0x20
    void Update();                                          // 0x00785620
    void OnThingCarried(CThing* carrier);
};

void CTCVolumeContainmentTracker::OnThingCarried(CThing* carrier)
{
    if (!PreviouslyUpdated)
        Update();
    if (carrier->m_pGraphicAppearance) {
        carrier->m_pGraphicAppearance->SetLightingChannel(CurrentThemeChannel, true, true);
        carrier->m_pGraphicAppearance->SetAsChanged(4);
    }
}