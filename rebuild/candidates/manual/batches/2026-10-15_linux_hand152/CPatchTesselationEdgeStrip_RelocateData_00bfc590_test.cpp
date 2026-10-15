#include <cstdio>
template <class T>
struct Arr {
    T* Data; long Count;
    bool RelocateData(void* oldBase, void* newBase)
    {
        T* data = Data;
        if (oldBase != (void*)data) return false;
        if (data) Data = (T*)((char*)data - (char*)oldBase + (long)newBase);
        return true;
    }
};
struct Strip {
    Arr<int> Verts; Arr<short> Fan; Arr<char> Water; Arr<long> WFan;
    bool RelocateData(void* o, void* n)
    {
        if (Verts.RelocateData(o, n)) return true;
        if (Fan.RelocateData(o, n)) return true;
        if (Water.RelocateData(o, n)) return true;
        if (WFan.RelocateData(o, n)) return true;
        return false;
    }
};
int main()
{
    static char a[64], b[64];
    Strip s;
    s.Verts.Data = (int*)(a + 0); s.Fan.Data = (short*)(a + 8); s.Water.Data = (char*)(a + 16); s.WFan.Data = (long*)(a + 32);
    int ok = 1;
    if (!s.RelocateData(a + 16, b + 16)) ok = 0;
    if ((char*)s.Water.Data != b + 16) ok = 0;
    if ((char*)s.Verts.Data != a) ok = 0;
    if (s.RelocateData(a + 40, b)) ok = 0;
    s.Fan.Data = 0;
    if (!s.RelocateData(0, b)) ok = 0;     /* null array owns a null block: stays null */
    if (s.Fan.Data != 0) ok = 0;
    if (!ok) { std::printf("00bfc590_TEST FAIL\n"); return 1; }
    std::printf("00bfc590_TEST PASS\n");
    return 0;
}
