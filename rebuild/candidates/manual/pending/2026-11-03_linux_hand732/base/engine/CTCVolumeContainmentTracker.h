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
class CTCVolumeContainmentTracker : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_VOLUME_CONTAINMENT_TRACKER; }
    CThing* GetPThingInside() const { return PThingInside.Get(); }
    CIntelligentPointer<CThing> PThingInside;                // +0x0c
};
#endif
