#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SFadeFlags_00592d97 { bool m_a, m_b, m_c, m_d; void Set(bool c, bool b, bool a, bool d) { m_c = c; m_b = b; m_a = a; m_d = d; } };
struct CFadeInFadeOutBase { bool m_active; unsigned char _pad01[3]; float m_fadeOutDuration; float m_fadeInDuration; bool m_fadingIn; bool m_fadingOut; unsigned char _pad0e[2]; float m_fadeOutElapsed; float m_fadeInElapsed; SFadeFlags_00592d97 m_flags;
    CFadeInFadeOutBase() { }
    CFadeInFadeOutBase(const CFadeInFadeOutBase& o) : m_active(o.m_active), m_fadeOutDuration(o.m_fadeOutDuration), m_fadeInDuration(o.m_fadeInDuration), m_fadingIn(o.m_fadingIn), m_fadingOut(o.m_fadingOut), m_fadeOutElapsed(o.m_fadeOutElapsed), m_fadeInElapsed(o.m_fadeInElapsed)
    { m_flags.Set(o.m_flags.m_c, o.m_flags.m_b, o.m_flags.m_a, o.m_flags.m_d); } };
int main() {
    CFadeInFadeOutBase a; std::memset(&a, 0, sizeof a); a.m_active = true; a.m_fadeOutDuration = 1.0f; a.m_fadeInDuration = 2.0f; a.m_fadingIn = true; a.m_fadeOutElapsed = 3.0f; a.m_fadeInElapsed = 4.0f; a.m_flags.m_a = true; a.m_flags.m_c = true;
    CFadeInFadeOutBase b(a); if (!b.m_active || b.m_fadeOutDuration != 1.0f || b.m_fadeInDuration != 2.0f || !b.m_fadingIn || b.m_fadingOut || b.m_fadeOutElapsed != 3.0f || b.m_fadeInElapsed != 4.0f || !b.m_flags.m_a || b.m_flags.m_b || !b.m_flags.m_c || b.m_flags.m_d) { std::printf("00592d97_TEST FAIL\n"); return 1; }
    std::printf("00592d97_TEST PASS\n"); return 0;
}
