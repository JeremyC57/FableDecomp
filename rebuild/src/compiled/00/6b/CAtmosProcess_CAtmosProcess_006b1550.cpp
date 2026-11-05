// CAtmosProcess::~CAtmosProcess() -- atmos-sound process destructor: when the sound manager exists, clear its atmos sounds and its atmos map, then the members are destroyed (the rain-player intelligent pointer)
// Leaf: ??1CAtmosProcess@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1550, 44 bytes, thiscall, tail jump to ~CBaseIntelligentPointer.
// Identity (proven): (1) the only callers are 0x004A8380 (a scalar deleting destructor: dtor, `test [esp+8],1`, delete) and
//   0x004A9B50, the counted-pointer DeleteData (null test, dtor, delete) that CWorld::PostInit 0x004A6550 stores in the count block
//   when it puts `new (0x20) ...` (ctor 0x006B1960) into CWorld +0x24 = PAtmosProcessor (CCountedPointer<CAtmosProcess>, engine/
//   CWorld.h); (2) Ego_r.pdb / FableWin.pdb hold ??1CAtmosProcess@@QAE@XZ (non-virtual: no vtable store) beside
//   ?DeleteData@?$CCountedPointer@VCAtmosProcess@@@@SIXPAX@Z and FableWin ??_GCAtmosProcess@@QAEPAXI@Z; (3) the member it
//   destroys at +0x0A is the CIntelligentPointer<CThingMarker> (vtable 0x0125A860) the ctor 0x006B1960 builds = Ego_r PRainPlayer.
//   The catalog label "CCounter::operator=" is wrong.
// Callees: the sound manager's virtual slots 41 (+0xA4) and 42 (+0xA8) through PSoundManager 0x013B8394 (re-read after the first
//   call); CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10 (Ego_r ??1CBaseIntelligentPointer@@UAE@XZ:
//   CIntelligentPointer<T> declares no destructor, so its implicit one only chains to the base, with no vtable reset).
//   CASoundManager is a TLC class absent from both PDBs: the two slot names are descriptive (CSoundManagerLH 0x00A3F340 empties the
//   atmos-sound lists and stops their samples; 0x00A3E920 hands the audio thread a fresh empty atmos-map object).
// Layout (retail 0x20, the ctor 0x006B1960; Ego_r 0x1C lacks the TLC atmos map): DayNightState +0x00, the CMyAtmosMap member
//   (vtable 0x0125D47C, RTTI CAtmosProcess::CMyAtmosMap : CASoundManager::CAtmosMap, two virtuals, no destructor) +0x04,
//   RainCurrentlyPlaying +0x08, AtmosSoundsMuted +0x09, PRainPlayer +0x0A (the pointer classes are pack(1), Ego_r has it at +6),
//   InsideBuilding +0x14 (= -1 in the ctor), ReverbType +0x18, ReverbLevel +0x1C. No engine/CAtmosProcess.h exists.
class C3DVector;
class CThingMarker;
class CBaseObjectPointer;
enum EDayNightState { };
enum EReverbEnvironmentType { };

class CASoundManager {
public:
    class CAtmosMap {
    public:
        virtual long  GetAtmosAt(const C3DVector& pos) const = 0;          // slot 0 (descriptive)
        virtual float GetAtmosLevelAt(const C3DVector& pos) const = 0;     // slot 1 (descriptive)
    };
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040();
    virtual void ClearAtmosSounds();                                      // slot 41 (+0xA4, descriptive)
    virtual void ClearAtmosMap();                                         // slot 42 (+0xA8, descriptive)
};
extern CASoundManager* PSoundManager;                                     // 0x013B8394 ?PSoundManager@@3PAVCASoundManager@@A

#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

class CAtmosProcess {
public:
    class CMyAtmosMap : public CASoundManager::CAtmosMap {
    public:
        virtual long  GetAtmosAt(const C3DVector& pos) const;             // 0x006B1580
        virtual float GetAtmosLevelAt(const C3DVector& pos) const;        // 0x006B15C0
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
        PSoundManager->ClearAtmosSounds();
        PSoundManager->ClearAtmosMap();
    }
}