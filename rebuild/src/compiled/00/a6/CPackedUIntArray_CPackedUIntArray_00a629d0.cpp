// CPackedUIntArray::Clear @ 00a629d0
// Leaf: ?Clear@CPackedUIntArray@@QAEXXZ
// Name fix (NF380): was ~CPackedUIntArray. Retail has no ICF (/DEBUG link): the identical body at 0x00A62AC0 is the destructor
//   (table witness 0x009CD740 binds Clear here, 0x009CE180 binds ~CPackedUIntArray at 0x00A62AC0); Ego_r order Clear < ~ = retail order.
// Layout: [+0] unsigned int* buffer, [+4] u32, [+8] bool, [+0xc] u32

void operator delete[](void*);

struct CPackedUIntArray {
    unsigned int* m_buffer;   // +0
    unsigned int  m_count;    // +4
    bool          m_flag;     // +8
    unsigned int  m_capacity; // +0xc
    void Clear();
};

void CPackedUIntArray::Clear()
{
    operator delete[](m_buffer);
    m_buffer = 0;
    m_count = 0;
    m_flag = false;
    m_capacity = 0;
}