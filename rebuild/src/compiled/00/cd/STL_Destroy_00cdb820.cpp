// std::_Destroy(CHeroSoulsRoundDef* first, CHeroSoulsRoundDef* last) -- run the virtual destructor of every 56-byte hero-souls round def in [first, last)
// Leaf: ??$_Destroy@PAVCHeroSoulsRoundDef@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDB820, 34 bytes, __fastcall (first in ecx, last in edx), plain `ret`.
// Identity: the only caller is vector<CHeroSoulsRoundDef>::operator= (0x00F26C30, the shrinking branch at 0x00F26CDE:
//   `_Destroy(__copy(...), _M_finish)`), which CScriptDef::operator= (0x00F2A250) calls for the member at +0x1050 right
//   after Feats (+0x1038) and ArenaRounds (+0x1044): Ego_r.pdb CScriptDef declares Feats, ArenaRounds, HeroSoulsRounds
//   in that order. The stride 0x38 = 56 = Ego_r CHeroSoulsRoundDef (60) less 4 for its STLport Waves vector; the
//   destructor is virtual (FableWin.pdb ??1CHeroSoulsRoundDef@@UAE@XZ / ??_G...@@UAEPAXI@Z). The body is ICF-shared
//   with every 56-byte polymorphic range destroy, hence the address far from the def code.
//   Catalog "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::_Destroy" is propagated.
// Callees: the element's scalar deleting destructor through vtable slot 0 (`push 0 ; call [eax]`).
// Shape: STLport 4.6 `_Destroy(first, last)` from a /Gr TU: `__destroy(first, last, value_type)` ->
//   `__destroy_aux(first, last, __false_type())` -> `_Destroy(&*first)` = `first->~T()` all inlined; the explicit
//   virtual destructor call goes through the vtable with the "do not free" flag 0.
class CDefString {
public:
    long Index;
};

class CGameDefinitionBase {                   // 40-byte def base (vtable + def bookkeeping)
public:
    virtual ~CGameDefinitionBase();
    unsigned char _defData[0x24];
};

class CHeroSoulsWaveDef;

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
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
class vector : protected _Vector_base<_Tp, _Alloc> { };
}

class CHeroSoulsRoundDef : public CGameDefinitionBase {   // retail 56 bytes
public:
    long NumWaves;                                        // +0x28
    std::vector<CHeroSoulsWaveDef> Waves;                // +0x2C (12-byte STLport vector)
    virtual ~CHeroSoulsRoundDef();
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
inline void __fastcall __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator>
inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, (CHeroSoulsRoundDef*)0);
}

template void __fastcall _Destroy<CHeroSoulsRoundDef*>(CHeroSoulsRoundDef*, CHeroSoulsRoundDef*);
}