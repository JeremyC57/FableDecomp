// NCameraModes::CTools::PotentialFieldBufferPoints(const C3DVector& pos, const list<C3DVector>& points, float strength, float radius) const -- the gradient of the summed potential field of the buffer points at pos, by forward differences over the four sample offsets (pos itself and one step along each axis)
// Leaf: ?PotentialFieldBufferPoints@CTools@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00876E10, 303 bytes (as cut), thiscall (`this` unused) with a hidden
//   result pointer, `ret 14h` (result, pos, points, strength, radius); returns the result pointer in eax.
// Identity: the catalog's "Vector_ComputePotentialGradient" is descriptive. FableWin/Ego_r PDB:
//   `?PotentialFieldBufferPoints@CTools@NCameraModes@@QBE?AVC3DVector@@ABV3@ABV?$list@VC3DVector@@...@std@@MM@Z` (the
//   neighbour 0x00876CA0 is PotentialFieldBufferObjects over list<const CThingGameObject*>, same globals). The inline
//   CTools::PotentialField(const C3DVector&, const C3DVector&, float, float) const is
//   `(strength * 0.1f) / ((pos - source).GetLength() - radius)` (the strength product hoisted out of both loops).
// Globals: the sample step 0x01385D20 (0.1f, .data) and the four sample offsets 0x01385D24 ({0,0,0}, {1,0,0},
//   {0,1,0}, {0,0,1}; .data, statically initialised) -- names not in the PDB sets, descriptive. Constants: 0.1f
//   0x0122EE58, 1.0f 0x0122DED8.
// Shape: four zeroed float potentials; for each point (STLport list: sentinel _M_node, node data at +8) and each offset
//   `potential[i] += PotentialField(pos + offset * step, point, strength, radius)`; then the forward differences each
//   divided by the step inside `return C3DVector(...)`: VC7.1 (no /Op) turns the three divisions by one value into a
//   reciprocal (`fld 1.0 ; fdiv step`, spilled to the dead `points` slot) and three multiplies, Z evaluated first. A
//   named `1.0f / step` scale stays on the x87 stack and stores X first; operator* / operator/ on a vector temporary
//   builds the differences before the reciprocal.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    const C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};
template <class _Tp> struct _List_const_iterator {
    _List_node_base* _M_node;
    _List_const_iterator(_List_node_base* __x) : _M_node(__x) { }
    const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_const_iterator& __x) const { return _M_node != __x._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_const_iterator<_Tp> const_iterator;
    _List_node<_Tp>* _M_node;                               // the sentinel
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
} // namespace std

extern float g_PotentialFieldSampleStep;                    // 0x01385D20 (0.1f)
extern C3DVector g_PotentialFieldSampleOffsets[4];          // 0x01385D24

namespace NCameraModes {
class CTools {
public:
    float PotentialField(const C3DVector& pos, const C3DVector& source, float strength, float radius) const
    {
        return (strength * 0.1f) / ((pos - source).GetLength() - radius);
    }
    C3DVector PotentialFieldBufferPoints(const C3DVector& pos, const std::list<C3DVector>& points, float strength, float radius) const;
};
}

C3DVector NCameraModes::CTools::PotentialFieldBufferPoints(const C3DVector& pos, const std::list<C3DVector>& points,
                                                          float strength, float radius) const
{
    float potential[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (std::list<C3DVector>::const_iterator it = points.begin(); it != points.end(); ++it) {
        for (int i = 0; i < 4; ++i)
            potential[i] += PotentialField(pos + g_PotentialFieldSampleOffsets[i] * g_PotentialFieldSampleStep, *it, strength, radius);
    }
    return C3DVector((potential[1] - potential[0]) / g_PotentialFieldSampleStep,
                     (potential[2] - potential[0]) / g_PotentialFieldSampleStep,
                     (potential[3] - potential[0]) / g_PotentialFieldSampleStep);
}