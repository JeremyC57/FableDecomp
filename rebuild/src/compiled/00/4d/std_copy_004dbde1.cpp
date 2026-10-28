// std::__copy(std::pair<CCharString, CTCBase::CRegisteredTCInfo>* first, std::pair<CCharString, CTCBase::CRegisteredTCInfo>* last, std::pair<CCharString, CTCBase::CRegisteredTCInfo>* result, const random_access_iterator_tag&, int*) -- STLport counted forward assignment of pair<CCharString, CTCBase::CRegisteredTCInfo> elements; returns the advanced result
// Leaf: ??$__copy@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DBDE1, 45 bytes, __fastcall (first ecx, last edx; result, the tag reference and the distance pointer on the stack), `ret 0Ch`; size-optimised TU (result advanced in place, `add dword ptr [esp+0Ch],0x10`).
// Identity (proven): std::__copy<std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, int>; evidence: the 16-byte element's assignment 0x004DA56A runs CCharString::operator= on +0 and then 0x004D78C5 on +4, which copies two dwords and a CCharString -- Ego_r.pdb CTCBase::CRegisteredTCInfo {AllocFunc, DriverType, Name}; the element is the pair<CCharString, CTCBase::CRegisteredTCInfo> of the TC registry's CVectorMap (Ego_r), and the live caller 0x004E1B5D is that vector's erase(first, last) (`__copy_ptrs(last, finish, first)` then _Destroy 0x004DBE8E), reached from Init_RegisterGameComponents 0x004EE23F: mutable iterators.
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*, ...>" is propagated (family H060: six out-of-line __copy loops, one per element).
// Callee: pair<CCharString, CTCBase::CRegisteredTCInfo>::operator= 0x004DA56A (the element assignment).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, random_access_iterator_tag, Distance*)` as written (the landed CWideString
//   0x00433CEC / CArray<unsigned short> 0x00F3B8A0 bodies), `__fastcall` on the template definition.
#pragma optimize("s", on)
class CCharString {                                        // 4 bytes: the shared string data pointer
public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
class CTCBase {
public:
    class CRegisteredTCInfo {                               // Ego_r.pdb, 12 bytes
    public:
        void* AllocFunc;
        unsigned long DriverType;
        CCharString Name;
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair& operator=(const pair& other);                     // the implicit assignment, out of line
};
}

namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template std::pair<CCharString, CTCBase::CRegisteredTCInfo>* __fastcall __copy<std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, int>(std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, const random_access_iterator_tag&, int*);
}