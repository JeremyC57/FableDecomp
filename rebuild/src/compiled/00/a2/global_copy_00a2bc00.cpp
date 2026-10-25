// __copy(CPhysicsMeshInfo* first, CPhysicsMeshInfo* last, CPhysicsMeshInfo* result, random_access_iterator_tag, int*) -- assign a range of physics-mesh infos forwards
// Leaf: ??$__copy@PAVCPhysicsMeshInfo@@PAV1@H@std@@YIPAVCPhysicsMeshInfo@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2BC00, 97 bytes, two `ret 0Ch` (n <= 0 early exit). Callers 0x00A2C47C and 0x00A2CA89 (the
//   CPhysicsMeshInfo vector's assignment/insert paths). The catalog label "_Move<CPhysicsMeshInfo*,CPhysicsMeshInfo*>"
//   is a descriptive Dinkumware-style name; the build ships STLport 4.6.
// Identity: STLport 4.6 random-access `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)`;
//   the element assignment is CPhysicsMeshInfo's implicit operator= (Bank/Index, then CIVCountedPointer<CMeshData>
//   operator=: release the old pointee via Delete (slot 1) when its IVRefCount drops to 0, then addref the new one).
//   The vtable is not touched by assignment.
// Calling convention: free template under /Gr = __fastcall: first in ecx, last in edx, result / tag / distance
//   pointer on the stack; trip count (last - first) >> 4; returns result advanced by n.
class CMeshDataBank;

class CMeshDataBankEntry {                               // vtable 0x0129CD28
public:
    virtual void v00();
    CMeshDataBank* Bank;                                 // +0x04
    unsigned long  Index;                                // +0x08
};

class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);
    virtual void Delete();                               // slot 1
    unsigned long IVRefCount;                            // +0x04
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            CIVCountedPointeeBase* old = Object;
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

class CPhysicsMeshInfo : public CMeshDataBankEntry {    // vtable 0x0129CD50, 0x10 bytes
public:
    class CMeshData;
    virtual void v00();
    CIVCountedPointer<CMeshData> MeshData;               // +0x0C
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}

template CPhysicsMeshInfo* __fastcall std::__copy(CPhysicsMeshInfo*, CPhysicsMeshInfo*, CPhysicsMeshInfo*, const std::random_access_iterator_tag&, int*);