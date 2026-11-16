// NUISystem::CComponent::Update(float deltaTime) -- advance the component's clock and its zoom / position / colour transitions, then push this component's render zoom, position and colour down to its position children and to every child it owns (live and dying), update those children and act on their deletion requests
// Grade: RELOCATION_MATCH (wfD5 successor 5, 2026-11-15) -- rule-clean: no __assume device; last-resort devices a (FITTED guards), c (k = 2 unused locals) and d (2 dead FABLE_RETAIL_ASSERT arms), all marked below.
// EVIDENCED: ConvertVector written through ConvertX / ConvertY (existing opt-in FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE_CONVERTXY of the matched row 0x0061E6D0, FableWin ConvertVector 28i/81b v this text 27i/77b v the direct-global text 29i/95b): it closes the loop-3 `ParentPosition + Position` X pair at 0x005329E8 (the expanded per-axis helpers add c2 records in loop 3, the only loop that expands ConvertVector).
// FITTED (device a, no bytes): loop 1 has its iterator declared first and THREE redundant `if (it != PositionChildren.end())` entry tests that c2 folds into the loop's own entry test (same device as the landed 0x00607330 / staged 0x005FF190 single guards). They lower c2's static frequency after loop 1 (for-loop leak 5 -> 1.5 = 1/2 + 1/4 + 1/8 + 5/8; measured window from the inline trace: loop-2 divisor k in (3.20, 4.27)), so VC7.1 calls ConvertVector at the four loop-2 sites and ~_List_base at the two loop-2 CDeletion sites and expands vector::erase in loop 3, as retail (106 calls). One or two guards leave the loop-2 sites expanded (3918 / 3898 bytes), four overshoot (3610); a loop-2 guard never folds (+16 bytes, every spelling).
// Fidelity (device c): `long u0 = 0; long u1 = 0;` unused named locals re-key c2's record count for the x87 operand order of the loop-1 / loop-2 `ConvertVector(Position) + RelativeParentPosition` pairs (0x005320C2 / 0x005320CE, 0x00532475) and the loop-3 `ParentPosition + Position` pair (0x005329E8); period 8 (k = 2 and k = 10 both match), smallest k = 2.
// Inferred (device d): TWO dead release asserts (FABLE_RETAIL_ASSERT, FABLE_RETAIL_ASSERT_DEAD_ARM, overlay fable_assert.h; emit no bytes). `PParent != this` before loop 1 (a component is not its own parent; same grade before loop 2 or loop 3) and `i < ChildrenToDelete.size()` in loop 3 between the `ParentPosition + Position` sum and the ChildrenToDelete[i] subscript that follows (bounds check; same grade after that call; NOT at the loop-3 top or before the sum). Their condition records close the 0x00532B58 `Position + RelativeParentPosition` pair. FableWin's Update is +675b over the /Od row (2-3 small asserts); conditions FITTED by grading.
// EVIDENCED: the deletion chain is a plain if / else-if with RemoveChild(i) in each arm (retail's cross-jumped `mov ecx,esi ; push edi ; call [eax+100h]` at 0x00532747).
// EVIDENCED (wfD5 successor 3): the same-TU landed text of std::__find<CCountedPointer<CComponent>*, ...> (0x00534EB0, rebuild/src/compiled/00/53/std_find_00534eb0.cpp) defined above the row __declspec(noinline) under auto_inline(off), with FableWin's free __fastcall operator== ??$?8, and the find forwarder through iterator_category(__first) -- exactly as the landed same-TU sibling RemoveChild 0x00533BC0; it closed the std::find load-order cluster at 0x00532C46 (c2 then knows __find does not capture &val).
// Leaf: ?Update@CComponent@NUISystem@@UAEXM@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00531EC0, 3642 bytes, `ret 4`, /Gr TU (STLport __find 0x00534EB0 / __copy 0x00535000 are __fastcall).
// Identity: rtti NUISystem::CComponent vtable 0x0124608C slot 1; Ego_r.pdb 0x005AD350 and FableWin.pdb ?Update@CComponent@NUISystem@@UAEXM@Z.
//   Catalog "CGuiComponent::Update" is wrong.
// Callees: own virtual UpdateZoom / UpdatePosition / UpdateColour, CUIScreenUnitConverter::ConvertVector 0x0052E580 (static __fastcall),
//   the child virtuals SetParentZoom / SetRelativeParentZoom / SetParentPosition / SetRelativeParentPosition / GetParent / SetParent /
//   GetPositionParent / IsBastardChild / SetParentColour / Update / GetDeletion / SetDeletion / ChangedState / GetCurrentStateIndex /
//   ChangeState, own RemoveChild (slot +0x100), CDeletion copy ctor 0x0042CD84, list insert 0x0042B4C3, ~_List_base 0x0042ABCA.
// Shape: x87 pairs need `ParentPosition + Position` / `Position + RelativeParentPosition` / `ConvertVector(Position) + RelativeParentPosition`
//   (member operator+ loads the reference operand first); `this == PParent` gives `cmp esi,[ecx+0C8h]`; the deletion tests run
//   0, ALL_INSTANCES, THIS_INSTANCE, PER_PARENT (plain if / else-if chain, RemoveChild(i) in every arm; c2 cross-jumps the identical calls).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE_CONVERTXY
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "engine/C2DVector.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"

template <class T, class U>
inline bool __fastcall operator==(const CCountedPointer<T>& lhs, const CCountedPointer<U>& rhs)   // FableWin ??$?8
{
    return lhs.Get() == rhs.Get();
}
namespace std {
template <class _InputIter, class _Tp> inline _InputIter __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last,
                                                         const _Tp& __val, const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)
inline bidirectional_iterator_tag iterator_category(const _List_iterator_base&) { return bidirectional_iterator_tag(); }
template <class _Tp> inline random_access_iterator_tag iterator_category(_Tp*) { return random_access_iterator_tag(); }
template <class _InputIter, class _Tp> inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, iterator_category(__first));
}
}


void NUISystem::CComponent::Update(float deltaTime)
{
    Time += deltaTime;
    UpdateZoom(deltaTime);
    UpdatePosition(deltaTime);
    UpdateColour(deltaTime);
    FABLE_RETAIL_ASSERT(PParent != this);   // Inferred: dead release assert 1 of 2 (condition FITTED)
    long u0 = 0;   // Fidelity: unused, re-keys the x87 pair order (k = 2)
    long u1 = 0;   // Fidelity: unused

    std::set<CComponent*>::iterator it = PositionChildren.begin();
    if (it != PositionChildren.end())   // FITTED: three redundant entry tests, folded by c2 (no bytes)
    if (it != PositionChildren.end())
    if (it != PositionChildren.end())
    for (; it != PositionChildren.end(); ++it) {
        (*it)->SetParentZoom(RenderZoom);
        (*it)->SetRelativeParentZoom(RelativeRenderZoom);
        if (IsPositionIndependent()) {
            if (IsZoomIndependent())
                (*it)->SetParentPosition(Position);
            else {
                C2DVector position = Position;
                position.X *= ParentZoom.X;
                position.Y *= ParentZoom.Y;
                (*it)->SetParentPosition(position);
            }
            if (IsUsingRelativePosition()) {
                if (IsZoomIndependent()) {
                    C2DVector position = CUIScreenUnitConverter::ConvertVector(Position);
                    (*it)->SetRelativeParentPosition(position);
                } else {
                    C2DVector position = CUIScreenUnitConverter::ConvertVector(Position);
                    position.X *= RelativeParentZoom.X;
                    position.Y *= RelativeParentZoom.Y;
                    (*it)->SetRelativeParentPosition(position);
                }
            } else {
                if (IsZoomIndependent())
                    (*it)->SetRelativeParentPosition(Position);
                else {
                    C2DVector position = Position;
                    position.X *= RelativeParentZoom.X;
                    position.Y *= RelativeParentZoom.Y;
                    (*it)->SetRelativeParentPosition(position);
                }
            }
        } else {
            if (IsZoomIndependent()) {
                C2DVector position = ParentPosition + Position;
                (*it)->SetParentPosition(position);
            } else {
                C2DVector position = Position;
                position.X *= ParentZoom.X;
                position.Y *= ParentZoom.Y;
                position += ParentPosition;
                (*it)->SetParentPosition(position);
            }
            if (IsUsingRelativePosition()) {
                if (IsZoomIndependent()) {
                    C2DVector position = CUIScreenUnitConverter::ConvertVector(Position) + RelativeParentPosition;
                    (*it)->SetRelativeParentPosition(position);
                } else {
                    C2DVector position = CUIScreenUnitConverter::ConvertVector(Position);
                    position.X *= RelativeParentZoom.X;
                    position.Y *= RelativeParentZoom.Y;
                    position += RelativeParentPosition;
                    (*it)->SetRelativeParentPosition(position);
                }
            } else {
                if (IsZoomIndependent()) {
                    C2DVector position = Position + RelativeParentPosition;
                    (*it)->SetRelativeParentPosition(position);
                } else {
                    C2DVector position = Position;
                    position.X *= RelativeParentZoom.X;
                    position.Y *= RelativeParentZoom.Y;
                    position += RelativeParentPosition;
                    (*it)->SetRelativeParentPosition(position);
                }
            }
        }
    }

    for (unsigned long i = 0; i < Children.size(); ++i) {
        if (Children[i]->GetParent() == 0)
            Children[i]->SetParent(this);
        if ((this == Children[i]->PParent && Children[i]->GetPositionParent() == 0) || Children[i]->GetPositionParent() == this || Children[i]->IsBastardChild()) {
            Children[i]->SetParentZoom(RenderZoom);
            Children[i]->SetRelativeParentZoom(RelativeRenderZoom);
            if (IsPositionIndependent()) {
                if (IsZoomIndependent())
                    Children[i]->SetParentPosition(Position);
                else {
                    C2DVector position = Position;
                    position.X *= ParentZoom.X;
                    position.Y *= ParentZoom.Y;
                    Children[i]->SetParentPosition(position);
                }
                if (IsUsingRelativePosition()) {
                    if (IsZoomIndependent()) {
                        C2DVector position = CUIScreenUnitConverter::ConvertVector(Position);
                        Children[i]->SetRelativeParentPosition(position);
                    } else {
                        C2DVector position = CUIScreenUnitConverter::ConvertVector(Position);
                        position.X *= RelativeParentZoom.X;
                        position.Y *= RelativeParentZoom.Y;
                        Children[i]->SetRelativeParentPosition(position);
                    }
                } else {
                    if (IsZoomIndependent())
                        Children[i]->SetRelativeParentPosition(Position);
                    else {
                        C2DVector position = Position;
                        position.X *= RelativeParentZoom.X;
                        position.Y *= RelativeParentZoom.Y;
                        Children[i]->SetRelativeParentPosition(position);
                    }
                }
            } else {
                if (IsZoomIndependent()) {
                    C2DVector position = ParentPosition + Position;
                    Children[i]->SetParentPosition(position);
                } else {
                    C2DVector position = Position;
                    position.X *= ParentZoom.X;
                    position.Y *= ParentZoom.Y;
                    position += ParentPosition;
                    Children[i]->SetParentPosition(position);
                }
                if (IsUsingRelativePosition()) {
                    if (IsZoomIndependent()) {
                        C2DVector position = CUIScreenUnitConverter::ConvertVector(Position) + RelativeParentPosition;
                        Children[i]->SetRelativeParentPosition(position);
                    } else {
                        C2DVector position = CUIScreenUnitConverter::ConvertVector(Position);
                        position.X *= RelativeParentZoom.X;
                        position.Y *= RelativeParentZoom.Y;
                        position += RelativeParentPosition;
                        Children[i]->SetRelativeParentPosition(position);
                    }
                } else {
                    if (IsZoomIndependent()) {
                        C2DVector position = Position + RelativeParentPosition;
                        Children[i]->SetRelativeParentPosition(position);
                    } else {
                        C2DVector position = Position;
                        position.X *= RelativeParentZoom.X;
                        position.Y *= RelativeParentZoom.Y;
                        position += RelativeParentPosition;
                        Children[i]->SetRelativeParentPosition(position);
                    }
                }
            }
        }
        if (this == Children[i]->PParent || Children[i]->IsBastardChild()) {
            Children[i]->SetParentColour(RenderColour);
            Children[i]->Update(deltaTime);
            CDeletion deletion = Children[i]->GetDeletion();
            if (deletion.Method == CDeletion::UI_DO_NOT_DELETE) {
            } else if (deletion.Method == CDeletion::UI_DELETE_ALL_INSTANCES) {
                RemoveChild(i);
            } else if (deletion.Method == CDeletion::UI_DELETE_THIS_INSTANCE) {
                CDeletion new_deletion;
                new_deletion.Method = CDeletion::UI_DO_NOT_DELETE;
                Children[i]->SetDeletion(new_deletion);
                RemoveChild(i);
            } else if (deletion.Method == CDeletion::UI_DELETE_THIS_INSTANCE_PER_PARENT) {
                std::list<CComponent*>::iterator it = std::find(deletion.AssociatedParents.begin(), deletion.AssociatedParents.end(), this);
                if (it != deletion.AssociatedParents.end()) {
                    deletion.AssociatedParents.erase(it);
                    if (deletion.AssociatedParents.empty()) {
                        CDeletion new_deletion;
                        new_deletion.Method = CDeletion::UI_DO_NOT_DELETE;
                        Children[i]->SetDeletion(new_deletion);
                    }
                    RemoveChild(i);
                }
            }
        }
    }

    for (unsigned long i = 0; i < ChildrenToDelete.size(); ++i) {
        if (ChildrenToDelete[i]->GetParent() == 0)
            ChildrenToDelete[i]->SetParent(this);
        if ((this == ChildrenToDelete[i]->PParent && ChildrenToDelete[i]->GetPositionParent() == 0) || ChildrenToDelete[i]->GetPositionParent() == this || ChildrenToDelete[i]->IsBastardChild()) {
            ChildrenToDelete[i]->SetParentZoom(RenderZoom);
            ChildrenToDelete[i]->SetRelativeParentZoom(RelativeRenderZoom);
            if (IsPositionIndependent()) {
                if (IsZoomIndependent())
                    ChildrenToDelete[i]->SetParentPosition(Position);
                else {
                    C2DVector position = Position;
                    position.X *= ParentZoom.X;
                    position.Y *= ParentZoom.Y;
                    ChildrenToDelete[i]->SetParentPosition(position);
                }
                if (IsUsingRelativePosition()) {
                    if (IsZoomIndependent()) {
                        C2DVector position = CUIScreenUnitConverter::ConvertVector(Position);
                        ChildrenToDelete[i]->SetRelativeParentPosition(position);
                    } else {
                        C2DVector position = CUIScreenUnitConverter::ConvertVector(Position);
                        position.X *= RelativeParentZoom.X;
                        position.Y *= RelativeParentZoom.Y;
                        ChildrenToDelete[i]->SetRelativeParentPosition(position);
                    }
                } else {
                    if (IsZoomIndependent())
                        ChildrenToDelete[i]->SetRelativeParentPosition(Position);
                    else {
                        C2DVector position = Position;
                        position.X *= RelativeParentZoom.X;
                        position.Y *= RelativeParentZoom.Y;
                        ChildrenToDelete[i]->SetRelativeParentPosition(position);
                    }
                }
            } else {
                if (IsZoomIndependent()) {
                    C2DVector position = ParentPosition + Position;
                    FABLE_RETAIL_ASSERT(i < ChildrenToDelete.size());   // Inferred: dead release assert 2 of 2 (place / condition FITTED)
                    ChildrenToDelete[i]->SetParentPosition(position);
                } else {
                    C2DVector position = Position;
                    position.X *= ParentZoom.X;
                    position.Y *= ParentZoom.Y;
                    position += ParentPosition;
                    ChildrenToDelete[i]->SetParentPosition(position);
                }
                if (IsUsingRelativePosition()) {
                    if (IsZoomIndependent()) {
                        C2DVector position = CUIScreenUnitConverter::ConvertVector(Position) + RelativeParentPosition;
                        ChildrenToDelete[i]->SetRelativeParentPosition(position);
                    } else {
                        C2DVector position = CUIScreenUnitConverter::ConvertVector(Position);
                        position.X *= RelativeParentZoom.X;
                        position.Y *= RelativeParentZoom.Y;
                        position += RelativeParentPosition;
                        ChildrenToDelete[i]->SetRelativeParentPosition(position);
                    }
                } else {
                    if (IsZoomIndependent()) {
                        C2DVector position = Position + RelativeParentPosition;
                        ChildrenToDelete[i]->SetRelativeParentPosition(position);
                    } else {
                        C2DVector position = Position;
                        position.X *= RelativeParentZoom.X;
                        position.Y *= RelativeParentZoom.Y;
                        position += RelativeParentPosition;
                        ChildrenToDelete[i]->SetRelativeParentPosition(position);
                    }
                }
            }
        }
        if (this == ChildrenToDelete[i]->PParent || ChildrenToDelete[i]->IsBastardChild()) {
            ChildrenToDelete[i]->SetParentColour(RenderColour);
            ChildrenToDelete[i]->Update(deltaTime);
            if (ChildrenToDelete[i]->ChangedState() && ChildrenToDelete[i]->GetCurrentStateIndex() == 2) {
                std::vector<CCountedPointer<CComponent> >::iterator it = std::find(ChildrenToDelete.begin(), ChildrenToDelete.end(), ChildrenToDelete[i]);
                if ((*it)->Deletion.Method == CDeletion::UI_DO_NOT_DELETE)
                    (*it)->ChangeState(0);
                (*it)->SetParent(0);
                ChildrenToDelete.erase(it);
            }
        }
    }
}