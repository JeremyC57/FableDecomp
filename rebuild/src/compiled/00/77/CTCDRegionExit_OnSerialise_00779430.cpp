// CTCDRegionExit::OnSerialise(CPersistContext& context, bool transfer) -- persist the exit's state (active flag, radii, minimap flags) and the UID of the entrance it connects to, remapped for the save and back on load
// Leaf: OnSerialise
//
// Retail 0x00779430, 351 bytes (`ret 8`).
// Callees: CPersistContext::Transfer<bool> @ 0x004045C0 ("Active" default true, "ReversedOnMiniMap", "HiddenOnMiniMap" default
// false), Transfer<float> @ 0x00410620 ("Radius", "MessageRadius", default 0), Transfer<unsigned __int64> @ 0x0044BB00
// ("EntranceConnectedToUID", default 0), CThing::IsLevelLess @ 0x004C7930, CTCBase::GetThingManager @ 0x00686AE0,
// CThingManager::RemapUIDForSerialisation(bool, bool, long, unsigned __int64) @ 0x0051E210.
// Layout = Ego_r.pdb CTCDRegionExit (0x38): Active +0x0c, Radius +0x14, MessageRadius +0x18, EntranceConnectedToUID +0x20,
// ReversedOnMiniMap +0x28, HiddenOnMiniMap +0x29; CThing level short +0x8e; CPersistContext Mode +0x18 (saving: 1 or 3).
// Shape: as the landed CTCActivationTrigger::OnSerialise (0x0077B0D0): the saving arm remaps into a zero-initialised local
// that is transferred, the loading arm transfers the member and remaps it in place when non-zero.
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
class CThingManager;

class CTCBase : public CTCBaseLayout
{
protected:
    CThingManager& GetThingManager();   // 0x00686AE0
};
class CThing { public:
    unsigned char _pad00[0x8e];
    short m_level;                              // +0x8e
public:
    bool IsLevelLess() const;   // 0x004C7930
};
class CThingManager { public:
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: bool 0x004045C0; float 0x00410620; unsigned __int64 0x0044BB00
};
class CTCDRegionExit : public CTCBase {
public:
    bool  Active;                               // +0x0c
    bool  InLetterBoxMode;                      // +0x0d
    bool  WaitingForConfirmation;               // +0x0e
    bool  WaitingForActivationOrPlayerToLeave;  // +0x0f
    bool  WaitingForPlayerToLeaveMessageArea;   // +0x10
    float Radius;                               // +0x14
    float MessageRadius;                        // +0x18
    unsigned __int64 EntranceConnectedToUID;    // +0x20
    bool  ReversedOnMiniMap;                    // +0x28
    bool  HiddenOnMiniMap;                      // +0x29
    void OnSerialise(CPersistContext& context, bool transfer);
};

void CTCDRegionExit::OnSerialise(CPersistContext& context, bool transfer)
{
    context.Transfer("Active", Active, true);
    context.Transfer("Radius", Radius, 0.0f);
    context.Transfer("MessageRadius", MessageRadius, 0.0f);
    context.Transfer("ReversedOnMiniMap", ReversedOnMiniMap, false);
    context.Transfer("HiddenOnMiniMap", HiddenOnMiniMap, false);
    if (context.IsSaving()) {
        unsigned __int64 uid = 0;
        if (EntranceConnectedToUID != 0)
            uid = GetThingManager().RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, EntranceConnectedToUID);
        context.Transfer<unsigned __int64>("EntranceConnectedToUID", uid, 0);
    } else {
        context.Transfer<unsigned __int64>("EntranceConnectedToUID", EntranceConnectedToUID, 0);
        if (EntranceConnectedToUID != 0)
            EntranceConnectedToUID = GetThingManager().RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, EntranceConnectedToUID);
    }
}