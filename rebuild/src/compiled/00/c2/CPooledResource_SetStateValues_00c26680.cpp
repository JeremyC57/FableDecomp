// CPooledResource::SetStateValues(long unused, unsigned int a, unsigned int b) -- set the two flagged values of the state block at +0x24 (0x00C20480 then 0x00C204A0)
// Leaf: ?SetStateValues@CPooledResource@@UAEXJII@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26680, 32 bytes, thiscall, `ret 0Ch`.
// Identity (descriptive): slot 6 of the non-RTTI vtable 0x012B3FDC (see 0x00C26650); libpk has no PDB or RTTI.
// Callees: State::Set2_00c20480(unsigned int) @ 0x00C20480 and State::Set3_00c204a0(unsigned int) @ 0x00C204A0 (the
//   existing reloc-binding spellings: each stores through the block's +8 member and sets dirty bit 4 / 8).
// Shape: the member's address is formed once into esi (`lea esi,[ecx+24h]`) and reused for both calls.
struct State {
    void Set2_00c20480(unsigned int value);     // 0x00C20480
    void Set3_00c204a0(unsigned int value);     // 0x00C204A0
};

struct CPooledResource {                        // vtable 0x012B3FDC
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual void SetStateValues(long unused, unsigned int a, unsigned int b);   // slot 6
    long          m_handle;                     // +0x04
    unsigned char _pad08[0x24 - 0x08];
    State         m_state24;                    // +0x24
};

void CPooledResource::SetStateValues(long unused, unsigned int a, unsigned int b)
{
    m_state24.Set2_00c20480(a);
    m_state24.Set3_00c204a0(b);
}