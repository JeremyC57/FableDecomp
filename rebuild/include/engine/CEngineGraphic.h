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
};
#pragma pack(pop)
#endif // FABLE_ENGINE_CENGINEGRAPHIC_H
