// behaviour test for 0x0075D140 CTCCameraPointFixedPoint persist (TrackThing + LookVector)
#include <cstdio>
#include <cstring>
class CPersistContext { public:
    int n; const char* names[8]; float fdef[8]; bool bdef;
    bool loading;
    void Transfer(const char* name, float& v, const float& d) { names[n++] = name; fdef[n - 1] = d; if (loading) v = d; }
    void Transfer(const char* name, bool& v, const bool& d) { names[n++] = name; bdef = d; if (loading) v = d; }
};
class C3DVector { public: float X, Y, Z; };
struct FP {
    bool TrackThing; C3DVector LookVector;
    void vfunc_33(CPersistContext& context)
    {
        context.Transfer("TrackThing", TrackThing, true);
        context.Transfer("LookVector.X", LookVector.X, 0.0f);
        context.Transfer("LookVector.Y", LookVector.Y, 0.0f);
        context.Transfer("LookVector.Z", LookVector.Z, 0.0f);
    }
};
int main()
{
    CPersistContext c; c.n = 0; c.loading = true;
    FP f; f.TrackThing = false; f.LookVector.X = f.LookVector.Y = f.LookVector.Z = 5;
    f.vfunc_33(c);
    bool ok = c.n == 4 && f.TrackThing && f.LookVector.Y == 0 && c.bdef
        && !std::strcmp(c.names[0], "TrackThing") && !std::strcmp(c.names[3], "LookVector.Z") && c.fdef[2] == 0.0f;
    if (!ok) { std::printf("0075d140_TEST FAIL\n"); return 1; }
    std::printf("0075d140_TEST PASS\n");
    return 0;
}