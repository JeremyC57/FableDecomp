// Behaviour test for retail 0x00b17f70: the candidate source below is embedded verbatim (from src/00b17f70.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// NParticleEngine::CPSCAttractor::WriteBinary(CDataOutputStream*) -- serialize the attractor settings and its user points
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B17F70. Callees (lane names): WriteEBOOL 0x00993E30, WriteLong(long) 0x00993EB0,
//   WriteULong(unsigned long) 0x00993EA0 (propagated label "WriteCBYTE"), WriteFloat 0x00993EE0,
//   Write3DVector(const C3DVector&) 0x00993FD0.
// Layout: PDB (Ego_r) CPSCAttractor names/types (retail offsets equal the PDB): flag bits +0x0c, AttractorInfluenceFallOff
//   (long) +0x10, AttractorPositionParam / AttractorPositionParamName (ulong) +0x14/+0x18, radius/force +0x1c/+0x20,
//   AttractorUserPointsArray (CArray<C3DVector>: begin/end/capacity) +0x24. Local model: engine/CDataOutputStream.h is
//   data-only; CDataOutputStream_Methods is the lane's method carrier.
#include "engine/CDataOutputStream.h"
class C3DVector { public: float X, Y, Z; };
struct CDataOutputStream_Methods : CDataOutputStream {
    void WriteEBOOL(bool value);                        // 0x00993E30
    void WriteLong(long value);                         // 0x00993EB0
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteFloat(float value);                       // 0x00993EE0
    void Write3DVector(const C3DVector& value);         // 0x00993FD0
};
template<class T> class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
namespace NParticleEngine {
class CPSCAttractor { public:
    unsigned char _base[0xc];                           // CPSCBase
    bool AttractorEnabled : 1;                          // +0x0c
    bool AttractorUseParamPosition : 1;
    long AttractorInfluenceFallOff;                     // +0x10
    unsigned long AttractorPositionParam;               // +0x14
    unsigned long AttractorPositionParamName;           // +0x18
    float AttractorInfluenceRadius;                     // +0x1c
    float AttractorInfluenceForce;                      // +0x20
    CArray<C3DVector> AttractorUserPointsArray;         // +0x24
    void WriteBinary(CDataOutputStream* out);
};
void CPSCAttractor::WriteBinary(CDataOutputStream* destination)
{
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)destination;
    out->WriteEBOOL(AttractorEnabled);
    out->WriteEBOOL(AttractorUseParamPosition);
    out->WriteLong(AttractorInfluenceFallOff);
    out->WriteULong(AttractorPositionParam);
    out->WriteULong(AttractorPositionParamName);
    out->WriteFloat(AttractorInfluenceRadius);
    out->WriteFloat(AttractorInfluenceForce);
    unsigned long n = AttractorUserPointsArray.size();
    out->WriteULong(n);
    if (n <= 0) return;
    {
        unsigned long offset = 0;
        do {
            out->Write3DVector(*(const C3DVector*)((unsigned char*)AttractorUserPointsArray._M_start + offset));
            offset += sizeof(C3DVector); --n;
        } while (n);
    }
}
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
using namespace NParticleEngine;
static char g_log[64]; static int g_n; static unsigned long g_u[8]; static int g_nu; static long g_l; static float g_f[2]; static int g_nf; static const C3DVector* g_v[8]; static int g_nv;
void CDataOutputStream_Methods::WriteEBOOL(bool v) { g_log[g_n++] = v ? 'T' : 'F'; }
void CDataOutputStream_Methods::WriteLong(long v) { g_log[g_n++] = 'l'; g_l = v; }
void CDataOutputStream_Methods::WriteULong(unsigned long v) { g_log[g_n++] = 'u'; g_u[g_nu++] = v; }
void CDataOutputStream_Methods::WriteFloat(float v) { g_log[g_n++] = 'f'; g_f[g_nf++] = v; }
void CDataOutputStream_Methods::Write3DVector(const C3DVector& v) { g_log[g_n++] = 'v'; g_v[g_nv++] = &v; }
int main() {
    C3DVector pts[3] = { {1,2,3}, {4,5,6}, {7,8,9} };
    CPSCAttractor a; memset(&a, 0, sizeof a);
    a.AttractorEnabled = true; a.AttractorUseParamPosition = false; a.AttractorInfluenceFallOff = -2; a.AttractorPositionParam = 5;
    a.AttractorPositionParamName = 0x1234; a.AttractorInfluenceRadius = 2.5f; a.AttractorInfluenceForce = -1.0f;
    a.AttractorUserPointsArray._M_start = pts; a.AttractorUserPointsArray._M_finish = pts + 3;
    CDataOutputStream_Methods s; a.WriteBinary(&s);
    int ok = offsetof(CPSCAttractor, AttractorInfluenceFallOff) == 0x10 && offsetof(CPSCAttractor, AttractorUserPointsArray) == 0x24;
    ok = ok && g_n == 11 && !memcmp(g_log, "TFluuffuvvv", 11) && g_l == -2 && g_u[0] == 5 && g_u[1] == 0x1234 && g_u[2] == 3;
    ok = ok && g_f[0] == 2.5f && g_f[1] == -1.0f && g_v[0] == &pts[0] && g_v[1] == &pts[1] && g_v[2] == &pts[2];
    g_n = g_nu = g_nf = g_nv = 0; a.AttractorUserPointsArray._M_finish = pts; a.WriteBinary(&s);
    ok = ok && g_n == 8 && !memcmp(g_log, "TFluuffu", 8) && g_u[2] == 0 && g_nv == 0;   /* empty: count only */
    printf(ok ? "00b17f70_TEST PASS\n" : "00b17f70_TEST FAIL\n"); return ok ? 0 : 1;
}