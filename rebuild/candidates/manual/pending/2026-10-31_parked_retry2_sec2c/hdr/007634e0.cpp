// NShape::CBasicShape::UpdateSubdivisions() -- rebuild the shape's triangle subdivision: clear the subdivision list, fracture the shape's point list (and every piece it splits off) into triangles, then wrap each triangle's points in a new CSubdivisionTriangle (counted pointer) appended to the list
// Grade: DIFFER(659v627) -- register allocation and frame packing: retail keeps &Subdivisions in ebp for the whole body (this spilled once to the frame), the new triangle's Data/Info in esi/edi and the triangle iterator in the dead order slot, and shares one frame slot between the first loop's piece vector and the counted pointer; ours keeps this in edi and the iterator in ebx, re-reads Info from memory (so the release is `dec; reload; cmp` instead of `dec [edi]; jne`), gives the piece vector its own slot (frame 0x34 vs 0x30) and runs erase(begin())'s copy with two induction pointers. Callees, layout and control flow match
// Header-moved (retry wave 2 sec2c): engine/CCountedPointer.h replaces the local model; DIFFER(659v627)/153, unchanged by the move
// Leaf: ?UpdateSubdivisions@CBasicShape@NShape@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007634E0, length 627 bytes, thiscall, plain `ret`.
// Name: NShape::CBasicShape vtable 0x0126A31C slot 5 (RTTI) = this body; Ego_r.pdb `?UpdateSubdivisions@CBasicShape@NShape@@UAEXXZ`
//   is CBasicShape's only virtual void() besides the destructor, and it sits in the PDB right after FractureShape (retail
//   0x00763060, the body's work-list callee) as retail 0x007634E0 follows 0x00763060.
// Callees: std::__copy<CCountedPointer<CSubdivisionTriangle>*, ...> 0x00764BC0 and std::__destroy_aux 0x00764EE0 (the
//   inline erase(begin(), end())), vector<vector<C3DVector> >::_M_insert_overflow 0x00765B70 (push_back into the empty
//   work list), vector<C3DVector>(const vector&) 0x006F0810, vector<C3DVector>::operator= 0x0075B8D0 (the inline
//   erase(begin())'s copy), NShape::FractureShape 0x00763060 (Ego_r YI signature), operator new 0x00BFEA1A / delete
//   0x00BFE9BC, CSubdivisionTriangle::GetUpDirection 0x00760A70 (Ego_r), CCountedPointer<CSubdivisionTriangle>::DeleteData
//   0x00763A20, vector<CCountedPointer<...> >::_M_insert_overflow 0x007659F0, malloc/free 0x00BFEA0E / 0x00BFEA14.
// Layout: retail NShape::CShape {vptr, Owner +4, Points +8, SelectedPoint +0x14} (0x20 bytes; Ego_r less the 4-byte-
//   narrower STLport vectors), CBasicShape::Subdivisions +0x20, CSubdivisionTriangle::UpDirection +0x20 (0x2C bytes).
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void __cdecl operator delete(void* block);              // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* p) { return p; }

class C3DVector { public:
    float X;
    float Y;
    float Z;
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                     const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
inline void _Destroy(C3DVector*, C3DVector*) { }

template <class _Tp> class allocator { public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc&, _Value __p) : _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    vector(const vector<_Tp, _Alloc>& __x);             // 0x006F0810 for C3DVector
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x0075B8D0 for C3DVector
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
    _Tp& front() { return *begin(); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy(__position + 1, this->_M_finish, __position, random_access_iterator_tag(), (int*)0);
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy(__last, this->_M_finish, __first, random_access_iterator_tag(), (int*)0);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);   // 0x00765B70 / 0x007659F0
};

} // namespace std


class CTCShapeManager;

namespace NShape {

enum ECirculationOrder { ECirculationOrder_Force32 = 0x7fffffff };

void __fastcall FractureShape(const std::vector<C3DVector>& shape, ECirculationOrder& order,
                              std::vector<std::vector<C3DVector> >& triangles,
                              std::vector<std::vector<C3DVector> >& pieces);   // 0x00763060

class CShape { public:                                  // vtable 0x0126A208
    virtual ~CShape();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void UpdateSubdivisions();
    CTCShapeManager*        Owner;                      // +0x04
    std::vector<C3DVector>  Points;                     // +0x08
    std::vector<C3DVector*> SelectedPoint;              // +0x14
    CShape(CTCShapeManager& owner, const std::vector<C3DVector>& points) : Owner(&owner), Points(points) { }
};

class CSubdivisionTriangle : public CShape { public:   // vtable 0x0126A240
    C3DVector UpDirection;                              // +0x20
    CSubdivisionTriangle(CTCShapeManager& owner, const std::vector<C3DVector>& points) : CShape(owner, points)
    {
        GetUpDirection();
    }
private:
    void GetUpDirection();                              // 0x00760A70
};

class CBasicShape : public CShape { public:            // vtable 0x0126A31C
    std::vector<CCountedPointer<CSubdivisionTriangle> > Subdivisions;   // +0x20
    virtual void UpdateSubdivisions();
};

}

namespace std {
template <> CCountedPointer<NShape::CSubdivisionTriangle>* __fastcall
__copy(CCountedPointer<NShape::CSubdivisionTriangle>* __first, CCountedPointer<NShape::CSubdivisionTriangle>* __last,
       CCountedPointer<NShape::CSubdivisionTriangle>* __result, const random_access_iterator_tag&, int*);   // 0x00764BC0
template <> void __fastcall
__destroy_aux(CCountedPointer<NShape::CSubdivisionTriangle>* __first, CCountedPointer<NShape::CSubdivisionTriangle>* __last,
              const __false_type&);                                                                        // 0x00764EE0
}

void NShape::CBasicShape::UpdateSubdivisions()
{
    std::vector<CCountedPointer<CSubdivisionTriangle> >& subdivisions = Subdivisions;
    subdivisions.clear();
    ECirculationOrder order = (ECirculationOrder)0;
    std::vector<std::vector<C3DVector> > pieces;
    std::vector<std::vector<C3DVector> > triangles;
    pieces.push_back(Points);
    while (!pieces.empty()) {
        std::vector<C3DVector> piece(pieces.front());
        pieces.erase(pieces.begin());
        FractureShape(piece, order, triangles, pieces);
    }
    for (std::vector<std::vector<C3DVector> >::iterator it = triangles.begin(); it != triangles.end(); ++it) {
        CCountedPointer<CSubdivisionTriangle> triangle(new CSubdivisionTriangle(*Owner, *it));
        subdivisions.push_back(triangle);
    }
}
