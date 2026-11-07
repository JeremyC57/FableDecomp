// NShape::IsShapeConvex(const std::vector<C3DVector>& shape) -- add up the turning angles (in turns) between the outline's consecutive XY edge directions, closing edges included, and call the shape convex when they come to one full turn
// Leaf: ?IsShapeConvex@NShape@@YI_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "engine/C3DVector.h"

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_iterator;
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
};
}

extern const float INV_TWO_PI;                       // 0x01237F08 (1 / (2 pi))

namespace NShape {

bool __fastcall IsShapeConvex(const std::vector<C3DVector>& shape);

bool __fastcall IsShapeConvex(const std::vector<C3DVector>& shape)
{
    std::vector<C3DVector>::const_iterator first = shape.begin();
    C3DVector start = first[0];
    C3DVector next = first[1];
    C3DVector previous(next.X - start.X, next.Y - start.Y, 0.0f);
    previous.Normalise();
    float total = 0.0f;
    C3DVector edge;
    std::vector<C3DVector>::const_iterator from = first + 1;
    for (std::vector<C3DVector>::const_iterator to = first + 2; to != shape.end(); ++from, ++to) {
        C3DVector a = *from;
        C3DVector b = *to;
        C3DVector direction = b - a;
        direction.Z = 0.0f;
        direction.Normalise();
        total += (float)acos(direction.Dot(previous)) * INV_TWO_PI;
        previous = direction;
    }
    {
        C3DVector a = *(shape.end() - 1);
        C3DVector b = *first;
        edge = C3DVector(b.X - a.X, b.Y - a.Y, 0.0f);
    }
    edge.Normalise();
    double closing = edge.Dot(previous);
    previous = edge;
    {
        C3DVector a = first[0];
        C3DVector b = first[1];
        edge = C3DVector(b.X - a.X, b.Y - a.Y, 0.0f);
    }
    edge.Normalise();
    float opening = edge.Dot(previous);
    float angle;
    float turns = total + (angle = (float)acos(closing)) * INV_TWO_PI;
    turns += (angle = (float)acos(opening)) * INV_TWO_PI;
    turns -= 1.0f;
    if (turns * turns < 0.0025f)
        return true;
    return false;
}

}
