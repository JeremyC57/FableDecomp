// NShape::CShape::UpdateDeletePoint() -- delete-point edit step: false with nothing selected; with exactly one selected point remove it from the outline when the point count minus one exceeds 2, then clear the selection; true otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00762010, length 116 bytes.
// Callees: the points vector's erase-by-pointer @ 0x0075F4E0 (catalog "MedianCut::GetNewTreeNode" is a propagated name),
// memmove @ 0x00BFEAE6 (STLport __copy_trivial inside the inlined clear()).
// Layout as the landed NShape::CShape members: points vector (12-byte C3DVector) at +0x08, selected-point pointers at +0x14.
// Shape: `lea ecx,[esi+8]` before the zero test is a reference local to the points vector; the `return false` exit
// at the end is the outer `if (n != 0) {...; return true;} return false;`; clear() is the landed STLport
// `_M_finish = CopyTrivial(_M_finish, _M_finish, _M_start)` (retail `cmp eax,eax`).
// Leaf: UpdateDeletePoint
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct C3DVector { float X, Y, Z; };

struct CPointVector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    void ErasePoint(C3DVector* point);           // 0x0075F4E0
};

struct CPointPtrVector {
    C3DVector** _M_start;
    C3DVector** _M_finish;
    C3DVector** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
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
    bool UpdateDeletePoint();
};
}

bool NShape::CShape::UpdateDeletePoint()
{
    unsigned long n = SelectedPoints.size();
    CPointVector& points = Points;
    if (n != 0) {
        if (n == 1) {
            if (points.size() - 1 > 2)
                points.ErasePoint(SelectedPoints.back());
            SelectedPoints.clear();
        }
        return true;
    }
    return false;
}