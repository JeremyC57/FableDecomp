// CFadeInFadeOutBase::GetFadeInOutAlpha() -- while fading out (+0xd): 1.0 when the duration (+4) is below the epsilon, else the elapsed (+0x10) over the duration; while fading in (+0xc): likewise with +8 / +0x14; otherwise 1.0 when active (+0) and 0.0 when inactive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004348D0, length 79 bytes.
// Callees: none. Constants: the epsilon @ 0x0129BA3C (1e-4f), 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC. Shape: the flag bytes through al, each duration compared with
// `fcomp ; fnstsw ; test ah,5 ; jnp` (below the epsilon: the 1.0 return), the quotient left in st0 by `fdiv`; the two constants are loaded with `fld`.
extern const float g_fadeEpsilon_0129ba3c;         // 0x0129BA3C (1e-4f)

struct CFadeInFadeOutBase {
    bool  m_active;                                // +0x00
    unsigned char _pad01[3];
    float m_fadeOutDuration;                       // +0x04
    float m_fadeInDuration;                        // +0x08
    bool  m_fadingIn;                              // +0x0c
    bool  m_fadingOut;                             // +0x0d
    unsigned char _pad0e[2];
    float m_fadeOutElapsed;                        // +0x10
    float m_fadeInElapsed;                         // +0x14
    float GetFadeInOutAlpha();
};

float CFadeInFadeOutBase::GetFadeInOutAlpha()
{
    if (m_fadingOut) {
        if (m_fadeOutDuration < g_fadeEpsilon_0129ba3c)
            return 1.0f;
        return m_fadeOutElapsed / m_fadeOutDuration;
    }
    if (m_fadingIn) {
        if (m_fadeInDuration < g_fadeEpsilon_0129ba3c)
            return 1.0f;
        return m_fadeInElapsed / m_fadeInDuration;
    }
    if (m_active)
        return 1.0f;
    return 0.0f;
}
