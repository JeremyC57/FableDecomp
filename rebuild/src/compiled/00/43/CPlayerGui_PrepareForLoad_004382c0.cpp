// CPlayerGui::PrepareForLoad() -- tells every GUI illustrator (CDrawBase) to clear its primitives before a load
// Leaf: PrepareForLoad@CPlayerGui
// Retail: Fable.exe @ 0x004382C0, 30 bytes. Catalog label CWorldMap::PrepareForLoad is wrong: CWorldMap is 0xfc bytes, and the
// only caller CQuestManager::UpdateUI_QuestMenu 0x0056AEAD calls it on [0x013B8790], the same object it hands to
// CPlayerGui 0x00438400 just before. Ego_r.pdb ?PrepareForLoad@CPlayerGui@@QAEXXZ.
// Body: std::for_each(Illustrators.begin(), Illustrators.end(), std::mem_fun(&NPlayerGui::CDrawBase::ClearPrimitives)) --
// STLport for_each 0x0043F310 is __fastcall (hidden return slot in ecx, first in edx, last and the functor on the stack);
// 0x0043E430 is the compiler's vcall thunk for CDrawBase slot 1 (`mov eax,[ecx] ; jmp [eax+4]`; base body is a bare `ret`).
// Corrected (rename1, 2026-10-24): CDrawBase has no virtual destructor -- its vtables are two slots long (e.g.
//   CDrawMoney 0x0125A544, CDrawMiniMap 0x0125A634): slot 0 = ClearPrimitivesIfNecessary (landed per-class overrides
//   0x00646567, 0x00645829, 0x00643DE5; PDB ?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ) and slot 1 =
//   ClearPrimitives (landed CDrawMiniMap override 0x00644660, the empty base 0x00442760; PDB
//   ?ClearPrimitives@CDrawBase@NPlayerGui@@UAEXXZ). The earlier comments called slot 0 a virtual destructor and
//   slot 1 an inferred "PrepareForLoad"; bytes are unchanged (no vtable is emitted here).
// Renamed (rename1, 2026-10-24) from the catalog name CWorldMap::PrepareForLoad (this source, its symbol and the ps1
//   Module already said CPlayerGui).
// Layout: Illustrators (STLport vector<NPlayerGui::CDrawBase*>) at retail +0x2c0 (Ego_r.pdb +0x2d0); engine/CPlayerGui.h has
// the PDB offsets, so the class is modelled locally as in ForceOpen 0x004374D0.
namespace NPlayerGui {
class CDrawBase {
public:
    virtual void ClearPrimitivesIfNecessary();           // slot 0
    virtual void ClearPrimitives();                      // slot 1
};
}
namespace std {
template <class Arg, class Result> struct unary_function { typedef Arg argument_type; typedef Result result_type; };
template <class R, class T> class mem_fun_t : public unary_function<T*, R> {
    typedef R (T::*fun_type)();
    fun_type f;
public:
    explicit mem_fun_t(fun_type pf) : f(pf) {}
    R operator()(T* p) const { return (p->*f)(); }
};
template <class R, class T> inline mem_fun_t<R, T> mem_fun(R (T::*f)()) { return mem_fun_t<R, T>(f); }
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f);      // 0x0043F310 (out of line)
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};
}
class CPlayerGui {
public:
    unsigned char                         _pad00[0x2c0];
    std::vector<NPlayerGui::CDrawBase*>   Illustrators;  // +0x2c0
    void PrepareForLoad();
};
void CPlayerGui::PrepareForLoad()
{
    std::for_each(Illustrators.begin(), Illustrators.end(), std::mem_fun(&NPlayerGui::CDrawBase::ClearPrimitives));
}