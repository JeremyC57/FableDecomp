// Behaviour test for CPSCUpdateNormal::WriteBinary (0x00B0B5C0): records every stream write and checks the order,
// the bit-field extraction and the member values.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CCharString { public: const char* Text; };
struct Rec { char kind; unsigned long u; float f; };
struct Stream {
    Rec recs[64]; int n;
    Stream() : n(0) {}
    void WriteEBOOL(bool v) { recs[n].kind = 'b'; recs[n].u = v ? 1 : 0; n++; }
    void WriteULong(unsigned long v) { recs[n].kind = 'u'; recs[n].u = v; n++; }
    void WriteFloat(float v) { recs[n].kind = 'f'; recs[n].f = v; n++; }
    void Write3DVector(const C3DVector& v) { recs[n].kind = 'v'; recs[n].f = v.X + v.Y + v.Z; n++; }
    void WriteNullTerminatedString(const CCharString& s) { recs[n].kind = 's'; recs[n].u = (unsigned long)s.Text[0]; n++; }
};
struct UpdateNormal {
    unsigned int FadeInEndInteger : 10;
    unsigned int FadeOutBeginInteger : 10;
    unsigned int B20 : 1; unsigned int B21 : 1; unsigned int B22 : 1; unsigned int B23 : 1;
    unsigned int B24 : 1; unsigned int B25 : 1; unsigned int B26 : 1; unsigned int B27 : 1;
    unsigned int B28 : 1; unsigned int B29 : 1; unsigned int B30 : 1; unsigned int B31 : 1;
    bool C0 : 1; bool C1 : 1; bool C2 : 1; bool C3 : 1; bool C4 : 1; bool C5 : 1;
    CCharString Name;
    float F[15];
    C3DVector V[5];
    unsigned int AccelerationParam, OrientFromGameParam;
    void WriteBinary(Stream* out)
    {
        out->WriteULong(FadeInEndInteger); out->WriteULong(FadeOutBeginInteger);
        out->WriteEBOOL(B20); out->WriteEBOOL(B21); out->WriteEBOOL(B22); out->WriteEBOOL(B23);
        out->WriteEBOOL(B24); out->WriteEBOOL(B25); out->WriteEBOOL(B26); out->WriteEBOOL(B27);
        out->WriteEBOOL(B28); out->WriteEBOOL(C5); out->WriteEBOOL(B29); out->WriteEBOOL(B30); out->WriteEBOOL(B31);
        out->WriteEBOOL(C0); out->WriteEBOOL(C1); out->WriteEBOOL(C2); out->WriteEBOOL(C3); out->WriteEBOOL(C4);
        out->WriteNullTerminatedString(Name);
        for (int i = 0; i < 15; ++i) out->WriteFloat(F[i]);
        for (int j = 0; j < 5; ++j) out->Write3DVector(V[j]);
        out->WriteULong(AccelerationParam); out->WriteULong(OrientFromGameParam);
    }
};
int main()
{
    UpdateNormal u;
    u.FadeInEndInteger = 1000; u.FadeOutBeginInteger = 7;
    u.B20 = 1; u.B21 = 0; u.B22 = 1; u.B23 = 0; u.B24 = 1; u.B25 = 0; u.B26 = 1; u.B27 = 0;
    u.B28 = 1; u.B29 = 0; u.B30 = 1; u.B31 = 1;
    u.C0 = true; u.C1 = false; u.C2 = true; u.C3 = false; u.C4 = true; u.C5 = false;
    u.Name.Text = "decal";
    for (int i = 0; i < 15; ++i) u.F[i] = (float)i * 0.5f;
    for (int j = 0; j < 5; ++j) { u.V[j].X = (float)j; u.V[j].Y = 1.0f; u.V[j].Z = 2.0f; }
    u.AccelerationParam = 0x1234; u.OrientFromGameParam = 0x5678;
    Stream s;
    u.WriteBinary(&s);
    static const unsigned long bits[18] = { 1,0,1,0,1,0,1,0,1, 0, 0,1,1, 1,0,1,0,1 };
    bool ok = s.n == 43 && s.recs[0].u == 1000 && s.recs[1].u == 7;
    for (int k = 0; ok && k < 18; ++k) ok = s.recs[2 + k].kind == 'b' && s.recs[2 + k].u == bits[k];
    ok = ok && s.recs[20].kind == 's' && s.recs[20].u == 'd';
    for (int i = 0; ok && i < 15; ++i) ok = s.recs[21 + i].kind == 'f' && s.recs[21 + i].f == (float)i * 0.5f;
    for (int j = 0; ok && j < 5; ++j) ok = s.recs[36 + j].kind == 'v' && s.recs[36 + j].f == (float)j + 3.0f;
    ok = ok && s.recs[41].u == 0x1234 && s.recs[42].u == 0x5678;
    if (!ok) { std::printf("00b0b5c0_TEST FAIL\n"); return 1; }
    std::printf("00b0b5c0_TEST PASS\n");
    return 0;
}