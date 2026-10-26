// CEngine::SetScreenColourFilter(const SScreenColourFilter&) -- copy the 32-byte filter record into the screen-effect state at 0x01436E40 unless its lock byte at +0xc is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23EC0, length 35 bytes.
// Callees: none. Shape: a 32-byte POD assignment is `mov ecx,8 ; rep movsd` (esi = the argument, edi = state+0x10); the lock byte is tested first
// and the registers are only saved on the copy path. A non-static member whose `this` is unused (no `this`, `ret 4`).
struct SScreenColourFilter {
    float m_values[8];                             // 32 bytes
};

struct SScreenEffectState_01436e40 {
    unsigned char       _pad[0x0c];
    unsigned char       m_filterLocked;            // +0x0c
    unsigned char       _pad0d[3];
    SScreenColourFilter m_filter;                  // +0x10
};
extern SScreenEffectState_01436e40* g_pScreenEffectState_01436e40;   // 0x01436E40

class CEngine { public:
    void SetScreenColourFilter(const SScreenColourFilter& filter);   // a member (`ret 4`), `this` unused
};

void CEngine::SetScreenColourFilter(const SScreenColourFilter& filter)
{
    if (!g_pScreenEffectState_01436e40->m_filterLocked)
        g_pScreenEffectState_01436e40->m_filter = filter;
}