// CMiniMapDisplay::CMarkerInfo::IsReversedOnMap() const -- whether the marked thing's minimap icon is drawn mirrored: false for a missing or dead thing; a region-exit driver answers its ReversedOnMiniMap, else a scripted-hook action component (interface 0xC2) answers its own; false when it has neither
// Leaf: ?IsReversedOnMap@CMarkerInfo@CMiniMapDisplay@@QBE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A3E0, 118 bytes. Ego_r.pdb ?IsReversedOnMap@CMarkerInfo@CMiniMapDisplay@@QBE_NXZ.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (four times), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const
//   0x004365B0.
// Shape: IS_THING_ALIVE on PThing (GetPItem, then GetPItem's Dead bit), then CThing::PeekTCDriver<const CTCDRegionExit>
//   (TCI_REGION_EXIT 0x64: the UsingComponents bit, then PTCDriver +0x5C) and CThing::PeekTC<const CTCActionUseScriptedHook>
//   (TCI_ACTION_USE_SCRIPTED_HOOK 0xC2 through Components.Find), each answering its inline IsReversedOnMiniMap() (Ego_r.pdb
//   ?IsReversedOnMiniMap@CTCDRegionExit@@QBE_NXZ / @CTCActionUseScriptedHook@@QBE_NXZ; ReversedOnMiniMap +0x28 / +0x4C).
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCDriverBase.h"
class CTCDRegionExit : public CTCDriverBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_REGION_EXIT; }
    bool IsReversedOnMiniMap() const { return ReversedOnMiniMap; }
private:
    unsigned char _pad_0x0c[0x28 - 0x0c];
    bool ReversedOnMiniMap;                                  // +0x28
};
class CTCActionUseScriptedHook : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ACTION_USE_SCRIPTED_HOOK; }
    bool IsReversedOnMiniMap() const { return ReversedOnMiniMap; }
private:
    unsigned char _pad_0x0c[0x4c - 0x0c];
    bool ReversedOnMiniMap;                                  // +0x4c
};
template<class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { return p.Get() != 0 && !p.Get()->IsDead(); }
class CMiniMapDisplay {
public:
    class CMarkerInfo;
};
class CMiniMapDisplay::CMarkerInfo {
public:
    CIntelligentPointer<const CThing> PThing;                // +0x0
    long                              GraphicIndex;          // +0x8
    bool                              AutoMarker;            // +0xc
    bool IsReversedOnMap() const;
};

bool CMiniMapDisplay::CMarkerInfo::IsReversedOnMap() const
{
    if (IS_THING_ALIVE(PThing)) {
        const CTCDRegionExit* regionExit;
        if (PThing->PeekTCDriver(&regionExit))
            return regionExit->IsReversedOnMiniMap();
        const CTCActionUseScriptedHook* hook;
        if (PThing->PeekTC(&hook))
            return hook->IsReversedOnMiniMap();
    }
    return false;
}