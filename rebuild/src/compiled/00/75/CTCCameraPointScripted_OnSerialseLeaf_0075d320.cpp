// CTCCameraPointScripted::OnSerialseLeaf(CPersistContext& context) -- persist the scripted camera point's look/start/end positions, transition time and the remapped UID of the thing it follows
// Leaf: OnSerialseLeaf
//
// Retail 0x0075D320, 594 bytes (`ret 4`; PDB name OnSerialseLeaf, sic).
// Callees: CPersistContext::Transfer<float> @ 0x00410620 (one per vector component; LookPosEnd defaults to LookPos),
// Transfer<unsigned __int64> @ 0x0044BB00 ("ThingUID", default 0), CThing::IsLevelLess @ 0x004C7930,
// CTCBase::GetThingManager @ 0x00686AE0, CThingManager::RemapUIDForSerialisation @ 0x0051E210.
// Layout = Ego_r.pdb CTCCameraPointScripted (0x88): StartPos +0x40, EndPos +0x4c, LookPos +0x58, LookPosEnd +0x64,
// ThingUID +0x70, PAT +0x78, TransitionTime +0x7c, LookDirSet/StartPosSet/EndPosSet +0x80..0x82; CPersistContext Mode +0x18.
// Shape: on load the three "set" flags are raised first; the UID tail follows the landed CTCDRegionExit::OnSerialise
// (0x00779430), except that the saving arm only transfers a non-zero UID.
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
    //   instances here: float 0x00410620; unsigned __int64 0x0044BB00
};
class C3DVector { public: float X, Y, Z; };
class CTCCameraPointScripted : public CTCBase {
public:
    unsigned char _base0c[0x40 - 0x0c];         // CTCCameraPointDefinitionBase members
    C3DVector StartPos;                         // +0x40
    C3DVector EndPos;                           // +0x4c
    C3DVector LookPos;                          // +0x58
    C3DVector LookPosEnd;                       // +0x64
    unsigned __int64 ThingUID;                  // +0x70
    long  PAT;                                  // +0x78
    float TransitionTime;                       // +0x7c
    bool  LookDirSet;                           // +0x80
    bool  StartPosSet;                          // +0x81
    bool  EndPosSet;                            // +0x82
    void OnSerialseLeaf(CPersistContext& context);
};

void CTCCameraPointScripted::OnSerialseLeaf(CPersistContext& context)
{
    if (!context.IsSaving()) {
        LookDirSet = true;
        StartPosSet = true;
        EndPosSet = true;
    }
    context.Transfer("LookDirection.X", LookPos.X, 0.0f);
    context.Transfer("LookDirection.Y", LookPos.Y, 0.0f);
    context.Transfer("LookDirection.Z", LookPos.Z, 0.0f);
    context.Transfer("LookDirectionEnd.X", LookPosEnd.X, LookPos.X);
    context.Transfer("LookDirectionEnd.Y", LookPosEnd.Y, LookPos.Y);
    context.Transfer("LookDirectionEnd.Z", LookPosEnd.Z, LookPos.Z);
    context.Transfer("StartPos.X", StartPos.X, 0.0f);
    context.Transfer("StartPos.Y", StartPos.Y, 0.0f);
    context.Transfer("StartPos.Z", StartPos.Z, 0.0f);
    context.Transfer("EndPos.X", EndPos.X, 0.0f);
    context.Transfer("EndPos.Y", EndPos.Y, 0.0f);
    context.Transfer("EndPos.Z", EndPos.Z, 0.0f);
    context.Transfer("TransitionTime", TransitionTime, 0.0f);
    if (context.IsSaving()) {
        if (ThingUID != 0) {
            unsigned __int64 uid = GetThingManager().RemapUIDForSerialisation(true, ParentThing->IsLevelLess(), ParentThing->m_level, ThingUID);
            context.Transfer<unsigned __int64>("ThingUID", uid, 0);
        }
    } else {
        context.Transfer<unsigned __int64>("ThingUID", ThingUID, 0);
        if (ThingUID != 0)
            ThingUID = GetThingManager().RemapUIDForSerialisation(false, ParentThing->IsLevelLess(), ParentThing->m_level, ThingUID);
    }
}