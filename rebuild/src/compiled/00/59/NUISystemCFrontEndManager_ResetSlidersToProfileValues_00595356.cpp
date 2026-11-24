// NUISystem::CFrontEndManager::ResetSlidersToProfileValues() -- re-read every slider and text slider on the top screen from the profile
// Leaf: ?ResetSlidersToProfileValues@CFrontEndManager@NUISystem@@QAEXXZ
// Retail: Fable.exe @ 0x00595356, 140 bytes, plain `ret`; one caller 0x00421159.
// Body: a local std::list<CComponent*> (ctor 0x0042ABAF with a default allocator temporary, clear 0x0042A18F, dtor
// 0x0042ABCA); m_stackScreens.top() (deque::back 0x0059B039) ->FindAllChildrenOfType(0x0f, list) (CComponent slot 6,
// +0x18, Ego_r.pdb ?FindAllChildrenOfType@CComponent@NUISystem@@UAEXW4ET...); each non-null entry gets
// CSlider::AdjustValueToProfile (0x0054A6F0, CSlider code range by RTTI); clear; the same with type 0x10 and
// CTextSlider::AdjustValueToProfile (0x00548B50, CTextSlider range). Both callee names are Ego_r.pdb QAEXXZ symbols.
// Layout: retail CFrontEndManager m_stackScreens at +0x20 (as the matched 0x005952D8); STLport list = one node pointer.
// Opt: size TU (unaligned entry) with a frame pointer (`leave`).
#pragma optimize("s", on)
#pragma optimize("y", off)
// NF42 (round 5): the local std::list / _List_node / _List_iterator / allocator stand-ins replaced by the shared
// stlport/fable_containers.h (list ctor / dtor COMDATs kept out of line: FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE overlay opt-in,
// FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE; clear() declaration-only), so clear binds STLport's _List_base<CComponent*>::clear.
// The deque / stack declarations below are the row's existing ones (unchanged).
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
// NF241 (2026-11-22, the list<CComponent*> family pass): the list calls bind _List_base<CComponent*>'s constructor 0x0042ABAF and
//   destructor 0x0042ABCA (was FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE + _DTOR_OUTOFLINE, binding list's own names there; retail
//   0x0042B6A6 / 0x0042B6D2 are list's size-optimised wrappers, a `call 0x0042ABAF` and a 5-byte `jmp 0x0042ABCA`). Same bytes.
#define FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE
#include "stlport/fable_containers.h"
namespace std {
// NF156 (2026-11-20, q6171): in-place respell of this row-local deque model to STLport 4.6's two template parameters (class allocator,
//   declared only), so back() binds 0x0059B039 as ?back@?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@
//   QAEAAPAVCComponent@NUISystem@@XZ, the self-row / Ego_r.pdb spelling (was ?back@?$deque@PAVCComponent@NUISystem@@@std@@QAE..).
template <class T> class allocator;
template <class T, class _Alloc = allocator<T> > class deque {
public:
    unsigned char _body[0x28];
    T& back();                                           // 0x0059B039 for CComponent*
};
template <class T, class C = deque<T> > class stack {
public:
    C c;
    T& top() { return c.back(); }
};
}
namespace NUISystem {
class CComponent {
public:
    enum EType { ETYPE_SLIDER = 0x0f, ETYPE_TEXT_SLIDER = 0x10 };   // values as pushed (names not recovered)
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005();
    virtual void FindAllChildrenOfType(EType type, std::list<CComponent*>& found);   // slot 6 (+0x18)
};
class CSlider : public CComponent {
public:
    void AdjustValueToProfile();                         // 0x0054A6F0
};
class CTextSlider : public CComponent {
public:
    void AdjustValueToProfile();                         // 0x00548B50
};
class CFrontEndManager {
public:
    unsigned char                  _pad00[0x20];
    std::stack<CComponent*>        m_stackScreens;       // +0x20 (Ego_r.pdb +0x2c)
    void ResetSlidersToProfileValues();
};
void CFrontEndManager::ResetSlidersToProfileValues()
{
    std::list<CComponent*> components;
    m_stackScreens.top()->FindAllChildrenOfType(CComponent::ETYPE_SLIDER, components);
    for (std::list<CComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    {
        CSlider* slider = static_cast<CSlider*>(*it);
        if (slider)
            slider->AdjustValueToProfile();
    }
    components.clear();
    m_stackScreens.top()->FindAllChildrenOfType(CComponent::ETYPE_TEXT_SLIDER, components);
    for (std::list<CComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    {
        CTextSlider* slider = static_cast<CTextSlider*>(*it);
        if (slider)
            slider->AdjustValueToProfile();
    }
}
}