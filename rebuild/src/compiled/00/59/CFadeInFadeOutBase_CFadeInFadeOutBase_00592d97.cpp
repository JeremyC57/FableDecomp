#pragma optimize("y", off)
#pragma optimize("s", on)
// CFadeInFadeOutBase::CFadeInFadeOutBase(const CFadeInFadeOutBase& other) -- the copy constructor of the 28-byte fade record: a byte, two floats, two bytes, two floats, four bytes, copied member by member
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00592D97, length 84 bytes.
// Callees: none. Shape: a frame pointer and the size-optimised module; the first seven members go through dl, the last four bytes are juggled through dl/bl/cl
// with one parked in the dead frame byte `[ebp+0Bh]` (ebx saved around them); `this` is returned, `ret 4`. The four flag bytes are loaded before any of them is
// stored: an inline helper taking the four values as arguments (argument order +0x1a, +0x19, +0x18, +0x1b reproduces the load/spill order and the store order).
struct SFadeFlags_00592d97 {                       // four flag bytes copied through an inline copy constructor
    bool m_a, m_b, m_c, m_d;
    void Set(bool c, bool b, bool a, bool d) { m_c = c; m_b = b; m_a = a; m_d = d; }   // argument order (+0x1a, +0x19, +0x18, +0x1b): the retail load/spill order
};

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
    SFadeFlags_00592d97 m_flags;                   // +0x18 .. +0x1b
    CFadeInFadeOutBase(const CFadeInFadeOutBase& other);
};

CFadeInFadeOutBase::CFadeInFadeOutBase(const CFadeInFadeOutBase& other)
    : m_active(other.m_active)
    , m_fadeOutDuration(other.m_fadeOutDuration), m_fadeInDuration(other.m_fadeInDuration)
    , m_fadingIn(other.m_fadingIn), m_fadingOut(other.m_fadingOut)
    , m_fadeOutElapsed(other.m_fadeOutElapsed), m_fadeInElapsed(other.m_fadeInElapsed)
{
    m_flags.Set(other.m_flags.m_c, other.m_flags.m_b, other.m_flags.m_a, other.m_flags.m_d);
}