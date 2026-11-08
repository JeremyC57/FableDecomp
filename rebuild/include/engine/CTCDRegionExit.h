#ifndef FABLE_ENGINE_CTCDREGIONEXIT_H
#define FABLE_ENGINE_CTCDREGIONEXIT_H
// CTCDRegionExit (chunk 849, row 0x00501450 CWorldMap::GenerateRegionConnectivityGraph): Ego_r.pdb members and order
// (struct_layouts_egor.tsv, sizeof 56: no containers, so the retail size is the PDB's), the CTCDriverBase base (Codex pass 13 TPI
// read; ??$PeekTCDriver@$$CBVCTCDRegionExit@@@CThing@@ in Ego_r.pdb), interface type TCI_REGION_EXIT = 100 (the row's UsingComponents
// bit test `test dl,10h` on byte +0x2C, 0x00501580, then the const LowerBound with key 0x64). PEntranceConnectedTo sits at the PDB's
// +0x2A only with the byte-packed pointer classes (FABLE_RETAIL_CINTELLIGENTPOINTER); otherwise it is opaque bytes.
#include "engine/CTCDriverBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CCharString;
class CTCDRegionExit : public CTCDriverBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_REGION_EXIT; }
#ifdef FABLE_RETAIL_CTCD_STATGETDRIVERTYPE
    // Retail opt-in (chunk 864, row 0x0050A650): the driver type this class registers with (CTCBase::StatGetDriverType 0x006869C0 is
    // the non-driver default 0; drivers hide it): 25, the value its slot-28 virtual 0x00779940 compares with (`cmp ecx,19h`) and the
    // rough-area TCD scan 0x00518900 pushes (`push 19h`). The static itself is inlined everywhere in retail.
    static unsigned long __fastcall StatGetDriverType() { return 25; }
#endif
    // ?GetNameOfRegionConnectingTo@CTCDRegionExit@@QBEABVCCharString@@XZ: two byte-identical retail copies, 0x007791F0 and
    // 0x007792D0 (both landed under this name); the row calls 0x007792D0.
    const CCharString& GetNameOfRegionConnectingTo() const;
private:
    bool             Active;                                             // +0x0c
    bool             InLetterBoxMode;                                    // +0x0d
    bool             WaitingForConfirmation;                             // +0x0e
    bool             WaitingForActivationOrPlayerToLeave;                // +0x0f
    bool             WaitingForPlayerToLeaveMessageArea;                 // +0x10
    float            Radius;                                             // +0x14
    float            MessageRadius;                                      // +0x18
    unsigned __int64 EntranceConnectedToUID;                             // +0x20
    bool             ReversedOnMiniMap;                                  // +0x28
    bool             HiddenOnMiniMap;                                    // +0x29
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER
    CIntelligentPointer<CThing> PEntranceConnectedTo;                    // +0x2a
#else
    unsigned char    PEntranceConnectedTo[0x8];                          // +0x2a CIntelligentPointer<CThing>
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCDRegionExit) == 0x38);
#endif
