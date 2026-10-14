// CVtable_012c1648::CVtable_012c1648() -- CBase (0x0099A2F0), this class's vtable (0x012C1648, no RTTI), the circular list at +4 (its 16-byte sentinel from malloc @ 0x00BFEA0E, linked to itself), then the two 16-byte morph records at +8 and +0x18, the four words at +0x2c..+0x38 and the flag byte at +0x3d cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB8110, length 96 bytes.
// The chunk label "CHeroMorphDef::CHeroMorphDef" is propagated; named by its vtable. Shape: ebx zero, the list pointer cleared then set, the `add esp,4` of malloc delayed to the end.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};
struct SListNode_00cb8110 { SListNode_00cb8110* m_pNext; SListNode_00cb8110* m_pPrev; unsigned char _payload[8]; };   // 16 bytes
struct CList_00cb8110 {                            // a circular list: one sentinel node
    SListNode_00cb8110* m_pNode;
    CList_00cb8110()
    {
        m_pNode = 0;
        SListNode_00cb8110* node = (SListNode_00cb8110*)malloc(0x10);
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pNode = node;
    }
};
struct SMorphRecord_00cb8110 {                     // 16 bytes
    long          m_a;
    long          m_b;
    long          m_c;
    unsigned char _padc;
    bool          m_flag;                          // +0x0d
    unsigned char _pade[2];
    SMorphRecord_00cb8110() : m_a(0), m_b(0), m_c(0), m_flag(false) { }
};
struct SRecord16_00cb8110 {                        // 16 bytes at +0x2c: four words cleared
    long m_a, m_b, m_c, m_d;
    SRecord16_00cb8110() : m_a(0), m_b(0), m_c(0), m_d(0) { }
};
struct CVtable_012c1648 : CBase {                  // vtable 0x012C1648
    CList_00cb8110        m_list;                  // +0x04
    SMorphRecord_00cb8110 m_morph08;               // +0x08
    SMorphRecord_00cb8110 m_morph18;               // +0x18
    long                  m_28;                    // +0x28 (untouched)
    SRecord16_00cb8110    m_record2c;              // +0x2c
    unsigned char         _pad3c;
    bool                  m_3d;                    // +0x3d
    virtual void v00();
    CVtable_012c1648();
};

CVtable_012c1648::CVtable_012c1648()
    : CBase()
{
    m_3d = false;                                  // after the member constructors: malloc's stack cleanup lands between
}
