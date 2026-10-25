// NUISystem::CIconText::ClearIcons() -- drop every embedded icon (Icons.clear()) and clear each icon's engine primitive handle
// Leaf: ?ClearIcons@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00542730, 71 bytes (80 with padding), plain `ret` (thiscall; Ego_r.pdb
//   `?ClearIcons@CIconText@NUISystem@@UAEXXZ`, CIconText vtable 0x012470CC slot 148, rtti-vtables.tsv).
// Callees: STLport __copy<CEmbeddedIcon*, CEmbeddedIcon*, int>(first, last, result, const random_access_iterator_tag&,
//   int*) 0x00543700 (__fastcall, the 7-dword `rep movsd` assignment; manifest "_Copy_impl<CActionInputControl*>",
//   propagated), for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> > 0x0043F360 (__fastcall,
//   the functor returned through the hidden pointer; manifest "_For_each<pair<long,CCharString> const*,
//   CAddDefNamesToComboBox>", propagated), CEnginePrimitiveHandle::Clear 0x0041BE70 (the member pointer pushed).
// Callers: the vtable entry 0x0124731C.
// Label correction: catalogued as "GetPackedNormalFromTargetVertex"; it is CIconText::ClearIcons.
// Layout: retail CIconText IconHandles (vector<CEnginePrimitiveHandle>, 8-byte elements) +0x180 and Icons
//   (vector<CEmbeddedIcon>, 28 bytes each) +0x18c (Ego_r.pdb +0x1b0 / +0x1c0 with 16-byte vectors). Local view.
// Shape: STLport `clear()` = `erase(begin(), end())` = `__copy_ptrs(last, _M_finish, first, __false_type())` (the
//   out-of-line __copy with the tag temporary in the high byte of a frame word and the null distance pointer), the
//   trivially-destroyed `_Destroy` range folds away, `_M_finish = i`; then `for_each(begin, end, mem_fun_ref(&Clear))`.
typedef int ptrdiff_t;

class CEnginePrimitiveHandle {                  // 8 bytes
public:
    virtual void v00();                         // +0x00 vptr (the PDB layout's leading word)
    void* Data;                                 // +0x04 CIVCountedPointer<CEnginePersistentPrimitiveBase>
    void Clear();                               // 0x0041BE70
};

class C2DVector {
public:
    float X;
    float Y;
};

namespace std {
struct random_access_iterator_tag {
    random_access_iterator_tag() {}
};
struct __false_type {
    __false_type() {}
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);              // 0x00543700
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class vector {             // STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, _M_finish, __first, __false_type());
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

template <class _Ret, class _Tp> class mem_fun_ref_t {
    typedef _Ret (_Tp::*__fun_type)(void);
public:
    explicit mem_fun_ref_t(__fun_type __pf) : _M_f(__pf) {}
    _Ret operator()(_Tp& __r) const { return (__r.*_M_f)(); }
private:
    __fun_type _M_f;
};
template <class _Ret, class _Tp> inline mem_fun_ref_t<_Ret, _Tp> mem_fun_ref(_Ret (_Tp::*__f)())
{
    return mem_fun_ref_t<_Ret, _Tp>(__f);
}
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);        // 0x0043F360
}

namespace NUISystem {
class CIconText {
public:
    enum EIconFlags { ICON_FLAGS_NONE };
    struct CEmbeddedIcon {                      // 28 bytes
        long       Graphic;                     // +0x00
        long       CharIndex;                   // +0x04
        EIconFlags Flags;                       // +0x08
        C2DVector  Size;                        // +0x0c
        C2DVector  Offset;                      // +0x14
    };
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147();
    virtual void ClearIcons();                                          // slot 148 (+0x250)
    unsigned char _body04[0x180 - 0x04];
    std::vector<CEnginePrimitiveHandle> IconHandles;                   // +0x180
    std::vector<CEmbeddedIcon>          Icons;                         // +0x18c
};

void CIconText::ClearIcons()
{
    Icons.clear();
    std::for_each(IconHandles.begin(), IconHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
}
}