// CItemOwner_0072d360::SetItem(CBaseIntelligentPointer) -- m_2c.SetPItem(item.GetPItem()); the by-value argument is destroyed here
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072D360, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esp+8] ; call GetPItem ; push eax ; lea ecx,[esi+0x2c] ; call SetPItem ; lea ecx,[esp+8] ; call ~CBaseIntelligentPointer ; pop esi ; ret 8
// Takes an 8-byte CBaseIntelligentPointer by value (so the callee destroys it, `ret 8`), reads its
// target through CBaseIntelligentPointer::GetPItem @ 0x00A01B50 and stores it into the member
// pointer at +0x2c through SetPItem @ 0x00A01B90; the argument's destructor @ 0x00A01C10
// runs before returning. The manifest label (`CTCInventoryBase::GetItemDescription`) is
// propagated; the class is named descriptively. One of 14 bodies of this shape.
class CBaseObject;

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

struct CItemOwner_0072d360 {
    unsigned char _pad[0x2c];
    CBaseIntelligentPointer m_item;            // +0x2c
    void SetItem_CItemOwner_0072d360(CBaseIntelligentPointer item);
};

void CItemOwner_0072d360::SetItem_CItemOwner_0072d360(CBaseIntelligentPointer item)
{
    m_item.SetPItem(item.GetPItem());
}