// Grade: DIFFER(242v274) -- control flow, size test, index math and the insert/clear tails match; the midpoint differs: retail keeps a.X/a.Y on the FPU but copies b.Y, b.Z and a.Z into frame words (36-byte frame: three 12-byte objects, the sum.X temporary stored) and holds a in ecx / b in ebp, ours adds straight from the points (24-byte frame) with a/b in ebp/ecx (tried: member const& operator+, a free by-value operator+, named point copies)
// NShape::CShape::UpdateSplit() -- split edit step: with exactly two selected points that are neighbours in the outline, insert their midpoint between them and clear the selection (true); false otherwise
// Leaf: UpdateSplit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007622C0, length 274 bytes (`ret`). FableWin.pdb ?UpdateSplit@CShape@NShape@@QAE_NXZ.
// Callees: the points vector's insert(iterator, const C3DVector&) @ 0x007653B0, memmove @ 0x00BFEAE6 (the inline clear's
// __copy_trivial). Float pool: 0x0122F59C = 0.5f.
// Layout as the landed NShape::CShape members (UpdateDeletePoint 0x00762010): Points (12-byte C3DVector) +0x08,
// SelectedPoints (C3DVector*) +0x14.
// Shape: size() == 2 folds to `and eax,-4 ; cmp eax,8`; the two selected pointers are locals (front through the vector,
// back() through `finish - 1`), their outline indices are pointer differences (/12 by reciprocal multiply), `abs(i0 - i1) ==
// 1` the cdq/xor/sub idiom; the midpoint is `(*a + *b) * 0.5f` through C3DVector value operators into a named local; the
// insert position is the later of the two points; the inline clear and the two `return` tails are duplicated.
#include <stdlib.h>
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};

struct CPointVector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVector* begin() { return _M_start; }
    C3DVector* insert(C3DVector* position, const C3DVector& value);   // 0x007653B0
};

struct CPointPtrVector {
    C3DVector** _M_start;
    C3DVector** _M_finish;
    C3DVector** _M_end_of_storage;
    C3DVector** begin() { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    C3DVector*& operator[](unsigned long n) { return *(begin() + n); }
    C3DVector*& back() { return *(_M_finish - 1); }
    void clear()
    {
        C3DVector** i = (C3DVector**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

struct CTCShapeManager;
namespace NShape {
struct CShape {
    virtual void v00();
    CTCShapeManager* Owner;
    CPointVector     Points;           // +0x08
    CPointPtrVector  SelectedPoints;   // +0x14
    bool UpdateSplit();
};
}

bool NShape::CShape::UpdateSplit()
{
    if (SelectedPoints.size() == 2) {
        C3DVector* a = SelectedPoints[0];
        long indexA = a - Points.begin();
        C3DVector* b = SelectedPoints.back();
        long indexB = b - Points.begin();
        if (abs(indexA - indexB) == 1) {
            C3DVector midpoint = (*a + *b) * 0.5f;
            Points.insert(indexA > indexB ? a : b, midpoint);
            SelectedPoints.clear();
            return true;
        }
    }
    return false;
}
