// GetDefaults_0139c988(SBlock160& out) const -- copy the 160-byte default block at 0x0139C988 (40 dwords, rep movsd) into the caller's structure; `this` is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C33400, length 23 bytes.
// Callees: (none)
// Propagated manifest label (C3DMeshFileBoneChunk::SetLocalizationMatrix) rejected: ecx is never read, the one
// stack argument is the destination (ret 4). Modelled as a const member returning the class defaults.
//
#include "rebuild_abi.h"
struct SBlock160 {
    unsigned long m_words[40];                              // 160 bytes, copied as 0x28 dwords
};
extern SBlock160 g_defaults_0139c988;                       // 0x0139C988 (starts ff ff ff ff ff ff ff ff 17 20 00 00 03 00 00 00)
struct CDefaultsProvider {
    void GetDefaults_0139c988(SBlock160& out) const;
};
void CDefaultsProvider::GetDefaults_0139c988(SBlock160& out) const
{
    out = g_defaults_0139c988;
}
SBlock160 g_defaults_0139c988;