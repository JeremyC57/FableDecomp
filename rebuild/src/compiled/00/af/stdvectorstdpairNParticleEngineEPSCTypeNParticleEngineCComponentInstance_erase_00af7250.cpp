// std::vector<std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> >::erase(iterator position) -- STLport single erase: shift the tail down one entry, destroy the old last entry (release its component's counted pointer), return position
// Leaf: ?erase@?$vector@U?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@V?$allocator@U?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@@2@@std@@QAEPAU?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@2@PAU32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF7250, 108 bytes, thiscall, `ret 4`, returns position in eax.
// Identity: its callee 0x00AF6ED0 is bound as ??$__copy@PAU?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@PAU12@H@std@@...
//   (random-access __copy over the same 24-byte pair), and its only reference is the tail jump at 0x00AF44D4 inside the
//   CParticleSystem component code that owns Components, CVectorMap<EPSCType, CComponentInstance, CKeyPairCompareLess<..> >
//   (Ego_r.pdb CParticleSystem +0x0C). The erase name is probable (STLport 4.6 vector::erase(iterator) body; no PDB symbol
//   names STLport members, as the PDBs were built against Dinkumware).
// Callees: __copy(first, last, result, const random_access_iterator_tag&, int*) 0x00AF6ED0 (/Gr `__fastcall`), the count
//   block's destroy function (+4, object +8, `__fastcall`), operator delete 0x00BFE9BC, CBaseClass::~CBaseClass() 0x0099A2E0.
// Layout (Ego_r.pdb): pair.first EPSCType +0x00; CComponentInstance (0x14) +0x04: vptr, Component CCountedPointer<CPSCBase>
//   (+0x04 data, +0x08 count block), Enabled +0x0C, InstanceName +0x10. The implicit ~CComponentInstance stores no vptr.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC

class CCPPointerInfo {
public:
    long  RefCount;                                         // +0x00
    void (__fastcall* DeleteFunc)(void*);                   // +0x04
    void* Data;                                             // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    ~CCountedPointer()
    {
        if (Info != 0)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace NParticleEngine {
enum EPSCType { PSC_NONE = 0 };
class CPSCBase;
class CComponentInstance : public CBaseClass {              // vtable 0x0129F478
public:
    CCountedPointer<CPSCBase> Component;                    // +0x04
    bool                      Enabled;                      // +0x0C
    unsigned long             InstanceName;                 // +0x10
};
}   // namespace NParticleEngine

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

struct __false_type { };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x00AF6ED0

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator { };

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position);
};

template <class _Tp, class _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::erase(iterator __position)
{
    if (__position + 1 != end())
        __copy_ptrs(__position + 1, this->_M_finish, __position, __false_type());
    --this->_M_finish;
    _Destroy(this->_M_finish);
    return __position;
}

typedef pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> _ComponentEntry;
template vector<_ComponentEntry>::iterator vector<_ComponentEntry>::erase(iterator);

}   // namespace std