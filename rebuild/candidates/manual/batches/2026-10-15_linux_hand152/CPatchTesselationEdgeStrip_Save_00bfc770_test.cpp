#include <cstdio>
static unsigned int g_log[16]; static int g_n = 0;
struct Stream {
    void WriteBool(bool v) { g_log[g_n++] = 0x100u + (v ? 1u : 0u); }
    void WriteUShort(unsigned short v) { g_log[g_n++] = v; }
};
template <class T> struct Arr { T* Data; long Count; };
struct Strip {
    unsigned short StartPos, Size;
    Arr<int> Verts; Arr<short> Fan; Arr<char> Water; Arr<long> WFan;
    bool Tesselated, Vertical;
    template <class T> void SaveVerts(Arr<T>* v, Stream* s) { g_log[g_n++] = 0x200u + (unsigned int)v->Count; }
    void Save(Stream* s)
    {
        s->WriteUShort(StartPos); s->WriteUShort(Size); s->WriteBool(Vertical);
        SaveVerts(&Verts, s); SaveVerts(&Fan, s); SaveVerts(&Water, s); SaveVerts(&WFan, s);
    }
};
int main()
{
    Strip t; t.StartPos = 7; t.Size = 9; t.Vertical = true;
    t.Verts.Count = 1; t.Fan.Count = 2; t.Water.Count = 3; t.WFan.Count = 4;
    Stream s; t.Save(&s);
    unsigned int want[7] = { 7, 9, 0x101, 0x201, 0x202, 0x203, 0x204 };
    for (int i = 0; i < 7; ++i) if (g_log[i] != want[i]) { std::printf("00bfc770_TEST FAIL\n"); return 1; }
    if (g_n != 7) { std::printf("00bfc770_TEST FAIL\n"); return 1; }
    std::printf("00bfc770_TEST PASS\n");
    return 0;
}
