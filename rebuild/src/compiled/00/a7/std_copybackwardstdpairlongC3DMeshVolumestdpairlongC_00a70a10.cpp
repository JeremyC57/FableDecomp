// std::__copy_backward<std::pair<long,C3DMeshVolume>*, std::pair<long,C3DMeshVolume>*, int>(first, last, result,
// const random_access_iterator_tag&, int*) -- STLport random-access copy_backward over the 16-byte
// CPhysicsMesh::NavigationContainmentVolumes entries (pair<long, C3DMeshVolume>)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70A10, length 151 bytes (+ int3 padding).
// STLport `__copy_backward(first, last, result, random_access_iterator_tag, _Distance*)`, a /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); count = (last - first)
// >> 4, kept in a frame word; the result pointer lives in its argument slot (rewritten each step and returned from it).
// Callers: the NavigationContainmentVolumes sort helpers -- __linear_insert 0x00A728F0 (`cmp ecx,[esi]; jge`: a signed key
//   compare), __make_heap 0x00A71CA0, 0x00A71370, 0x00A72400.
// Element: std::pair<long,C3DMeshVolume>, 16 bytes: Ego_r.pdb CPhysicsMesh::NavigationContainmentVolumes is
//   CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> > (the signed key; the catalog's unsigned long
//   was a propagated manifest label) + Ego_r.pdb C3DMeshVolume (CCharString Name +0, CCountedPointer<CContainmentVolume>
//   ConvexVolume +4). The implicit operator= copies the key, calls the out-of-line CCharString::operator= @ 0x0099EFB0 for
//   the name, then runs the inlined CCountedPointer::operator= (source fields read first; when the count blocks differ
//   release the old block -- destroy function at +4 with the object, then operator delete @ 0x00BFE9BC -- adopt both words
//   and add a reference), modelled as the landed 0x0084F130.
// Leaf: ??$__copy_backward@PAU?$pair@JVC3DMeshVolume@@@std@@PAU12@H@std@@YIPAU?$pair@JVC3DMeshVolume@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
class CCharString {
public:
    void* m_pData;                           // +0x00
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

template <class T> class CCountedPointer {  // CCountedPointer<CContainmentVolume>, 8 bytes
public:
    void*        m_pObject;                  // +0
    SCountBlock* m_pCount;                   // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

class CContainmentVolume;

class C3DMeshVolume {                        // Ego_r.pdb, 12 bytes
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

struct random_access_iterator_tag { };

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                               const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

typedef pair<long, C3DMeshVolume> _CNavVolumeEntry;
template _CNavVolumeEntry* __fastcall __copy_backward(_CNavVolumeEntry*, _CNavVolumeEntry*, _CNavVolumeEntry*,
                                                      const random_access_iterator_tag&, int*);
}
