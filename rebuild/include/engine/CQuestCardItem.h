#ifndef FABLE_ENGINE_CQUESTCARDITEM_H
#define FABLE_ENGINE_CQUESTCARDITEM_H
// New header (chunk 1023, row 0x0061A6A0 CTCInventoryQuests::OnCreate): one entry of CTCInventoryQuests::QuestCardItemList.
// Ego_r.pdb CQuestCardItem (8 bytes: ItemIndex +0, ItemText +4; ghidra_out/struct_layouts_egor.tsv), class key V (PDB name sets
// ??0CQuestCardItem@@QAE@XZ / ??4CQuestCardItem@@QAEAAV0@ABV0@@Z / ??1). Retail expands the special members: the default
// constructor only constructs ItemText (CWideString() 0x0099AED0, ItemIndex left unset: 0x0061A74C), the assignment copies
// ItemIndex and calls CWideString::operator= 0x0099B7D0 (0x0061A83A..0x0061A84D), the destructor is ~CWideString 0x0099B510.
// Needs fable_string.h (CWideString) included first.
class CQuestCardItem {
public:
    long        ItemIndex;                                               // +0x00
    CWideString ItemText;                                                // +0x04
};
#endif // FABLE_ENGINE_CQUESTCARDITEM_H
