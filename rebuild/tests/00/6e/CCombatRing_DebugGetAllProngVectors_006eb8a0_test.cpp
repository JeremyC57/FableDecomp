// Behaviour test for 006eb8a0: src/006eb8a0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CCombatRing::DebugGetAllProngVectors(vector<pair<C3DVector,bool> >& out, float scale)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatRing::DebugGetAllProngVectors(vector<pair<C3DVector,bool> >& out, float scale) -- debug view of the ring: every prong direction rotated by the ring's angle about +Z, scaled, paired with its blocked flag and appended to `out`
// Leaf: ?DebugGetAllProngVectors@CCombatRing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006EB8A0, length 253 bytes (`ret 8`).
// Callees: C3DVector::Rotate(const C3DVector& axis, float angle) 0x00A13C60 (axis = C3DVector(0, 0, 1), angle =
//          RotationAngle), vector<pair<C3DVector,bool> >::_M_insert_overflow 0x006F3260 (tag homed in the dead `scale` slot).
// Layout: Ego_r.pdb CCombatRing (RotationAngle +0, Prongs +0x10) and CDefendingCombatantInfo::CProng (Blocked +0, Prong +1:
//          C3DVector is 1-aligned in this module -- modelled under pack(1) -- ProngBlockageTimer +0x10, POccupier +0x14).
#include <new>
#pragma pack(push, 1)
class C3DVector { public:                              // 1-aligned in retail (CProng::Prong sits at +1)
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Rotate(const C3DVector& axis, float angle);   // 0x00A13C60
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
#pragma pack(pop)
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) { }
};
typedef pair<C3DVector, bool> SProngVector;      // 13 bytes
struct __false_type {};
struct CProngVectorVector {                      // STLport vector<pair<C3DVector,bool> >
    SProngVector* _M_start;
    SProngVector* _M_finish;
    SProngVector* _M_end_of_storage;
    void _M_insert_overflow(SProngVector* pos, const SProngVector& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x006F3260
    void push_back(const SProngVector& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SProngVector(x);
            ++_M_finish;
        } else {
            __false_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1, true);
        }
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
__declspec(noinline) void CProngVectorVector::_M_insert_overflow(SProngVector* pos, const SProngVector& x, const __false_type& p2, unsigned int fill_len, bool atend)
{
    SProngVector* p = (SProngVector*)GrowVectorStorage_s7(this, fill_len);
    if (p)
        *p = x;
    
}
#pragma auto_inline(on)
struct CBaseIntelligentPointer { void* __vftable; void* m_pItem; };
struct CProng {                                 // CDefendingCombatantInfo::CProng, 0x1c bytes
    bool      Blocked;                          // +0x00
    C3DVector Prong;                            // +0x01
    long      ProngBlockageTimer;               // +0x10
    CBaseIntelligentPointer POccupier;          // +0x14
};
class CCombatRing {
public:
    float   RotationAngle;                      // +0x00
    float   DeltaRotation;                      // +0x04
    long    Index;                              // +0x08
    float   RingThickness;                      // +0x0c
    CProng* _M_start;                           // +0x10 Prongs
    CProng* _M_finish;                          // +0x14
    CProng* _M_end_of_storage;                  // +0x18
    void DebugGetAllProngVectors(CProngVectorVector& out, float scale);
};

void CCombatRing::DebugGetAllProngVectors(CProngVectorVector& out, float scale)
{
    for (CProng* it = _M_start; it != _M_finish; ++it) {
        C3DVector prong = it->Prong;
        prong.Rotate(C3DVector(0, 0, 1), RotationAngle);
        C3DVector scaled = prong * scale;
        out.push_back(SProngVector(scaled, it->Blocked));
    }
}

// ---- stubs and driver ----
static unsigned char g_raw[8 * sizeof(SProngVector)]; static int g_grow = 0, g_rot = 0; static float g_angle = 0; static C3DVector g_axis(9, 9, 9);
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; CProngVectorVector* v = (CProngVectorVector*)vector; SProngVector* p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
void C3DVector::Rotate(const C3DVector& axis, float angle) { ++g_rot; g_axis = axis; g_angle = angle; X += angle; }
int main()
{
    CProng prongs[3]; memset(prongs, 0, sizeof(prongs));
    prongs[0].Blocked = true; prongs[0].Prong = C3DVector(1, 2, 3);
    prongs[1].Blocked = false; prongs[1].Prong = C3DVector(-1, 0, 4);
    prongs[2].Blocked = true; prongs[2].Prong = C3DVector(0, 5, 0);
    CCombatRing ring; ring.RotationAngle = 0.5f; ring._M_start = prongs; ring._M_finish = prongs + 3; ring._M_end_of_storage = prongs + 3;
    SProngVector* store = (SProngVector*)g_raw;
    CProngVectorVector out; out._M_start = store; out._M_finish = store; out._M_end_of_storage = store + 2;
    ring.DebugGetAllProngVectors(out, 2.0f);
    CHECK(out._M_finish == store + 3); CHECK(g_grow == 1); CHECK(g_rot == 3 && g_angle == 0.5f && g_axis.X == 0 && g_axis.Y == 0 && g_axis.Z == 1);
    CHECK(store[0].first.X == 3.0f && store[0].first.Y == 4.0f && store[0].first.Z == 6.0f && store[0].second == true);
    CHECK(store[1].first.X == -1.0f && store[1].first.Y == 0.0f && store[1].first.Z == 8.0f && store[1].second == false);
    CHECK(store[2].first.X == 1.0f && store[2].first.Y == 10.0f && store[2].first.Z == 0.0f && store[2].second == true);
    CHECK(prongs[0].Prong.X == 1.0f);   // the ring's prongs are not modified
    ring._M_finish = prongs; ring.DebugGetAllProngVectors(out, 2.0f); CHECK(g_rot == 3 && out._M_finish == store + 3);
    if (fails == 0) printf("006eb8a0_TEST PASS\n"); else printf("006eb8a0_TEST FAIL\n");
    return fails;
}