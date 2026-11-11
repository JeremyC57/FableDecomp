#ifndef FABLE_ENGINE_CTCBED_H
#define FABLE_ENGINE_CTCBED_H
// CTCBed (new shared header, chunk 2026, row 0x0092DA20 CAIStateGroup_MinionSleep::Update): the bed component, interface 0x35
// (TCI_BED 53; HasTC tests bit 21 of CThing +0x24, 0x0092DC27). RTTI .?AVCTCBed@@ : CTCBase (ghidra_out/rtti_bases.tsv). Members
// Ego_r.pdb (struct_layouts_egor.tsv, sizeof 0x18): Empty +0x0C, GettingIn +0x0D, GettingOut +0x0E, Animating +0x0F, BedSleepAnim
// +0x10, PDef (CDefPointer<const CBedDef>) +0x14 -- the landed StartGetInBedAnimation / StartGetOutOfBedAnimation 0x007469C0 /
// 0x00746A70 use the same offsets. Ego_r.pdb / FableWin.pdb ?GetDefinition@CTCBed@@QBE?AV?$CDefPointer@$$CBVCBedDef@@@@XZ is inline
// in retail (0x0092DC58: `mov edi,[tc+14h] ; test edi,edi ; je ; inc [edi+4]`, the CDefPointer copy), ?StartGetOutOfBedAnimation@CTCBed@@QAEXJ@Z
// out of line (0x00746A70, landed). Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE.
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefPointer.h"
#include "fable_compiler.h"
class CBedDef;
class CTCBed : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_BED; }
    CDefPointer<const CBedDef> GetDefinition() const { return PDef; }
    void StartGetOutOfBedAnimation(long animation);                      // 0x00746A70
private:
    bool                       Empty;                                    // +0x0c
    bool                       GettingIn;                                // +0x0d
    bool                       GettingOut;                               // +0x0e
    bool                       Animating;                                // +0x0f
    long                       BedSleepAnim;                             // +0x10
    CDefPointer<const CBedDef> PDef;                                     // +0x14
};
#endif // FABLE_ENGINE_CTCBED_H
