// CKrakenHolder_00c75440::~CKrakenHolder_00c75440() -- destructor of the 0x24-byte holder at CTCKraken+0x25C: clear both elements' two words, then destroy the two-element array
// Leaf: ??1CKrakenHolder_00c75440
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75440, 39 bytes (+ int3 padding; the 48-byte catalog row includes 9 of it).
// Label: the catalog name "CPersistTraits<NInventory::CItem>::TransferBinaryIn" is propagated and wrong. The landed
//   CTCKraken ctor 0x00C6A650 builds this member with the ctor 0x00C75410 (vtable 0x012BF078, then
//   `eh vector constructor iterator` over two 0x10-byte elements at +4 with ctor 0x00C74F60 / dtor 0x00C75400) and
//   names 0x00C75410 / 0x00C75440 its ctor / dtor; 0x00C754A0 is the scalar deleting destructor calling this body.
// Callees: `eh vector destructor iterator' @ 0x00BFEED8 (ptr, 0x10, 2, element dtor 0x00C75400).
// Layout: holder = vptr + two polymorphic 0x10-byte elements (vptr 0x012BF074, two words +4/+8 the element ctor and
//   dtor zero, a third word +0xC). Classes named by address, as in the landed Kraken models.
// Flags: /EHsc (the array teardown goes through the `eh vector destructor iterator`).
// Shape: the user dtor body clears both elements inline (the element's inline Clear(); its stores interleave with the
//   iterator's argument pushes); the vptr store is the holder's own, the array teardown is the implicit member dtor.
struct CKrakenElement_00c75400 {                     // vtable 0x012BF074
    virtual ~CKrakenElement_00c75400();              // 0x00C75400
    long m_first;                                    // +0x04
    long m_second;                                   // +0x08
    long m_third;                                    // +0x0C
    CKrakenElement_00c75400();                       // 0x00C74F60
    void Clear() { m_first = 0; m_second = 0; }
};

struct CKrakenHolder_00c75440 {                      // vtable 0x012BF078
    virtual ~CKrakenHolder_00c75440();
    CKrakenElement_00c75400 m_elements[2];           // +0x04
};

CKrakenHolder_00c75440::~CKrakenHolder_00c75440()
{
    m_elements[0].Clear();
    m_elements[1].Clear();
}