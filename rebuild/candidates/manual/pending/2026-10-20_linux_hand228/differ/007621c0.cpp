// Grade: DIFFER(243v243) -- retail keeps `this` in ebp and the point iterator in ebx (ours the reverse), spills dx to a frame word and squares it from memory (`fld [x] ; fmul [x]`, ours keeps it on the FPU), and homes push_back's __true_type tag in the live `position` argument slot (ours in a frame byte); the 12-byte frame / `selected` frame byte now match (inline AddUnique helper + __find stand-in)
// NShape::CShape::UpdateSelectedPoint(const C3DVector& position) -- select-point edit step: add every outline point within 0.25 of the position to the selection (once each); true when one was hit, else clear the selection and return false
// Leaf: UpdateSelectedPoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007621C0, length 241 bytes (`ret 4`).
// Callees: std::find<C3DVector**> @ 0x00764080 (Dinkumware/STLport `__find`, `__fastcall`, value and tag by reference; landed
// as Find_00764080), the selection vector's _M_insert_overflow(pos, const value&, const __true_type&, n, atEnd) @ 0x00765130,
// memmove @ 0x00BFEAE6 (the inline clear's __copy_trivial). Float pool: 0x0122DC98 = 0.0625f.
// Layout as the landed NShape::CShape members (UpdateDeletePoint 0x00762010): Points (12-byte C3DVector) +0x08,
// SelectedPoints (C3DVector*) +0x14.
// Shape: the point iterator is a named local kept in ebx and in its frame word (its address feeds find and push_back); the
// squared distance is one expression (Y term first); the inline push_back stores through a null-checked placement and
// falls back to _M_insert_overflow with the `__true_type` tag homed in the `position` argument slot; `selected` is a frame
// byte; the inline clear runs when nothing was hit. The retail __find is defined above as a plain stand-in body under
// `auto_inline(off)` (the lander binds the call to 0x00764080): with it VC7.1 places the find tag in the argument slot too.
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct __true_type { __true_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };

#pragma auto_inline(off)
C3DVector** __fastcall __find(C3DVector** first, C3DVector** last, C3DVector* const& value, const random_access_iterator_tag&)
{
    for (; first != last; ++first)
        if (*first == value)
            return first;
    return last;
}
#pragma auto_inline(on)

struct CPointVector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
};

struct CPointPtrVector {
    C3DVector** _M_start;
    C3DVector** _M_finish;
    C3DVector** _M_end_of_storage;
    C3DVector** begin() { return _M_start; }
    C3DVector** end() { return _M_finish; }
    void _M_insert_overflow(C3DVector** position, C3DVector* const& value, const __true_type&, unsigned int count, bool atEnd);   // 0x00765130
    void push_back(C3DVector* const& value)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = value;
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, value, __true_type(), 1, true);
    }
    inline void AddUnique(C3DVector* const& value);
    void clear()
    {
        C3DVector** i = (C3DVector**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

inline C3DVector** find(C3DVector** first, C3DVector** last, C3DVector* const& value)
{
    return __find(first, last, value, random_access_iterator_tag());
}

inline void CPointPtrVector::AddUnique(C3DVector* const& value)
{
    if (find(begin(), end(), value) == end())
        push_back(value);
}

struct CTCShapeManager;
namespace NShape {
struct CShape {
    virtual void v00();
    CTCShapeManager* Owner;
    CPointVector     Points;           // +0x08
    CPointPtrVector  SelectedPoints;   // +0x14
    bool UpdateSelectedPoint(const C3DVector& position);
};
}

bool NShape::CShape::UpdateSelectedPoint(const C3DVector& position)
{
    bool selected = false;
    for (C3DVector* point = Points._M_start; point != Points._M_finish; ++point) {
        float dx = point->X - position.X;
        float dy = point->Y - position.Y;
        if (dx * dx + dy * dy < 0.0625f) {
            SelectedPoints.AddUnique(point);
            selected = true;
        }
    }
    if (!selected)
        SelectedPoints.clear();
    return selected;
}
