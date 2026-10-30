// CWorld::SetAsLoadingRegion(const C3DVector&, float, bool, bool, bool) -- queue a region load at a start position unless a movie sequence blocks it or a load is already in progress
// Retail: Fable.exe @ 0x0049E2C0, 116 bytes. PDB ?SetAsLoadingRegion@CWorld@@IAEXABVC3DVector@@M_N11@Z (protected)
// Callee: CGameScriptInterface slot 395 IsInMovieSequence() 0x0088E9E0 through PGameScriptInterface (+0x38).
// Retail CWorld offsets (engine/CWorld.h is the PDB layout; the RegionLoad block sits 0xC below it because the PDB
// containers before it are 16 bytes and retail's 12, and 0x10 below after the 12-byte RegionLoadFollowers vector):
//   RegionLoadStatus +0x104 (PDB +0x110), RegionLoadStartPos +0x108 (PDB +0x114), RegionLoadStartAngleXY +0x114
//   (PDB +0x120), RegionLoadViaTeleporter +0x12C (PDB +0x13C), RegionLoadViaDoor +0x12D (PDB +0x13D),
//   RegionLoadWaitingForConfirmation +0x130 (PDB +0x140).
// Shape: the movie test is skipped when `force` is set; a load is accepted only when idle (status 0) or finished
// (status >= 8); the position is a struct copy (lea + three dwords).
#include <stddef.h>

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class C3DVector { public: float X, Y, Z; };

class CWorld { public:
    void*                 __vftable;                       // +0x00
    unsigned char         _pad04[0x34];
    CGameScriptInterface* PGameScriptInterface;            // +0x38
    unsigned char         _pad3c[0xc8];
    long                  RegionLoadStatus;                // +0x104 ERegionLoadStatus
    C3DVector             RegionLoadStartPos;              // +0x108
    float                 RegionLoadStartAngleXY;          // +0x114
    unsigned char         RegionLoadFollowers[0xc];        // +0x118 STLport vector<CIntelligentPointer<CThing> >
    long                  PreviousRegion;                  // +0x124
    long                  NumberOfTimesFreezeControlsModeAddedDuringRegionLoad;   // +0x128
    bool                  RegionLoadViaTeleporter;         // +0x12C
    bool                  RegionLoadViaDoor;               // +0x12D
    bool                  PutIntoPauseModeOnRegionChange;  // +0x12E
    bool                  DisplayMiniMapOnRegionLoad;      // +0x12F
    bool                  RegionLoadWaitingForConfirmation; // +0x130
    void SetAsLoadingRegion(const C3DVector& pos, float angleXY, bool viaTeleporter, bool force, bool viaDoor);
};

void CWorld::SetAsLoadingRegion(const C3DVector& pos, float angleXY, bool viaTeleporter, bool force, bool viaDoor)
{
    if (!force && PGameScriptInterface->IsInMovieSequence())
        return;
    if (RegionLoadStatus == 0 || RegionLoadStatus >= 8)
    {
        RegionLoadStartPos = pos;
        RegionLoadStartAngleXY = angleXY;
        RegionLoadViaTeleporter = viaTeleporter;
        RegionLoadViaDoor = viaDoor;
        RegionLoadWaitingForConfirmation = false;
        RegionLoadStatus = 1;
    }
}