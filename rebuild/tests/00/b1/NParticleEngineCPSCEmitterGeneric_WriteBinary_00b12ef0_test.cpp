// Behaviour test for CPSCEmitterGeneric::WriteBinary (0x00B12EF0): the bit-field writes and the optional spline
// block (flag, tension, count, points from a copy of the control points).
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct PointVector {
    C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
    PointVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    PointVector(const PointVector& o)
    {
        unsigned long n = (unsigned long)(o._M_finish - o._M_start);
        _M_start = n ? (C3DVector*)malloc(n * sizeof(C3DVector)) : 0;
        if (n) memcpy(_M_start, o._M_start, n * sizeof(C3DVector));
        _M_finish = _M_start + n; _M_end_of_storage = _M_finish;
    }
    ~PointVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
struct Spline { PointVector ControlPoints; float Tension; };
struct Stream {
    char kind[64]; double val[64]; int n;
    Stream() : n(0) {}
    void put(char k, double v) { kind[n] = k; val[n] = v; n++; }
    void WriteEBOOL(bool v) { put('b', v ? 1 : 0); }
    void WriteULong(unsigned long v) { put('u', (double)v); }
    void WriteFloat(float v) { put('f', v); }
    void Write3DVector(const C3DVector& v) { put('v', v.X * 100 + v.Y * 10 + v.Z); }
};
struct Emitter {
    Spline* SplineData;
    unsigned int EmitterPosParam, DirectionParamName;
    unsigned int NoParticlesToStart : 10; unsigned int NoParticlesToStartRand : 10; unsigned int EmitterType : 2;
    unsigned int Solid : 1;
    void WriteBinary(Stream* out)
    {
        out->WriteULong(EmitterPosParam); out->WriteULong(DirectionParamName);
        out->WriteULong(NoParticlesToStart); out->WriteULong(NoParticlesToStartRand); out->WriteULong(EmitterType);
        out->WriteEBOOL(Solid);
        bool hasSpline = SplineData != 0;
        out->WriteEBOOL(hasSpline);
        if (hasSpline) {
            out->WriteFloat(SplineData->Tension);
            unsigned long count = SplineData->ControlPoints.size();
            out->WriteULong(count);
            PointVector points(SplineData->ControlPoints);
            for (unsigned long i = 0; i < count; ++i) out->Write3DVector(points._M_start[i]);
        }
    }
};
int main()
{
    Emitter e;
    e.SplineData = 0; e.EmitterPosParam = 4; e.DirectionParamName = 9;
    e.NoParticlesToStart = 1023; e.NoParticlesToStartRand = 5; e.EmitterType = 3; e.Solid = 1;
    Stream a;
    e.WriteBinary(&a);
    bool ok = a.n == 7 && a.val[0] == 4 && a.val[2] == 1023 && a.val[4] == 3 && a.val[5] == 1 && a.val[6] == 0;
    Spline sp;
    C3DVector pts[2] = { { 1, 2, 3 }, { 4, 5, 6 } };
    sp.ControlPoints._M_start = pts; sp.ControlPoints._M_finish = pts + 2; sp.ControlPoints._M_end_of_storage = pts + 2;
    sp.Tension = 0.5f;
    e.SplineData = &sp;
    Stream b;
    e.WriteBinary(&b);
    ok = ok && b.n == 11 && b.val[6] == 1 && b.kind[7] == 'f' && b.val[7] == 0.5 && b.val[8] == 2
        && b.kind[9] == 'v' && b.val[9] == 123 && b.val[10] == 456;
    sp.ControlPoints._M_start = 0; sp.ControlPoints._M_finish = 0;
    if (!ok) { std::printf("00b12ef0_TEST FAIL\n"); return 1; }
    std::printf("00b12ef0_TEST PASS\n");
    return 0;
}