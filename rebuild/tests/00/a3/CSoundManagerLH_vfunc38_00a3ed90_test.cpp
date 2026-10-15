#include <cstdio>
struct CEntry {
    long r; long a[5]; float f;
    virtual long Play(long sound, long position, long flags, long loop, float fade, long priority)
    { a[0] = sound; a[1] = position; a[2] = flags; a[3] = loop; f = fade; a[4] = priority; return r; }
};
struct CSoundManagerLH {
    CEntry* m_entry; long m_key;
    CEntry* Find(long key) { return key == m_key ? m_entry : 0; }
    virtual bool vfunc_38(long key, long sound, long priority, long position, long flags)
    {
        CEntry* entry = Find(key);
        if (!entry) return false;
        return entry->Play(sound, position, flags, 0, 4.0f, priority) >= 0;
    }
};
int main()
{
    CEntry e; e.r = 0;
    CSoundManagerLH m; m.m_entry = &e; m.m_key = 5;
    bool ok = !m.vfunc_38(4, 1, 2, 3, 4);
    ok = ok && m.vfunc_38(5, 11, 22, 33, 44) && e.a[0] == 11 && e.a[1] == 33 && e.a[2] == 44 && e.a[3] == 0 && e.f == 4.0f && e.a[4] == 22;
    e.r = -1;
    ok = ok && !m.vfunc_38(5, 1, 2, 3, 4);
    if (!ok) { std::printf("00a3ed90_TEST FAIL\n"); return 1; }
    std::printf("00a3ed90_TEST PASS\n");
    return 0;
}