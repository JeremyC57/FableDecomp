#include <cstdio>
#include <new>
class CThing { public: int id; };
class CTCBase { public: CThing* Thing; CTCBase(CThing* t) : Thing(t) { } virtual ~CTCBase() { } };
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) { } virtual ~CIntelligentPointer() { } };
class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct CDefPointerTrackerDef { const void* Object; CDefPointerTrackerDef() : Object(0) { } };
class CTCVolumeContainmentTracker : public CTCBase { public:
    CIntelligentPointer PThingInside; CEnginePrimitiveThemeChannelDesc CurrentThemeChannel; bool PreviouslyUpdated;
    CDefPointerTrackerDef PDef; bool UpdateLightingChannels;
    CTCVolumeContainmentTracker(CThing* thing) : CTCBase(thing), PreviouslyUpdated(false), UpdateLightingChannels(true) { }
};
int main()
{
    CThing owner;
    unsigned char raw[sizeof(CTCVolumeContainmentTracker)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0x33;
    CTCVolumeContainmentTracker* t = new (raw) CTCVolumeContainmentTracker(&owner);
    if (t->Thing != &owner || t->PThingInside.m_pItem || t->CurrentThemeChannel.ChannelIndex0 || t->CurrentThemeChannel.ChannelIndex1
        || t->CurrentThemeChannel.ChannelBlendFactor || t->CurrentThemeChannel.Pad != 0x33 || t->PreviouslyUpdated || t->PDef.Object || !t->UpdateLightingChannels) {
        std::printf("00785110_TEST FAIL\n"); return 1;
    }
    std::printf("00785110_TEST PASS\n");
    return 0;
}