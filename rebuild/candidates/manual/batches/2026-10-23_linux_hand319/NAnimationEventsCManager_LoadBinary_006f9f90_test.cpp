// Behaviour test for NAnimationEvents::CManager::LoadBinary (0x006F9F90): parse the binary extra-event stream.
#include <cstdio>
#include <cstring>
struct Event { unsigned int Index : 30; unsigned int ExternallyImported : 1; unsigned int StopEvent : 1; float Time; };
struct Anim { unsigned int id; Event* events; int count; };
struct Stream {
    const unsigned char* p; int remaining;
    void Read(void* out, int n) { if (n <= remaining) { std::memcpy(out, p, n); p += n; remaining -= n; } else std::memset(out, 0, n); }
    int ReadSLONG() { int v; Read(&v, 4); return v; }
    unsigned int ReadULONG() { unsigned int v; Read(&v, 4); return v; }
    bool ReadEBOOL() { unsigned char v; Read(&v, 1); return v != 0; }
    float ReadFLOAT() { float v; Read(&v, 4); return v; }
};
static Event g_events[8]; static Anim g_anims[4];
static int Load(Stream& s)
{
    int count = s.ReadSLONG(); int used = 0;
    for (int i = 0; i < count; ++i) {
        g_anims[i].id = s.ReadULONG();
        int n = s.ReadSLONG();
        g_anims[i].events = g_events + used; g_anims[i].count = n; used += n;
        for (int j = 0; j < n; ++j) {
            g_anims[i].events[j].Index = s.ReadULONG();
            g_anims[i].events[j].ExternallyImported = s.ReadEBOOL();
            g_anims[i].events[j].StopEvent = s.ReadEBOOL();
            g_anims[i].events[j].Time = s.ReadFLOAT();
        }
    }
    return count;
}
static unsigned char* Put(unsigned char* p, const void* v, int n) { std::memcpy(p, v, n); return p + n; }
int main()
{
    unsigned char buf[128]; unsigned char* p = buf;
    int two = 2, one = 1, zero = 0; unsigned int a = 77, b = 99, idx = 0x3fffffff, idx2 = 5; float t = 1.5f, t2 = 0.25f;
    unsigned char yes = 7, no = 0;
    p = Put(p, &two, 4);
    p = Put(p, &a, 4); p = Put(p, &one, 4); p = Put(p, &idx, 4); p = Put(p, &yes, 1); p = Put(p, &no, 1); p = Put(p, &t, 4);
    p = Put(p, &b, 4); p = Put(p, &one, 4); p = Put(p, &idx2, 4); p = Put(p, &no, 1); p = Put(p, &yes, 1); p = Put(p, &t2, 4);
    (void)zero;
    Stream s; s.p = buf; s.remaining = (int)(p - buf);
    bool ok = Load(s) == 2;
    ok = ok && g_anims[0].id == 77 && g_anims[0].count == 1 && g_anims[0].events[0].Index == 0x3fffffff
        && g_anims[0].events[0].ExternallyImported == 1 && g_anims[0].events[0].StopEvent == 0 && g_anims[0].events[0].Time == 1.5f;
    ok = ok && g_anims[1].id == 99 && g_anims[1].events[0].Index == 5 && g_anims[1].events[0].ExternallyImported == 0
        && g_anims[1].events[0].StopEvent == 1 && g_anims[1].events[0].Time == 0.25f && s.remaining == 0;
    if (!ok) { std::printf("006f9f90_TEST FAIL\n"); return 1; }
    std::printf("006f9f90_TEST PASS\n");
    return 0;
}
