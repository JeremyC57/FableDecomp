#ifndef FABLE_ENGINE_NFIRE_CFIREPOINT_H
#define FABLE_ENGINE_NFIRE_CFIREPOINT_H
// NFire::CFirePoint (new header, hand3c, row 0x00780D80 CTCFlammable::FrameUpdate): Ego_r.pdb members in order (struct_layouts_egor.tsv,
// 44 bytes, no STL container, so the PDB offsets hold in retail: Resistance +0x0C is read at 0x00780DA4, Burning +0x10 at 0x00780DAB):
// HelperName, HelperIndex, EffectIndex, Resistance, Burning, FireDamageAfterFrame, PParentThing, PEffect, PSingeEffect. Accessors:
// FableWin.pdb ?IsOnFire@CFirePoint@NFire@@QBE_NXZ 0x01F4F1E0 (`Resistance <= 0`) and ?IsBurning@CFirePoint@NFire@@QBE_NXZ 0x01F519D0
// (`Burning`), inline in retail; ?SetAsBurning@CFirePoint@NFire@@QAEXXZ out of line (retail 0x00780540). Needs FABLE_RETAIL_CCHARSTRING
// and FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
class CThing;
namespace NFire {
class CFirePoint {
public:
    bool IsOnFire() const { return Resistance <= 0; }
    bool IsBurning() const { return Burning; }
    void SetAsBurning();                                                 // 0x00780540
    CCharString                 HelperName;                              // +0x00
    long                        HelperIndex;                             // +0x04
    long                        EffectIndex;                             // +0x08
    long                        Resistance;                              // +0x0c
    bool                        Burning;                                 // +0x10
    long                        FireDamageAfterFrame;                    // +0x14
    CThing*                     PParentThing;                            // +0x18
    CIntelligentPointer<CThing> PEffect;                                 // +0x1c
    CIntelligentPointer<CThing> PSingeEffect;                            // +0x24
};
}
FABLE_STATIC_ASSERT(sizeof(NFire::CFirePoint) == 0x2c);
#endif
