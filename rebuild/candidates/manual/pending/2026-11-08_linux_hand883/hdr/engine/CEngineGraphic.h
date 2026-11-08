#ifndef FABLE_ENGINE_CENGINEGRAPHIC_H
#define FABLE_ENGINE_CENGINEGRAPHIC_H
// CEngineGraphic (new shared header, chunk 883, row 0x006096E0 CTCInventoryMap::ConstructLogbookListPC): the graphic reference a
// def carries (bank index of the mesh / sprite, animation step, render size, additive-alpha flag, engine graphic type).
// Layout: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) CEngineGraphic, 14 bytes: BankIndex +0 (ulong), AnimStep +4, RenderSizeX
// +8 (float), AdditiveAlpha +0xC (bool), Type +0xD (uchar). Byte-packed: CAppearanceDef::Graphic sits at +0x25 right after the
// 0x25-byte CDefClassBase, and CThingObjectDef::Graphic at retail +0x70 has its Type byte at +0x7D (`cmp byte ptr [edx+7Dh],4`,
// 0x0060B3E9). The member set is the PDB's. Constructors: FableWin.pdb ??0CEngineGraphic@@QAE@XZ and
// ??0CEngineGraphic@@QAE@W4EEngineGraphicType@@KMM_N@Z (declared only; their bodies are not proven). They make the class a
// non-aggregate, which is what retail's calls show: a __fastcall function returning one by value takes the hidden result
// pointer in ecx (CTCInventoryItem::GetEngineGraphic 0x005D96A0 at 0x0060B565: `lea ecx,[esp+10Ch]` with the index in edx) and
// the caller reads the result in place (`cmp dword ptr [eax],0`, 0x0060B571); an aggregate is passed the pointer on the stack
// and copied out.
#include "rebuild_abi.h"
enum EEngineGraphicType;
#pragma pack(push, 1)
class CEngineGraphic {
public:
    CEngineGraphic();                                                    // ??0CEngineGraphic@@QAE@XZ
    CEngineGraphic(EEngineGraphicType, unsigned long, float, float, bool); // ??0CEngineGraphic@@QAE@W4EEngineGraphicType@@KMM_N@Z
    unsigned long BankIndex;                                             // +0x00
    float         AnimStep;                                              // +0x04
    float         RenderSizeX;                                           // +0x08
    bool          AdditiveAlpha;                                         // +0x0C
    unsigned char Type;                                                  // +0x0D (EEngineGraphicType, stored as a byte)
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CEngineGraphic) == 14);
#endif // FABLE_ENGINE_CENGINEGRAPHIC_H
