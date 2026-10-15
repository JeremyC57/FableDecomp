// CVtable_012b5c60::GetDataSize() -- vtable slot 13: rows * row size, plus the owner descriptor's extra size when the descriptor's key differs from ours; 0 without an owner
// Leaf: GetDataSize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F510, length 62 bytes.
// Callees: 0x00C3C4A0 (landed; returns its object + 0x10, here the owner's embedded descriptor), called once
// for the key compare and again for the extra size.
// Identity: the manifest label CPlayer::GlobalEventCreatureDied is wrong (BSim propagation); the body is slot
// 13 of the no-RTTI vtables 0x012B5C60 / 0x012B5CD0 / 0x012B5D70 / 0x012B8AE0 (data refs at 0x012B5C94..),
// a family of 0x00C5xxxx-module classes. Names are descriptive (vtable-addressed class, member roles).
// Shape: the null test is the outer `if (owner) { ... return size; } return 0;` (the null arm sits at the end);
// the key and the product are taken into locals before the first call, so ebx/edi hold them across it.
struct CDescriptor {
    unsigned char _pad00[0x10];
    long          m_extraSize;                              // +0x10
    unsigned char _pad14[4];
    long          m_key;                                    // +0x18
};

struct COwner {
    unsigned char _pad00[0x10];
    CDescriptor   m_descriptor;                             // +0x10
    CDescriptor*  GetDescriptor();                          // 0x00C3C4A0 (returns this + 0x10)
};

struct CVtable_012b5c60 {
    virtual void v00();
    unsigned char _pad04[8];
    COwner*       m_pOwner;                                 // +0x0c
    unsigned char _pad10[0xa8];
    long          m_key;                                    // +0xb8
    unsigned char _padbc[4];
    long          m_rowSize;                                // +0xc0
    unsigned char _padc4[4];
    long          m_rowCount;                               // +0xc8
    long GetDataSize();
};

long CVtable_012b5c60::GetDataSize()
{
    if (m_pOwner) {
        long key = m_key;
        long size = m_rowCount * m_rowSize;
        if (key != m_pOwner->GetDescriptor()->m_key)
            size += m_pOwner->GetDescriptor()->m_extraSize;
        return size;
    }
    return 0;
}