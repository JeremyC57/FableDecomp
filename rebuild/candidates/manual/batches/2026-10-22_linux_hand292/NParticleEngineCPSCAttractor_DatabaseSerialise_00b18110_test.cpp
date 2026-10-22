// Behaviour test for CPSCAttractor::DatabaseSerialise (0x00B18110): a recording persist context checks the save path
// (CRC words transferred directly) and the load path (names read into strings and hashed back), the transfer order,
// the user-points resize to the transferred size and the per-point names (the literal advanced by the index).
#include <cstdio>
#include <cstring>
struct V3 { float X, Y, Z; };
struct Str { const char* data; int length; };
static unsigned int Crc(unsigned int seed, const char* d, int n) { unsigned int h = seed + 7; for (int i = 0; i < n; i++) h = h * 31 + (unsigned char)d[i]; return h; }
static unsigned int GetCRC(const Str& s)
{
    const char* data; int length;
    if (s.data == 0) { length = 0; data = ""; } else { length = s.length; data = s.data; }
    return Crc(0, data, length);
}
struct Ctx {
    int mode; int n; char log[32][40]; int loadCount; const char* loadName;
    bool IsSaving() const { return mode == 1 || mode == 3; }
    void rec(const char* s) { strncpy(log[n], s, 39); log[n][39] = 0; n++; }
    void Transfer(const char* name, bool& v, const bool&) { rec(name); if (!IsSaving()) v = true; }
    void Transfer(const char* name, unsigned int& v, const unsigned int&) { rec(name); (void)v; }
    void Transfer(const char* name, Str& v, const Str&) { rec(name); v.data = loadName; v.length = (int)strlen(loadName); }
    void Transfer(const char* name, float& v, const float&) { rec(name); if (!IsSaving()) v = 2.5f; }
    void Transfer(const char* name, int& v, const int&) { rec(name); if (!IsSaving() && strcmp(name, "AttractorUserPointsArraySize") == 0) v = loadCount; }
    void Transfer(const char* name, V3& v, const V3& d) { rec(name); if (!IsSaving()) v = d; }
};
struct Attractor {
    bool Enabled : 1; bool UseParam : 1; int FallOff; unsigned int Param, ParamName; float Radius, Force;
    V3 points[8]; int count;
    void DatabaseSerialise(Ctx& c)
    {
        { bool e = Enabled; c.Transfer("AttractorEnabled", e, false); Enabled = e; }
        { bool u = UseParam; c.Transfer("AttractorUseParamPosition", u, false); UseParam = u; }
        if (c.IsSaving()) {
            c.Transfer("AttractorPositionParam", Param, 0u);
            c.Transfer("AttractorPositionParamName", ParamName, 0u);
        } else {
            Str name; name.data = 0; name.length = 0; Str empty = name;
            c.Transfer("AttractorPositionParam", name, empty); Param = GetCRC(name);
            c.Transfer("AttractorPositionParamName", name, empty); ParamName = GetCRC(name);
        }
        c.Transfer("AttractorInfluenceRadius", Radius, 0.0f);
        c.Transfer("AttractorInfluenceForce", Force, 0.0f);
        c.Transfer("AttractorInfluenceFallOffType", FallOff, 0);
        int n = count;
        c.Transfer("AttractorUserPointsArraySize", n, 0);
        count = n;
        int i = 0;
        if (i < n) {
            V3 zero = { 0.0f, 0.0f, 0.0f };
            do { c.Transfer("AttractorUserPointsArray" + i, points[i], zero); } while (++i < n);
        }
    }
};
int main()
{
    int fails = 0;
    Attractor a; memset(&a, 0, sizeof(a)); a.count = 2; a.Param = 11; a.ParamName = 12;
    Ctx s; memset(&s, 0, sizeof(s)); s.mode = 3; s.loadName = "";
    a.DatabaseSerialise(s);
    if (s.n != 10) fails++;
    if (strcmp(s.log[2], "AttractorPositionParam") != 0 || strcmp(s.log[7], "AttractorUserPointsArraySize") != 0) fails++;
    if (strcmp(s.log[8], "AttractorUserPointsArray") != 0 || strcmp(s.log[9], "ttractorUserPointsArray") != 0) fails++;
    if (a.Param != 11 || a.ParamName != 12 || a.Enabled) fails++;
    Ctx l; memset(&l, 0, sizeof(l)); l.mode = 2; l.loadCount = 3; l.loadName = "Bone";
    a.points[2].X = 9.0f;
    a.DatabaseSerialise(l);
    if (a.Param != Crc(0, "Bone", 4) || a.ParamName != a.Param) fails++;
    if (!a.Enabled || !a.UseParam || a.Radius != 2.5f || a.count != 3) fails++;
    if (a.points[2].X != 0.0f || strcmp(l.log[10], "tractorUserPointsArray") != 0 || l.n != 11) fails++;
    Ctx z; memset(&z, 0, sizeof(z)); z.mode = 0; z.loadCount = 0; z.loadName = "";
    a.DatabaseSerialise(z);
    if (z.n != 8 || a.Param != Crc(0, "", 0)) fails++;
    if (fails == 0) printf("00b18110_TEST PASS\n"); else printf("00b18110_TEST FAIL %d\n", fails);
    return fails;
}
