// CAtmosProcess::~CAtmosProcess() -- atmos-sound process destructor: when the sound manager exists, clear its atmos sounds and its atmos map, then the members are destroyed (the rain-player intelligent pointer)
// Leaf: ??1CAtmosProcess@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1550, 44 bytes, thiscall, tail jump to ~CBaseIntelligentPointer.
// Sound manager (agentSND2, 2026-11-10): engine/CASoundManager.h (with FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP for the nested map base);
//   was a local CASoundManager / CAtmosMap model (ClearAtmosSounds / ClearAtmosMap, GetAtmosAt / GetAtmosLevelAt).
// Identity (proven): (1) the only callers are 0x004A8380 (a scalar deleting destructor: dtor, `test [esp+8],1`, delete) and
//   0x004A9B50, the counted-pointer DeleteData (null test, dtor, delete) that CWorld::PostInit 0x004A6550 stores in the count block
//   when it puts `new (0x20) ...` (ctor 0x006B1960) into CWorld +0x24 = PAtmosProcessor (CCountedPointer<CAtmosProcess>, engine/
//   CWorld.h); (2) Ego_r.pdb / FableWin.pdb hold ??1CAtmosProcess@@QAE@XZ (non-virtual: no vtable store) beside
//   ?DeleteData@?$CCountedPointer@VCAtmosProcess@@@@SIXPAX@Z and FableWin ??_GCAtmosProcess@@QAEPAXI@Z; (3) the member it
//   destroys at +0x0A is the CIntelligentPointer<CThingMarker> (vtable 0x0125A860) the ctor 0x006B1960 builds = Ego_r PRainPlayer.
//   The catalog label "CCounter::operator=" is wrong.
// Callees: CASoundManager slots 41 DeleteAllAtmoses (+0xA4) and 42 StopAllAtmos (+0xA8) through PSoundManager 0x013B8394 (re-read
//   after the first call); CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10 (Ego_r ??1CBaseIntelligentPointer@@UAE@XZ:
//   CIntelligentPointer<T> declares no destructor, so its implicit one only chains to the base, with no vtable reset).
//   Slot names: engine/CASoundManager.h (Fable Anniversary 360 PDB; agentSND SLOTS.tsv).
// Layout (retail 0x20, the ctor 0x006B1960; Ego_r 0x1C lacks the TLC atmos map): DayNightState +0x00, the CMyAtmosMap member
//   (vtable 0x0125D47C, RTTI CAtmosProcess::CMyAtmosMap : CASoundManager::CAtmosMap, two virtuals, no destructor) +0x04,
//   RainCurrentlyPlaying +0x08, AtmosSoundsMuted +0x09, PRainPlayer +0x0A (the pointer classes are pack(1), Ego_r has it at +6),
//   InsideBuilding +0x14 (= -1 in the ctor), ReverbType +0x18, ReverbLevel +0x1C. No engine/CAtmosProcess.h exists.
class C3DVector;
class CThingMarker;
class CBaseObjectPointer;
enum EDayNightState { };
enum EReverbEnvironmentType { };

#define FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP
#include "engine/CASoundManager.h"

#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

class CAtmosProcess {
public:
    class CMyAtmosMap : public CASoundManager::CAtmosMap {
    public:
        virtual long GetBlockValue(const C2DCoordI& block);               // 0x006B1580 (the landed spelling)
        virtual float GetBlockHeight(const C2DCoordI& block);             // 0x006B15C0
    };
    CAtmosProcess();                                                      // 0x006B1960
    ~CAtmosProcess();
    EDayNightState                     DayNightState;                     // +0x00
    CMyAtmosMap                        AtmosMap;                          // +0x04 (TLC; descriptive member name)
    bool                               RainCurrentlyPlaying;              // +0x08
    bool                               AtmosSoundsMuted;                  // +0x09
    CIntelligentPointer<CThingMarker>  PRainPlayer;                       // +0x0A
    long                               InsideBuilding;                    // +0x14
    EReverbEnvironmentType             ReverbType;                        // +0x18
    float                              ReverbLevel;                       // +0x1C
};

CAtmosProcess::~CAtmosProcess()
{
    if (PSoundManager) {
        PSoundManager->DeleteAllAtmoses();
        PSoundManager->StopAllAtmos();
    }
}