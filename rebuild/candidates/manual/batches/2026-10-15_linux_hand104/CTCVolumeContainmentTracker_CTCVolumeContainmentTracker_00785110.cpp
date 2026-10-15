// CTCVolumeContainmentTracker::CTCVolumeContainmentTracker(CThing*) -- CTCBase(owner), own vtable, an empty PThingInside pointer, a zero theme channel, not previously updated, a null def, UpdateLightingChannels = true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00785110, length 64 bytes. Vtable 0x0126B66C = CTCVolumeContainmentTracker.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (then the CIntelligentPointer vtable 0x01238C6C).
// Shape: members in declaration order after the vtable store: PThingInside's inline constructor, then one zero
// register for the theme channel's three bytes (its default constructor leaves Pad alone), PreviouslyUpdated and the
// CDefPointer, the `true` immediate last.
// Layout: Ego_r.pdb CTCVolumeContainmentTracker (no containers, retail = PDB): PThingInside +0x0C,
// CurrentThemeChannel +0x14 (CEnginePrimitiveThemeChannelDesc: ChannelIndex0/1, ChannelBlendFactor, Pad),
// PreviouslyUpdated +0x18, PDef +0x1C, UpdateLightingChannels +0x20. No engine header.
struct CThing;
struct CBaseObject;

struct CTCBase {
    virtual void v00();
    CThing*       Thing;                    // +0x04
    unsigned char _pad_0x08[0x4];
    CTCBase(CThing* thing);                 // 0x00686800
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                   // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {          // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CEnginePrimitiveThemeChannelDesc {
    unsigned char ChannelIndex0;            // +0x00
    unsigned char ChannelIndex1;            // +0x01
    unsigned char ChannelBlendFactor;       // +0x02
    unsigned char Pad;                      // +0x03
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};

struct CTCVolumeContainmentTrackerDef;

struct CDefPointerTrackerDef {
    const CTCVolumeContainmentTrackerDef* Object;
    CDefPointerTrackerDef() : Object(0) { }
};

struct CTCVolumeContainmentTracker : CTCBase {
    CIntelligentPointer              PThingInside;            // +0x0C
    CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;     // +0x14
    bool                             PreviouslyUpdated;       // +0x18
    CDefPointerTrackerDef            PDef;                    // +0x1C
    bool                             UpdateLightingChannels;  // +0x20
    virtual void v00();
    CTCVolumeContainmentTracker(CThing* thing);
};

CTCVolumeContainmentTracker::CTCVolumeContainmentTracker(CThing* thing)
    : CTCBase(thing), PreviouslyUpdated(false), UpdateLightingChannels(true)
{
}
