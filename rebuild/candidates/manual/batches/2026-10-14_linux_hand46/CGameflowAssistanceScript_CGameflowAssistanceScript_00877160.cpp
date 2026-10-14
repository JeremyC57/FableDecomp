// CGameflowAssistanceScript::CGameflowAssistanceScript(void* owner) -- store the owner, clear the three words at +4..+0xc and set the four tuning floats (0.12, 0.03, 0.2, 0.01)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00877160, length 64 bytes.
// Callees: none. Shape: `this` moved to eax, one zero register for the three clears, the floats stored as immediates, `ret 4`.
struct CGameflowAssistanceScript {
    void* m_pOwner;                                // +0x00
    long  m_state;                                 // +0x04
    long  m_timer;                                 // +0x08
    long  m_count;                                 // +0x0c
    float m_rateA;                                 // +0x10
    float m_rateB;                                 // +0x14
    float m_rateC;                                 // +0x18
    float m_rateD;                                 // +0x1c
    CGameflowAssistanceScript(void* owner);     // `mov eax,ecx` at the top: a constructor (returns `this`), not an Init
};

CGameflowAssistanceScript::CGameflowAssistanceScript(void* owner)
{
    m_pOwner = owner;
    m_state = 0;
    m_timer = 0;
    m_count = 0;
    m_rateA = 0.12f;
    m_rateB = 0.03f;
    m_rateC = 0.2f;
    m_rateD = 0.01f;
}
