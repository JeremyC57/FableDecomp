// std::find(CCharString* first, CCharString* last, const CCharString& val) -- STLport find: forward to __find with the random-access iterator tag
// Leaf: ??$find@PAVCCharString@@V1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B9E20, 18 bytes, /Gr `__fastcall` (first ecx, last edx, the value reference on
//   the stack): `lea eax,[esp+4] ; push eax ; mov eax,[esp+8] ; push eax ; call 0x004B8E40 ; ret 4`.
// Identity: the sole caller is CTCInventoryExperience::IsTutorialPlayed 0x0057598A (landed), whose
//   `find(TutorialsPlayed.begin(), TutorialsPlayed.end(), name)` runs over the PDB member
//   `vector<CCharString> TutorialsPlayed` (Ego_r CTCInventoryExperience +0x2A8; retail +0x248), so the instantiation is
//   find<CCharString*, CCharString>; the callee is the landed std::__find<CCharString*, CCharString> 0x004B8E40
//   (12 callers, all `find(v.begin(), v.end(), name)` over a vector<CCharString>). The manifest label
//   "vector<EWeaponClass>::_Umove<EWeaponClass*>" is a propagated label of a byte-identical body.
// Shape: STLport 4.6 `find(__first, __last, __val)` = `__find(__first, __last, __val,
//   _STLP_ITERATOR_CATEGORY(__first, _InputIter))`; the empty `random_access_iterator_tag` temporary is homed in the
//   value reference's own argument slot (`lea eax,[esp+4]`).
class CCharStringData { public:                  // the shared string block
    const char*   Chars;                  // +0x00
    unsigned long Length;                 // +0x04
};

class CCharString { public:
    CCharStringData* PStringData;         // +0x00
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&);       // 0x004B8E40

template <class _InputIter, class _Tp>
_InputIter __fastcall find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}

template CCharString* __fastcall find<CCharString*, CCharString>(CCharString*, CCharString*, const CCharString&);
}