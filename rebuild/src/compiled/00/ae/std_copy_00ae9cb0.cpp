// std::__copy(_E* first, _E* last, _E* result, const random_access_iterator_tag&, int*) -- (_E = pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> >) assign [first, last) onto result
// Leaf: ??$__copy@PAU?$pair@JV?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@@std@@PAU12@H@std@@YIPAU?$pair@JV?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE9CB0, 145 bytes, `__fastcall` (first ecx, last edx), `ret 0Ch`.
// Identity (proven): NParticleEngine::CParticleEmitterDatabase::DeleteEmitterTemplate 0x00AE8B50 erases with it: Ego_r.pdb CParticleEmitterDatabase::Emitters is CVectorMap<long, CCountedPointer<NParticleEngine::CParticleEmitter> >.
// Callees: the count block's delete function (`call [eax+4]` with the data in ecx), operator delete 0x00BFE9BC.
// Shape (family M029, representative 0x004487C0): STLport `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result =
//   *__first; ++__first; ++__result; } return __result;` -- 12-byte elements (the 0x2AAAAAAB divide); CCountedPointer::operator=
//   reads the source Info then Data into locals before the self test.
void __cdecl operator delete(void* p);
class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info) ++info->RefCount;
        }
        return *this;
    }
    T* Data;
    CCPPointerInfo* Info;
};
namespace NParticleEngine { class CParticleEmitter; }
namespace std {
struct random_access_iterator_tag { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
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
typedef pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> > _E;
template _E* __fastcall __copy<_E*, _E*, int>(_E*, _E*, _E*, const random_access_iterator_tag&, int*);
}