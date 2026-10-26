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