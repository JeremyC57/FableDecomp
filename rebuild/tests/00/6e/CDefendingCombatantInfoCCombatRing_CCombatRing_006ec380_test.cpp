// Behaviour test for 006ec380: src/006ec380.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CDefendingCombatantInfo::CCombatRing::CCombatRing(long, const CCombatTypeDef&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CDefendingCombatantInfo::CCombatRing::CCombatRing(long, const CCombatTypeDef&) -- ring `index` of a combat wheel: the def's prong count spread evenly round the Z axis, odd rings rotated by half a step
// Leaf: ??0CCombatRing
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EC380, 295 bytes, `ret 8`; called by CCombatWheel::AddRing 0x006EC7D0 (landed).
// Callees: C3DVector::Rotate(const C3DVector& axis, float angle) 0x00A13C60, CBaseIntelligentPointer ctor 0x00A01B10 / dtor
//   0x00A01C10 (CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8), CProng copy ctor 0x006EFF60, STLport
//   vector<CProng>::_M_insert_overflow 0x006F3F00. Constants: 1.0f 0x0122DED8, 0.5f 0x0122F59C.
// Layout: Ego_r.pdb CCombatRing (RotationAngle +0, DeltaRotation +4, Index +8, RingThickness +0xc, Prongs +0x10, retail
//   12-byte STLport vector) and CProng (28 bytes: Blocked +0, the C3DVector Prong at +1 -- byte-packed in the PDB, hence
//   the unaligned `mov [esp+2Dh]` copies --, ProngBlockageTimer +0x10, POccupier +0x14); CCombatTypeDef +0x3c the
//   per-ring prong counts.
// Shape: init list over one ebx zero register (the Prongs vector through the _Vector_base + _STLP_alloc_proxy model);
//   the count's float conversion and the loop counter live in the dead def argument slot; `dir` is rotated in place by
//   one step per prong and a CProng(dir) temporary is push_back'ed (inline STLport push_back: placement copy or
//   _M_insert_overflow with the __false_type tag in the argument slot, n = 1, at end); the temporary's destructor is
//   POccupier's out-of-line base destructor. RotationAngle = (index % 2) * step * 0.5f.
#include <new>
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Rotate(const C3DVector& axis, float angle);  // 0x00A13C60
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                             // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
#pragma pack(push, 1)
struct CProngHead {                                   // the PDB's byte-packed head of CProng
    bool Blocked;                                     // +0x00
    C3DVector Prong;                                  // +0x01
    unsigned char _pad0d[3];
    CProngHead(const C3DVector& prong) : Blocked(false), Prong(prong) { }
};
#pragma pack(pop)
struct CProng : CProngHead {                          // CDefendingCombatantInfo::CProng, 28 bytes
    long ProngBlockageTimer;                          // +0x10
    CIntelligentPointer POccupier;                    // +0x14
    CProng(const C3DVector& prong) : CProngHead(prong), ProngBlockageTimer(0), POccupier() { }
    CProng(const CProng& other);                      // 0x006EFF60
};
struct __false_type { __false_type() { } };
struct allocator_type { };
template <class T> struct _STLP_alloc_proxy {
    T* _M_data;
    _STLP_alloc_proxy(const allocator_type&, T* data) : _M_data(data) { }
};
struct CProngVector {                                 // STLport vector<CProng>
    CProng* _M_start;
    CProng* _M_finish;
    _STLP_alloc_proxy<CProng> _M_end_of_storage;
    CProngVector() : _M_start(0), _M_finish(0), _M_end_of_storage(allocator_type(), 0) { }
    void _M_insert_overflow(CProng* pos, const CProng& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x006F3F00
    void push_back(const CProng& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) CProng(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
__declspec(noinline) void CProngVector::_M_insert_overflow(CProng* pos, const CProng& x, const __false_type& p2, unsigned long fill_len, bool atend)
{
    CProng* p = (CProng*)GrowVectorStorage_s7(this, fill_len);
    if (p)
        new (p) CProng(x);
    
}
#pragma auto_inline(on)
class CCombatTypeDef { public:
    unsigned char _pad00[0x3c];
    long* ProngsPerRing;                              // +0x3c (vector data)
};
class CDefendingCombatantInfo { public:
    class CCombatRing { public:
        float RotationAngle;                          // +0x00
        float DeltaRotation;                          // +0x04
        long Index;                                   // +0x08
        float RingThickness;                          // +0x0c
        CProngVector Prongs;                          // +0x10
        CCombatRing(long index, const CCombatTypeDef& def);
    };
};

CDefendingCombatantInfo::CCombatRing::CCombatRing(long index, const CCombatTypeDef& def)
    : RotationAngle(0), DeltaRotation(0), Index(index), RingThickness(0)
{
    long count = def.ProngsPerRing[index];
    C3DVector dir(1.0f, 0.0f, 0.0f);
    C3DVector axis(0.0f, 0.0f, 1.0f);
    float step = 1.0f / count;
    while (count != 0) {
        dir.Rotate(axis, step);
        Prongs.push_back(CProng(dir));
        --count;
    }
    RotationAngle = index % 2 * step * 0.5f;
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_grow = 0, g_rot = 0, g_ctor = 0, g_dtor = 0, g_copy = 0; static float g_steps[8];
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{   // the stub grows into fresh heap storage of exactly the new size (old elements copied bytewise)
    ++g_grow; CProngVector* v = (CProngVector*)storage; long n = v->_M_finish - v->_M_start;
    CProng* s = (CProng*)malloc((n + count) * sizeof(CProng)); if (n) memcpy(s, v->_M_start, n * sizeof(CProng));
    v->_M_start = s; v->_M_finish = s + n + count; v->_M_end_of_storage._M_data = v->_M_finish; return s + n;
}
void C3DVector::Rotate(const C3DVector& axis, float angle) { CHECK(axis.X == 0 && axis.Y == 0 && axis.Z == 1); g_steps[g_rot & 7] = angle; ++g_rot; Y += 1.0f; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { ++g_ctor; m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtor; }
CProng::CProng(const CProng& other) : CProngHead(other.Prong), ProngBlockageTimer(other.ProngBlockageTimer), POccupier() { ++g_copy; Blocked = other.Blocked; }
typedef CDefendingCombatantInfo::CCombatRing Ring;
int main()
{
    long counts[3] = { 4, 3, 0 };
    CCombatTypeDef def; def.ProngsPerRing = counts;
    Ring* r0 = (Ring*)malloc(sizeof(Ring));
    new (r0) Ring(0, def);
    CHECK(r0->Index == 0 && r0->DeltaRotation == 0 && r0->RingThickness == 0 && r0->RotationAngle == 0.0f);
    CHECK(g_rot == 4 && g_steps[0] == 0.25f && g_steps[3] == 0.25f); CHECK(g_grow == 4 && r0->Prongs._M_finish - r0->Prongs._M_start == 4);
    for (int i = 0; i < 4; ++i) { CProng& p = r0->Prongs._M_start[i]; CHECK(p.Blocked == false && p.ProngBlockageTimer == 0 && p.Prong.X == 1.0f && p.Prong.Y == (float)(i + 1) && p.Prong.Z == 0.0f); }
    CHECK(g_ctor == 8 && g_dtor == 4 && g_copy == 4);   // one temporary prong per push, destroyed after the push
    // ring 1, room for the pushes already reserved: the in-place copy constructor path
    Ring* r1 = (Ring*)malloc(sizeof(Ring));
    int rot0 = g_rot, grow0 = g_grow;
    new (r1) Ring(1, def);
    CHECK(g_rot == rot0 + 3); CHECK(g_steps[rot0 & 7] == 1.0f / 3);
    CHECK(r1->RotationAngle == 1 % 2 * (1.0f / 3) * 0.5f); CHECK(r1->Index == 1); CHECK(g_grow - grow0 == 3 && g_copy == 7);   // every push copy-constructs into the new storage
    Ring* r2 = (Ring*)malloc(sizeof(Ring));
    rot0 = g_rot; new (r2) Ring(2, def); CHECK(g_rot == rot0 && r2->Prongs._M_start == 0 && r2->Prongs._M_finish == 0); CHECK(r2->RotationAngle != r2->RotationAngle);   // 0 * (1/0) * 0.5: NaN, as retail
    if (fails == 0) printf("006ec380_TEST PASS\n"); else printf("006ec380_TEST FAIL\n");
    return fails;
}