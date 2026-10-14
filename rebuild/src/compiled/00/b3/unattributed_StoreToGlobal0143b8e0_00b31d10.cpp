// StoreToGlobal_0143b8e0() const -- copy this 112-byte block (28 dwords, rep movsd) into the global at 0x0143B8E0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B31D10, length 19 bytes.
// Callees: (none)
// Propagated manifest label (C3DMeshFileBoneChunk::SetLocalizationMatrix) rejected: the body is a plain
// struct assignment from `this` to a global; the class is modelled as the 112-byte POD it copies.
//
#include "rebuild_abi.h"
struct SBlock112 {
    unsigned long m_words[28];                              // 112 bytes, copied as 0x1c dwords
};
extern SBlock112 g_block_0143b8e0;                          // 0x0143B8E0
struct CBlock112 : SBlock112 {
    void StoreToGlobal_0143b8e0() const;
};
void CBlock112::StoreToGlobal_0143b8e0() const
{
    g_block_0143b8e0 = *this;
}
SBlock112 g_block_0143b8e0;