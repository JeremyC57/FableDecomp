// std::__destroy_aux(CIntelligentPointer<NParticleEngine::CParticleEmitter>* first, CIntelligentPointer<...>* last, const __false_type&) -- destroy each intelligent pointer in [first, last) through its virtual destructor
// Leaf: ??$__destroy_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043F850, 36 bytes (+ int3 padding), /Gr `__fastcall` (first in ecx -> esi, last
//   in edx -> edi), `ret 4` (the tag reference).
// Identity: STLport `__destroy_aux(first, last, __false_type)` at the 8-byte CIntelligentPointer stride, the pointer's
//   virtual destructor (CBaseIntelligentPointer's, slot 0) called with flag 0. Callers: the /Os vector destructor
//   0x0065605F and erase(first, last) 0x00656202 of an intelligent-pointer vector (its element assignment 0x0065533C is
//   CIntelligentPointer::operator=, `SetPItem(o.GetPItem())`); every CIntelligentPointer<T> instantiation folds to this
//   body, so the catalog's element type (NParticleEngine::CParticleEmitter) is kept.
// Shape: `for (; first != last; ++first) _Destroy(&*first);` in a speed TU (entry test, aligned loop head).
class CBaseObject;
namespace NParticleEngine { class CParticleEmitter; }

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10 (via the vtable here)
    CBaseObject* PItem;                                 // +0x04
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v01();
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> TEmitterPointer;
template void __fastcall __destroy_aux<TEmitterPointer*>(TEmitterPointer*, TEmitterPointer*, const __false_type&);
}