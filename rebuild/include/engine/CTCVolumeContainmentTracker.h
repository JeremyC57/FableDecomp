#ifndef FABLE_ENGINE_CTCVOLUMECONTAINMENTTRACKER_H
#define FABLE_ENGINE_CTCVOLUMECONTAINMENTTRACKER_H
// CTCVolumeContainmentTracker (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): the retail class over FABLE_RETAIL_CTCBASE.
// Ego_r.pdb: CIntelligentPointer<CThing> PThingInside at +0x0C (no container before it, so the retail offset is Ego_r's); FableWin.pdb
// ?StatGetInterfaceType@CTCVolumeContainmentTracker@@SI?AW4ETCInterfaceType@@XZ (GetTC key 0x75) and ?GetPThingInside@CTCVolumeContainmentTracker@@QBEPAVCThing@@XZ,
// both inline in retail (FrameUpdate: `lea ecx,[eax+0Ch] ; call GetPItem`). The members after PThingInside are not modelled here.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CIntelligentPointer.h"
class CThing;
#ifdef FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_LIGHTING
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/CDefPointer.h"
class CTCVolumeContainmentTrackerDef;
#endif
class CTCVolumeContainmentTracker : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_VOLUME_CONTAINMENT_TRACKER; }
    CThing* GetPThingInside() const { return PThingInside.Get(); }
    CIntelligentPointer<CThing> PThingInside;                // +0x0c
#ifdef FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_LIGHTING
    // Retail opt-in (chunk 782, row 0x007941D0 CTCUnholyPower ctor): the Ego_r.pdb members after PThingInside (no container among them, so
    // the retail offsets are Ego_r's) and FableWin.pdb ?SetUpdateLightingChannels@CTCVolumeContainmentTracker@@QAEX_N@Z, inline in retail
    // (the ctor stores `mov [eax+20h],bl` through the GetTC result).
    void SetUpdateLightingChannels(bool update) { UpdateLightingChannels = update; }
    CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;    // +0x14
    bool PreviouslyUpdated;                                  // +0x18
    CDefPointer<const CTCVolumeContainmentTrackerDef> PDef;  // +0x1c
    bool UpdateLightingChannels;                             // +0x20
#endif
};
#endif
