#ifndef FABLE_ENGINE_CSYSTEMFONT_H
#define FABLE_ENGINE_CSYSTEMFONT_H
// Hand-written (chunk 761, row 0x00A0B060 ~CRenderManagerCore): RTTI .?AVCSystemFont@@ : CInitBaseClass : CBaseClass
// (ghidra_out/rtti_bases.tsv); no own data members in retail (CRenderManagerCore::SystemFont +0x3A4C is 8 bytes = vptr + Valid).
// Members: Ego_r.pdb / FableWin.pdb ??0CSystemFont@@QAE@XZ, ?Initialise@CSystemFont@@QAE_NXZ (retail 0x00A6C170),
// ?Uninitialise@CSystemFont@@QAEXXZ (retail 0x00A6C120: `jmp 0x0099A360`, the protected CInitBaseClass::Uninitialise, right before
// Initialise as in the donor), ?GetCharacter@CSystemFont@@QAEPAVCSystemFontCharacter@@E@Z. Needs FABLE_RETAIL_CINITBASECLASS (IsValid).
#include "engine/CInitBaseClass.h"
class CSystemFontCharacter;
class CSystemFont : public CInitBaseClass {
public:
    CSystemFont();
    bool Initialise();                                                   // 0x00A6C170
    void Uninitialise();                                                 // 0x00A6C120
    CSystemFontCharacter* GetCharacter(unsigned char);
};
#endif // FABLE_ENGINE_CSYSTEMFONT_H
