// CThingFilter_HasDefGlobalIndex::operator()(const CThing& thing) const -- whether the thing's definition global index equals the filter's
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A70B0, length 21 bytes.
// Leaf: ??RCThingFilter_HasDefGlobalIndex
// Callees: none.
// Layout (local model): CThing's definition global index (unsigned short) at +0x8c; the filter's index (long) at +0.
// Shape: Ego_r.pdb `??RCThingFilter_HasDefGlobalIndex@@QBE_NABVCThing@@@Z`. `return index == m_index;` with the zero-extended
// word compared against the filter's long: `movzx ; sub eax,[ecx] ; neg ; sbb eax,eax ; inc eax`.
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short m_defGlobalIndex;            // +0x8c
};

struct CThingFilter_HasDefGlobalIndex {
    long m_defGlobalIndex;                      // +0x00
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_HasDefGlobalIndex::operator()(const CThing& thing) const
{
    return thing.m_defGlobalIndex == m_defGlobalIndex;
}