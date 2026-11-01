// CParticleProgram::ExecuteProgram(CParticleSimulation& simulation) -- run the constraint stream once: repeat records push a CLoopBegin on LoopStack, end-repeat records loop back or pop, distance-constraint records are satisfied on the simulation, the other constraint kinds do nothing in this build
// Grade: DIFFER(296v292) -- frame 0x14 v 0x10: VC7.1 gives the empty __false_type tag of _M_insert_overflow its own slot where retail homes it on the live `simulation` argument slot (lea [esp+2Ch]); plus push_back's fast path reloads LoopCount from the CLoopBegin local instead of keeping ecx, and two register / scheduling choices in cases 1 and 2. Jump table, register assignment and every other instruction match.
// Leaf: ?ExecuteProgram@CParticleProgram@@ABEXAAVCParticleSimulation@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE1A90, 292 bytes incl. the 6-entry case table at 0x00AE1B9C, thiscall, `ret 4`.
// Identity (proven): sole caller 0x00AE252E in 0x00AE2510, a `ret 0Ch` loop that zeroes simulation+0x30 (MaximumAdjustment),
//   calls this row and compares MaximumAdjustment with its float argument = ?ApplyConstraints@CParticleProgram@@ABEXAAVCParticleSimulation@@JM@Z
//   (Ego_r.pdb / FableWin.pdb, beside ?ExecuteProgram@CParticleProgram@@ABEXAAVCParticleSimulation@@@Z: private const, one
//   CParticleSimulation& argument = `ret 4`). Catalog label right.
// Layout: engine/CParticleProgram.h retail-container offsets (flattened byte arrays): Constraints (CMemoryDataOutputStream 0x18)
//   +0x114, LoopStack (CArray<CParticleProgram::CLoopBegin>, the 12-byte STLport vector) +0x12c. CLoopBegin = Ego_r
//   {Position, LoopCount} (8 bytes). The stream records are {TConstraintType, count, data size} headers followed by the data.
// FableWin.pdb names what the body calls: CArray<CLoopBegin>::push_back, vector::empty (QBE), vector::back (QAE, so LoopStack is
//   reached non-const from a const member: `mutable`), vector::pop_back, vector::clear; and the private CParticleSimulation
//   SatisfyDistanceConstraints / SatisfyMinMaxDistanceConstraints / SatisfyUnbendConstraints / SatisfySphereCollisionConstraints
//   (const T*, unsigned long). Ego_r.pdb keeps only SatisfyDistanceConstraints 0x00ADF960: the other three are empty inline
//   bodies in this build, which is why cases 3..5 of the retail table jump straight to `position += size`.
// Callees: CMemoryDataOutputStream::PeekData() const 0x00995E70, CMemoryDataOutputStream::GetLength() const 0x00994B40,
//   vector<CLoopBegin>::_M_insert_overflow 0x00AE64C0, CParticleSimulation::SatisfyDistanceConstraints 0x00ADF960.
// Descriptive: the TConstraintType enumerator names and SConstraintHeader (no PDB spelling; the enum itself is global
//   ?IsControlType@CParticleProgram@@AAE_NW4TConstraintType@@@Z).
// Next: find what makes VC7.1 home the empty tag on a live argument slot (the 0x00B1B380 precedent homes it on a DEAD one);
//   then the LoopCount reload should follow the frame.
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp& reference;
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
    reference back() { return *(end() - 1); }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                    // 0x00AE64C0 (protected, as STLport 4.6 and the landed row)
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    void pop_back() { --this->_M_finish; _Destroy(this->_M_finish); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}
template <class T> class CArray : public std::vector<T> {   // retail CArray<T> = the 12-byte STLport vector
public:
    void push_back(const T& x) { std::vector<T>::push_back(x); }
};
class CMemoryDataOutputStream {
public:
    const void* PeekData() const;                                       // 0x00995E70
    unsigned long GetLength() const;                                    // 0x00994B40
    unsigned char _body[0x18];
};
class CParticleDistanceConstraint;
class CParticleMinMaxDistanceConstraint;
class CParticleUnbendConstraint;
class CParticleSphereCollisionConstraint;
class CParticleProgram;
class CParticleSimulation {
    friend class CParticleProgram;
    void SatisfyDistanceConstraints(const CParticleDistanceConstraint* constraints, unsigned long count);   // 0x00ADF960
    void SatisfyMinMaxDistanceConstraints(const CParticleMinMaxDistanceConstraint*, unsigned long) { }
    void SatisfyUnbendConstraints(const CParticleUnbendConstraint*, unsigned long) { }
    void SatisfySphereCollisionConstraints(const CParticleSphereCollisionConstraint*, unsigned long) { }
};
enum TConstraintType { CT_REPEAT, CT_END_REPEAT, CT_DISTANCE, CT_MINMAX_DISTANCE, CT_UNBEND, CT_SPHERE_COLLISION };
class CParticleProgram {
public:
    class CLoopBegin {
    public:
        unsigned long Position;
        long          LoopCount;
    };
    unsigned char           _body[0x114];
    CMemoryDataOutputStream Constraints;                                // +0x114
    mutable CArray<CLoopBegin> LoopStack;                               // +0x12c
    long                    MemoryHandle;
    void*                   MemoryPool;
private:
    void ExecuteProgram(CParticleSimulation& simulation) const;
};
struct SConstraintHeader { TConstraintType Type; unsigned long Count; unsigned long Size; };
void CParticleProgram::ExecuteProgram(CParticleSimulation& simulation) const
{
    const unsigned char* data = (const unsigned char*)Constraints.PeekData();
    unsigned long length = Constraints.GetLength();
    LoopStack.clear();
    for (unsigned long position = 0; position < length; ) {
        const SConstraintHeader* header = (const SConstraintHeader*)(data + position);
        unsigned long count = header->Count;
        unsigned long size = header->Size;
        TConstraintType type = header->Type;
        position += sizeof(SConstraintHeader);
        switch (type) {
        case CT_REPEAT: {
            CLoopBegin loopBegin;
            loopBegin.Position = position + size;
            loopBegin.LoopCount = count;
            LoopStack.push_back(loopBegin);
            break;
        }
        case CT_END_REPEAT:
            if (!LoopStack.empty()) {
                CLoopBegin& loopBegin = LoopStack.back();
                if (--loopBegin.LoopCount > 0)
                    position = loopBegin.Position;
                else
                    LoopStack.pop_back();
            }
            break;
        case CT_DISTANCE:
            simulation.SatisfyDistanceConstraints((const CParticleDistanceConstraint*)(data + position), count);
            break;
        case CT_MINMAX_DISTANCE:
            simulation.SatisfyMinMaxDistanceConstraints((const CParticleMinMaxDistanceConstraint*)(data + position), count);
            break;
        case CT_UNBEND:
            simulation.SatisfyUnbendConstraints((const CParticleUnbendConstraint*)(data + position), count);
            break;
        case CT_SPHERE_COLLISION:
            simulation.SatisfySphereCollisionConstraints((const CParticleSphereCollisionConstraint*)(data + position), count);
            break;
        }
        position += size;
    }
}
