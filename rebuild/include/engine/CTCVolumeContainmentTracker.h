#ifndef FABLE_ENGINE_CTCVOLUMECONTAINMENTTRACKER_H
#define FABLE_ENGINE_CTCVOLUMECONTAINMENTTRACKER_H
#ifdef FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
// Original complete owner: Ego_r0x292f4, FableWin0x2b926, native member accesses00785620.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
class CTCVolumeContainmentTrackerDef;
class CTCVolumeContainmentTracker:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_VOLUME_CONTAINMENT_TRACKER;}
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCVolumeContainmentTracker(const CTCVolumeContainmentTracker & p0);
 CTCVolumeContainmentTracker(CThing & p0);
 virtual bool HasFrameUpdate() const;
 virtual void FrameUpdate();
 virtual void OnCreate();
 CThing * GetPThingInside() const{return PThingInside.Get();}
 const CEnginePrimitiveThemeChannelDesc & GetCurrentThemeChannel() const;
 void Update();
 void OnThingCarried(CThing & p0);
 bool GetUpdateLightingChannels() const;
#ifdef FABLE_RETAIL_DIVINE_FINALCLEANUP
 void SetUpdateLightingChannels(bool update){UpdateLightingChannels=update;}
#else
 void SetUpdateLightingChannels(bool p0);
#endif
private:
 void CalculateBlendedLightingChannel(CEnginePrimitiveThemeChannelDesc & p0);
 CIntelligentPointer<CThing> PThingInside;
 CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;
 bool PreviouslyUpdated;
 CDefPointer<CTCVolumeContainmentTrackerDef const > PDef;
 bool UpdateLightingChannels;
public:
 virtual ~CTCVolumeContainmentTracker();
 CTCVolumeContainmentTracker & operator=(const CTCVolumeContainmentTracker & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCVolumeContainmentTracker)==36);
#else
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
#endif
