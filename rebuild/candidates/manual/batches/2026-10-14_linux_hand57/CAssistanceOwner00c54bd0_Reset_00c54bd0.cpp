// CAssistanceOwner_00c54bd0::Reset() -- re-initialise the assistance script member at +0xb4 (0x00C5CE40) and clear the dword at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C54BD0, length 23 bytes.
// Leaf: Reset
// Callees: 0x00C5CE40 (labelled CGameflowAssistanceScript::Init, thiscall on the member). The chunk label (CMemoryDataOutputStream::Clear) is propagated; named by address. Shape: `this` in esi, `ret`.
struct CGameflowAssistanceScript {
    void Init();                                   // 0x00C5CE40
};

struct CAssistanceOwner_00c54bd0 {
    unsigned char              _pad00[0xc];
    long                       m_c;                // +0x0c
    unsigned char              _pad10[0xa4];
    CGameflowAssistanceScript  m_script;           // +0xb4
    void Reset();
};

void CAssistanceOwner_00c54bd0::Reset()
{
    m_script.Init();
    m_c = 0;
}
