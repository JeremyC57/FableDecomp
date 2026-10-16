#pragma optimize("s", on)
// CTCVillage::GetNumberOfGayVillagers() -- count the live village people whose creature-type query (0x00661FF0) is 1 and whose flag byte at +0x1c9 has both bits 3 and 4 set
// Leaf: GetNumberOfGayVillagers
// Retail: Fable.exe @ 0x00658F41, 97 bytes; size TU (unaligned entry, as the landed CTCVillage rows): `optimize("s",on)`.
// Callees: IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>&) 0x004AB8F0 (__fastcall); CBaseIntelligentPointer::
//          GetPItem 0x00A01B50 (x3); the creature query 0x00661FF0 (manifest "CPixelShader::CountReferences" is a propagated
//          label; returns a long, 1 = the counted kind).
// Layout: retail CTCVillage (lists are a bare 4-byte head pointer, see the landed CTCVillage rows): VillagePeople +0x2c
//         (PDB +0x34); list node {next, prev, CIntelligentPointer<CThingAICreature> +8}; the creature's flag byte +0x1c9.
// Shape: the count lives in ebp; the list end is re-read from the member each pass; the element address is kept in ebx.
class CThingAICreature {
public:
    unsigned char _pad_0x00[0x1c9];
    unsigned char m_flags1c9;                   // +0x1c9 (bit 3, bit 4)
    long GetKindQuery();                        // 0x00661FF0
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    void* _vfptr;
    CThingAICreature* m_pItem;
    CThingAICreature* GetPItem() const;         // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& thing);   // 0x004AB8F0

struct SVillagerNode {                          // STLport list node
    SVillagerNode* _M_next;
    SVillagerNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};

struct CTCVillage {
    unsigned char  _pad_0x00[0x2c];
    SVillagerNode* VillagePeople;               // +0x2c list head node
    long GetNumberOfGayVillagers();
};

long CTCVillage::GetNumberOfGayVillagers()
{
    long count = 0;
    for (SVillagerNode* node = VillagePeople->_M_next; node != VillagePeople; node = node->_M_next) {
        CBaseIntelligentPointer& villager = node->_M_data;
        if (IS_THING_ALIVE(villager) && villager.GetPItem()->GetKindQuery() == 1 &&
            (villager.GetPItem()->m_flags1c9 & 8) && (villager.GetPItem()->m_flags1c9 & 0x10))
            ++count;
    }
    return count;
}
