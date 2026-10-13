// CItemOwner_0072d360::SetItem(CIntelligentPointer) -- m_2c.SetPItem(item.GetPItem()); the by-value argument is destroyed here
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072D360, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esp+8] ; call GetPItem ; push eax ; lea ecx,[esi+0x2c] ; call SetPItem ; lea ecx,[esp+8] ; call ~CIntelligentPointer ; pop esi ; ret 8
// Takes an 8-byte CIntelligentPointer by value (so the callee destroys it, `ret 8`), reads its
// target through CBaseIntelligentPointer::GetPItem @ 0x00A01B50 and stores it into the member
// pointer at +0x2c through SetPItem @ 0x00A01B90; the argument's destructor @ 0x00A01C10
// runs before returning. The manifest label (`CTCInventoryBase::GetItemDescription`) is
// propagated; the class is named descriptively. One of 14 bodies of this shape.
struct CBaseObjectPointer;

struct CIntelligentPointer {              // CBaseIntelligentPointer: 8 bytes (struct_layouts_egor.tsv)
    void* m_pLink;                         // +0
    CBaseObjectPointer* m_pData;           // +4
    CBaseObjectPointer* GetPItem();        // 0x00A01B50
    void SetPItem(CBaseObjectPointer* item);   // 0x00A01B90
    ~CIntelligentPointer();                // 0x00A01C10
};

struct CItemOwner_0072d360 {
    unsigned char _pad[0x2c];
    CIntelligentPointer m_item;            // +0x2c
    void SetItem_CItemOwner_0072d360(CIntelligentPointer item);
};

void CItemOwner_0072d360::SetItem_CItemOwner_0072d360(CIntelligentPointer item)
{
    m_item.SetPItem(item.GetPItem());
}
