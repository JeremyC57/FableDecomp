// Behaviour test for 00b18060: src/00b18060.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CPSCAttractor::DatabaseCopy(const CPSCAttractor&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPSCAttractor::DatabaseCopy(const CPSCAttractor&) -- copy an attractor component's database settings: the two flag bits, the position parameter and its name, radius, force, fall-off and the user point list
// Leaf: DatabaseCopy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18060, 170 bytes (+ int3 padding; `ret 4`).
// Callees: vector<C3DVector>::reserve(size_type) 0x00920FF0 (manifest "vector_reserve") and resize(size_type, const C3DVector&)
//   0x0075C040 (landed as CVector3D::resize in CTCNymph::OnInitialActivate 0x0080FD90; default, uninitialised C3DVector fill).
// Layout = Ego_r.pdb CPSCAttractor (52 bytes; the user point CArray is a 12-byte STLport vector in retail): AttractorEnabled /
//   AttractorUseParamPosition bits 0/1 of +0x0c, AttractorInfluenceFallOff +0x10, AttractorPositionParam +0x14,
//   AttractorPositionParamName +0x18, AttractorInfluenceRadius +0x1c, AttractorInfluenceForce +0x20,
//   AttractorUserPointsArray +0x24.
// Shape: the source is read through a `const CPSCAttractor* src = &other` local (with the reference itself VC7.1 re-reads
//   the destination flag byte instead of merging through bl); each flag is a bitfield assignment (`xor ; and mask ; xor`
//   read-modify-write); the scalars copy in retail order; the
//   point list is sized with reserve + resize to the source count, then copied element by element (implicit C3DVector
//   assignment) with both arrays re-read each pass.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
};
struct CVector3D {                                 // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    C3DVector& operator[](unsigned int n) const { return _M_start[n]; }
    void reserve(unsigned int count);              // 0x00920FF0
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());   // 0x0075C040
};
class CPSCAttractor {
public:
    void*         __vftable;                       // +0x00
    unsigned char _pad04[8];
    bool          AttractorEnabled : 1;            // +0x0c bit 0
    bool          AttractorUseParamPosition : 1;   // +0x0c bit 1
    long          AttractorInfluenceFallOff;       // +0x10
    unsigned long AttractorPositionParam;          // +0x14
    unsigned long AttractorPositionParamName;      // +0x18
    float         AttractorInfluenceRadius;        // +0x1c
    float         AttractorInfluenceForce;         // +0x20
    CVector3D     AttractorUserPointsArray;        // +0x24
    void DatabaseCopy(const CPSCAttractor& other);
};

void CPSCAttractor::DatabaseCopy(const CPSCAttractor& other)
{
    const CPSCAttractor* src = &other;
    AttractorEnabled = src->AttractorEnabled;
    AttractorPositionParam = src->AttractorPositionParam;
    AttractorUseParamPosition = src->AttractorUseParamPosition;
    AttractorPositionParamName = src->AttractorPositionParamName;
    AttractorInfluenceRadius = src->AttractorInfluenceRadius;
    AttractorInfluenceForce = src->AttractorInfluenceForce;
    AttractorInfluenceFallOff = src->AttractorInfluenceFallOff;
    unsigned int count = src->AttractorUserPointsArray.size();
    AttractorUserPointsArray.reserve(count);
    AttractorUserPointsArray.resize(count);
    for (unsigned int i = 0; i < count; ++i)
        AttractorUserPointsArray[i] = src->AttractorUserPointsArray[i];
}

// ---- stubs and driver ----
static int g_reserves = 0, g_resizes = 0; static unsigned int g_reserveN = 0, g_resizeN = 0;
static C3DVector g_store[8];
void CVector3D::reserve(unsigned int count) { ++g_reserves; g_reserveN = count; if (!_M_start) { _M_start = _M_finish = g_store; _M_end_of_storage = g_store + 8; } }
void CVector3D::resize(unsigned int newSize, const C3DVector&) { ++g_resizes; g_resizeN = newSize; CHECK(g_reserves == g_resizes); _M_finish = _M_start + newSize; }
int main()
{
    static CPSCAttractor a, b;
    static C3DVector pts[3];
    for (int k = 0; k < 3; ++k) { pts[k].X = (float)(3 * k + 1); pts[k].Y = (float)(3 * k + 2); pts[k].Z = (float)(3 * k + 3); }
    memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b));
    b.AttractorEnabled = true; b.AttractorUseParamPosition = false;
    b.AttractorInfluenceFallOff = 7; b.AttractorPositionParam = 0x1234; b.AttractorPositionParamName = 0xabcd;
    b.AttractorInfluenceRadius = 2.5f; b.AttractorInfluenceForce = -1.25f;
    b.AttractorUserPointsArray._M_start = pts; b.AttractorUserPointsArray._M_finish = pts + 3; b.AttractorUserPointsArray._M_end_of_storage = pts + 3;
    a.AttractorUseParamPosition = true;
    unsigned char other = ((unsigned char*)&a)[0x0c] & 0xfc;   // bits outside the two flags
    ((unsigned char*)&a)[0x0c] |= 0xa0; other = ((unsigned char*)&a)[0x0c] & 0xfc;
    a.DatabaseCopy(b);
    CHECK(a.AttractorEnabled == true && a.AttractorUseParamPosition == false);
    CHECK((((unsigned char*)&a)[0x0c] & 0xfc) == other);           // only the two flag bits change
    CHECK(a.AttractorInfluenceFallOff == 7 && a.AttractorPositionParam == 0x1234 && a.AttractorPositionParamName == 0xabcd);
    CHECK(a.AttractorInfluenceRadius == 2.5f && a.AttractorInfluenceForce == -1.25f);
    CHECK(g_reserves == 1 && g_reserveN == 3 && g_resizes == 1 && g_resizeN == 3);
    CHECK(a.AttractorUserPointsArray.size() == 3);
    CHECK(g_store[0].X == 1 && g_store[1].Y == 5 && g_store[2].Z == 9);
    // second copy: flags the other way round, empty point list (no element copies)
    b.AttractorEnabled = false; b.AttractorUseParamPosition = true; b.AttractorUserPointsArray._M_finish = pts;
    g_store[0].X = 42;
    a.DatabaseCopy(b);
    CHECK(a.AttractorEnabled == false && a.AttractorUseParamPosition == true);
    CHECK((((unsigned char*)&a)[0x0c] & 0xfc) == other);
    CHECK(g_reserveN == 0 && g_resizeN == 0 && a.AttractorUserPointsArray.size() == 0 && g_store[0].X == 42);
    if (fails == 0) printf("00b18060_TEST PASS\n");
    return fails;
}
