#include <cstdio>
// Behaviour test for 006d4ce0: IS_THING_DEAD -- null pointer or dead flag (bit 0 of +0x91) set.
struct CThing { unsigned char pad[0x91]; unsigned char m_flags; };
template <class T> bool IS_THING_DEAD(T const& thing)
{
    if (thing != 0 && !(thing->m_flags & 1)) return false;
    return true;
}
int main()
{
    CThing t; t.m_flags = 0;
    CThing* p = 0;
    int ok = IS_THING_DEAD(p);
    p = &t; ok = ok && !IS_THING_DEAD(p);
    t.m_flags = 1; ok = ok && IS_THING_DEAD(p);
    t.m_flags = 2; ok = ok && !IS_THING_DEAD(p);
    if (!ok) { std::printf("006d4ce0_TEST FAIL\n"); return 1; }
    std::printf("006d4ce0_TEST PASS\n");
    return 0;
}
