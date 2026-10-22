// CLocalDetailObjectCollectionType::IsEquivalent(const CLocalDetailObjectCollectionType& other) -- true when the three dwords at +0x14 / +0x18 / +0x1c and the 13-dword block at +0x20 are all equal (the block through the intrinsic memcmp, `repe cmpsd`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE2680, length 64 bytes.
// Leaf: IsEquivalent
// Shape: `this` copied to eax, the compares in order with one shared false exit, `ret 4`.
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)
struct SCollectionBlock_00be2680 {                 // 0x34 bytes of plain data at +0x20
    long m_words[13];
};

struct CLocalDetailObjectCollectionType {
    unsigned char             _pad00[0x14];
    long                      MeshBankIndex;                // +0x14
    long                      m_18;                // +0x18
    long                      m_1c;                // +0x1c
    SCollectionBlock_00be2680 m_block;             // +0x20
    bool IsEquivalent(const CLocalDetailObjectCollectionType& other);
};

bool CLocalDetailObjectCollectionType::IsEquivalent(const CLocalDetailObjectCollectionType& other)
{
    if (MeshBankIndex == other.MeshBankIndex && m_18 == other.m_18 && m_1c == other.m_1c && memcmp(&m_block, &other.m_block, sizeof(m_block)) == 0)
        return true;
    return false;
}