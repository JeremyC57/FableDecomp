#ifndef FABLE_ENGINE_CENGINEGRAPHIC_H
#define FABLE_ENGINE_CENGINEGRAPHIC_H
// CEngineGraphic (new shared header, chunk 1002): Ego_r.pdb CEngineGraphic (struct_layouts_egor.tsv, 14 bytes: byte-packed; class key V,
// ghidra_out/pdb_class_keys.tsv), the graphic reference of inventory items (CTCInventoryItem::GetEngineGraphic 0x005D92D0 returns it by
// const reference; 0x005CB2B0 reads BankIndex +0) and of the graphic appearance (the landed 0x005C552C model's CTCGraphicAppearance +0x44).
#pragma pack(push, 1)
class CEngineGraphic {
public:
    unsigned long BankIndex;                                         // +0x00
    float         AnimStep;                                          // +0x04
    float         RenderSizeX;                                       // +0x08
    bool          AdditiveAlpha;                                     // +0x0c
    unsigned char Type;                                              // +0x0d
#ifdef FABLE_RETAIL_CENGINEGRAPHIC_CTOR
    // Retail opt-in (chunk 1002b, rows 0x005C3E91 / 0x005C47D5): Ego_r.pdb ??0CEngineGraphic@@QAE@XZ, the default constructor that
    // zeroes the five members in order (inline in the landed 0x005D96A0). It makes the class non-POD, which is what puts the static
    // GetEngineGraphic(long)'s hidden result pointer in ecx (`mov edx,edi ; lea ecx,[ebp-6Eh] ; call 0x005D96A0`) instead of on the stack.
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { }
#endif
};
#pragma pack(pop)
#endif // FABLE_ENGINE_CENGINEGRAPHIC_H
