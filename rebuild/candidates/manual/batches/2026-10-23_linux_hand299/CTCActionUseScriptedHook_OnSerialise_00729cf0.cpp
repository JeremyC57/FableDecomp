// CTCActionUseScriptedHook::OnSerialise(CPersistContext& context, bool) -- persist the hook's flags, entrance/camera
// UIDs (versioned block) and its sound/animation names and replacement object
// Retail 0x00729CF0, 0x26C bytes, `ret 8`. Callees: CPersistContext::Transfer(const char*, bool&, const bool&)
// 0x004045C0, Transfer(const char*, long&, const long&) 0x00410BE0, Transfer(const char*, unsigned __int64&,
// const unsigned __int64&) 0x0044BB00, Transfer(const char*, CCharString*) 0x00411010 (the manifest's
// Transfer<signed char> labels are folded), CThing::IsLevelLess 0x004C7930, CTCBase::GetThingManager 0x00686AE0,
// CThingManager::RemapUIDForSerialisation 0x0051E210 (edx:eax result).
// Shape: the name/animation/replacement tail is written in BOTH arms (VC7.1 cross-jumps the copies; a single tail after
// the if/else schedules `mov ecx,edi` after the name push). Saving (mode 1 or 3) writes VersionNumber 1 and transfers a UID only when it is set or the context is
// also loading (mode 2 or 3); loading reads the version, bails out on anything but 1, and remaps each non-zero UID.
// Layout = Ego_r.pdb CTCActionUseScriptedHook (0x50 bytes; the landed 0x00729C30 agrees on EntranceLocked +0x28).
#include "engine/CTCBase.h"
struct CCharString { void* PStringData; };
struct CThing {
    unsigned char _pad00[0x8e];
    short m_level;                              // +0x8e
    bool IsLevelLess();                         // 0x004C7930
};
struct CThingManager {
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};
struct CPersistContext {
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    bool IsLoading() const { return Mode == 2 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);                         // 0x004045C0
    void Transfer(const char* name, long& value, const long& defaultValue);                         // 0x00410BE0
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue); // 0x0044BB00
    void Transfer(const char* name, CCharString* value);                                            // 0x00411010
};
struct CIntelligentPointerThing { int _padding_; CThing* PData; };

class CTCActionUseScriptedHook : public CTCBase {
public:
    bool Usable;                                // +0x0c
    bool ForceConfirmation;                     // +0x0d
    bool WaitingForConfirmation;                // +0x0e
    bool PerformAction;                         // +0x0f
    bool TeleportToRegionEntrance;              // +0x10
    unsigned char _pad11[7];
    unsigned __int64 EntranceConnectedToUID;    // +0x18
    CIntelligentPointerThing PEntranceConnectedTo; // +0x20
    bool EntranceLocked;                        // +0x28
    unsigned char _pad29[7];
    unsigned __int64 CameraTrackUID;            // +0x30
    CIntelligentPointerThing PCameraTrack;      // +0x38
    CCharString SoundName;                      // +0x40
    CCharString AnimationName;                  // +0x44
    long ReplacementObject;                     // +0x48
    bool ReversedOnMiniMap;                     // +0x4c
    bool HiddenOnMiniMap;                       // +0x4d
    CThingManager* GetThingManager();           // 0x00686AE0 (CTCBase)
    void OnSerialise(CPersistContext& context, bool);
};

void CTCActionUseScriptedHook::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("Usable", Usable, true);
    context.Transfer("ReversedOnMiniMap", ReversedOnMiniMap, false);
    context.Transfer("HiddenOnMiniMap", HiddenOnMiniMap, false);
    if (context.IsSaving()) {
        long version = 1;
        context.Transfer("VersionNumber", version, 0);
        context.Transfer("ForceConfirmation", ForceConfirmation, false);
        context.Transfer("TeleportToRegionEntrance", TeleportToRegionEntrance, false);
        if (EntranceConnectedToUID != 0 || context.IsLoading())
            context.Transfer("EntranceConnectedToUID", EntranceConnectedToUID, 0);
        if (CameraTrackUID != 0 || context.IsLoading())
            context.Transfer("CameraTrackUID", CameraTrackUID, 0);
        context.Transfer("SoundName", &SoundName);
        context.Transfer("AnimationName", &AnimationName);
        context.Transfer("ReplacementObject", ReplacementObject, 0);
    } else {
        long version;
        context.Transfer("VersionNumber", version, 0);
        if (version != 1)
            return;
        context.Transfer("ForceConfirmation", ForceConfirmation, false);
        context.Transfer("TeleportToRegionEntrance", TeleportToRegionEntrance, false);
        context.Transfer("EntranceConnectedToUID", EntranceConnectedToUID, 0);
        if (EntranceConnectedToUID != 0)
            EntranceConnectedToUID = GetThingManager()->RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, EntranceConnectedToUID);
        context.Transfer("CameraTrackUID", CameraTrackUID, 0);
        if (CameraTrackUID != 0)
            CameraTrackUID = GetThingManager()->RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, CameraTrackUID);
        context.Transfer("SoundName", &SoundName);
        context.Transfer("AnimationName", &AnimationName);
        context.Transfer("ReplacementObject", ReplacementObject, 0);
    }
}
