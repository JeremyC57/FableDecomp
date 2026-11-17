// CTCDExplosion::SetSpecificThingsToHit(const vector<CThing*>& things) -- append an intelligent pointer to each given thing to the explosion's list of things it must hit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080CC40, 162 bytes (`ret 4`; VC7.1 aligns the frame itself, `and esp,-8`, for the
// 8-byte intelligent-pointer temporary).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), the vector's _M_insert_overflow @ 0x004AC940 (manifest
// "Std_Vector_Insert_Range_SmartPtr").
// Layout: Ego_r.pdb CTCDExplosion SpecifiedThingsToHit +0x78 sits at retail +0x6c (three 12-byte vectors before it).
// Shape: an iterator loop over the argument vector re-reading its end through the reference every pass; each element becomes
// a CIntelligentPointer temporary (the item loaded before the base ctor) handed to the inline STLport push_back (placement
// copy: base ctor, vtable, SetPItem(GetPItem()); or _M_insert_overflow with the __false_type tag in the high byte of the
// frame word below the temporary), destroyed through the base dtor.
// Name fix (agentNF54): the local CBaseIntelligentPointer / CIntelligentPointer / CRetailVector stand-ins replaced by the shared
//   engine/CIntelligentPointer.h (retail branch, FABLE_RETAIL_CINTELLIGENTPOINTER_COPY) over engine/CThing.h and the STLport 4.6 vector of
//   stlport/fable_containers.h: the overflow call 0x004AC940 binds
//   ?_M_insert_overflow@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@IAEXPAV?$CIntelligentPointer@VCThing@@@@ABV3@ABU__false_type@2@I_N@Z
//   (the landed row's own leaf; was ?_M_insert_overflow@?$CRetailVector@UCIntelligentPointer@@@@...), and the row's own symbol becomes the
//   PDB spelling ?SetSpecificThingsToHit@CTCDExplosion@@QAEXABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z (Ego_r 0x00808A30 +
//   FableWin 0x0276C390; was ...ABU?$CRetailVector@PAVCThing@@@@@Z).
// Leaf: ?SetSpecificThingsToHit@CTCDExplosion@@QAEXABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
class CTCDExplosion { public:
    virtual void v00();
    unsigned char _pad04[0x6c - 0x04];
    std::vector<CIntelligentPointer<CThing> > SpecifiedThingsToHit;   // +0x6c (PDB +0x78)
    void SetSpecificThingsToHit(const std::vector<CThing*>& things);
};

void CTCDExplosion::SetSpecificThingsToHit(const std::vector<CThing*>& things)
{
    for (CThing* const* it = things.begin(); it != things.end(); ++it)
        SpecifiedThingsToHit.push_back(CIntelligentPointer<CThing>(*it));
}