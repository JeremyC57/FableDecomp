// behaviour test for 0x00705C30 CTCCarryable::vfunc_6 (def offset copy + normalise + release)
#include <cstdio>
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    C3DVector& Normalise() { float l = std::sqrt(GetLengthSquared()); X /= l; Y /= l; Z /= l; return *this; }
};
static const float g_eps = 0.0001f;
static int g_deleted = 0;
struct Def { long refs; C3DVector Offset; void Delete() { ++g_deleted; } };
struct ThingDef { Def* sub; bool GetSubDef(Def** out) { if (!sub) return false; ++sub->refs; *out = sub; return true; } };
struct Carryable {
    ThingDef* parentDef; C3DVector Offset;
    void vfunc_6()
    {
        ThingDef* thingDef = parentDef;
        Def* out = 0;
        bool found = thingDef->GetSubDef(&out);
        Def* def = out;
        if (found) {
            Offset = def->Offset;
            if (Offset.GetLengthSquared() > g_eps * g_eps) Offset.Normalise();
        }
        if (def && --def->refs == 0) def->Delete();
    }
};
int main()
{
    Def d; d.refs = 0; d.Offset.X = 3; d.Offset.Y = 0; d.Offset.Z = 4;
    ThingDef td; td.sub = &d;
    Carryable c; c.parentDef = &td; c.Offset.X = c.Offset.Y = c.Offset.Z = 9;
    c.vfunc_6();
    bool ok = std::fabs(c.Offset.X - 0.6f) < 1e-5f && std::fabs(c.Offset.Z - 0.8f) < 1e-5f && g_deleted == 1 && d.refs == 0;
    d.Offset.X = d.Offset.Y = d.Offset.Z = 0; d.refs = 1;
    c.vfunc_6();
    ok = ok && c.Offset.X == 0 && c.Offset.Z == 0 && g_deleted == 1 && d.refs == 1;
    td.sub = 0; c.Offset.X = 5;
    c.vfunc_6();
    ok = ok && c.Offset.X == 5;
    if (!ok) { std::printf("00705c30_TEST FAIL\n"); return 1; }
    std::printf("00705c30_TEST PASS\n");
    return 0;
}