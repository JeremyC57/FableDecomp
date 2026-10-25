// Behaviour test for 0x00A3ED90 CSoundManagerLH::vfunc_38 (links the source object): the name reference reaches FindByName
// unchanged; absent -> false, else slot 6 gets (sound, position, flags, 0, 4.0f, priority) and the result is `>= 0`.
#include <cstdio>
struct CSoundBankEntry_00a3ed90 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long Play(long sound, long position, long flags, long loop, float fade, long priority);
    long r; long a[5]; float f;
};
void CSoundBankEntry_00a3ed90::v00() { } void CSoundBankEntry_00a3ed90::v01() { } void CSoundBankEntry_00a3ed90::v02() { }
void CSoundBankEntry_00a3ed90::v03() { } void CSoundBankEntry_00a3ed90::v04() { } void CSoundBankEntry_00a3ed90::v05() { }
long CSoundBankEntry_00a3ed90::Play(long sound, long position, long flags, long loop, float fade, long priority)
{ a[0] = sound; a[1] = position; a[2] = flags; a[3] = loop; f = fade; a[4] = priority; return r; }
class CCharString { public: void* PStringData; };
struct CSoundManagerLH {
    virtual void v00();
    CSoundBankEntry_00a3ed90* FindByName(const CCharString& name);
    virtual bool vfunc_38(const CCharString& name, long sound, long priority, long position, long flags);
};
static CSoundBankEntry_00a3ed90* g_entry = 0;
static const CCharString* g_known = 0;
static const CCharString* g_asked = 0;
void CSoundManagerLH::v00() { }
CSoundBankEntry_00a3ed90* CSoundManagerLH::FindByName(const CCharString& name) { g_asked = &name; return &name == g_known ? g_entry : 0; }
int main()
{
    CSoundBankEntry_00a3ed90 e; e.r = 0;
    CSoundManagerLH m; CCharString known, other; known.PStringData = other.PStringData = 0;
    g_entry = &e; g_known = &known;
    bool ok = !m.vfunc_38(other, 1, 2, 3, 4) && g_asked == &other;
    ok = ok && m.vfunc_38(known, 11, 22, 33, 44) && g_asked == &known && e.a[0] == 11 && e.a[1] == 33 && e.a[2] == 44 && e.a[3] == 0 && e.f == 4.0f && e.a[4] == 22;
    e.r = -1;
    ok = ok && !m.vfunc_38(known, 1, 2, 3, 4);
    if (!ok) { std::printf("00a3ed90_TEST FAIL\n"); return 1; }
    std::printf("00a3ed90_TEST PASS\n");
    return 0;
}
