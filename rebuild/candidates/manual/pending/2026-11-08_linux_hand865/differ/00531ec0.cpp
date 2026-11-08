// NUISystem::CComponent::Update(float deltaTime) -- advance the component's clock and its zoom / position / colour transitions, then push this component's render zoom, position and colour down to its position children and to every child it owns (live and dying), update those children and act on their deletion requests
// Grade: DIFFER(3455v3642) -- md=407 (structural 193): the first 543 instructions match; residue is VC7.1 inline-budget only: retail expands ConvertVector at the 4 ChildrenToDelete sites (calls at the first 8) and calls ~_List_base 0x0042ABCA for the UI_DELETE_THIS_INSTANCE local (ours expands it, so that local takes ebx and the deletion list head spills)
// Next idea: an inline ConvertVector body (same-TU body under FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR) inlines loops 2-3 instead of 3 only; a body that builds `C2DVector(x / ScreenWidth, ...)` gives retail's 8-call/4-inline split but no longer matches 0x0052E580 out of line -- find a 0x0052E580-exact spelling with that cost
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
//   0, ALL_INSTANCES, THIS_INSTANCE, PER_PARENT and ALL_INSTANCES shares PER_PARENT's RemoveChild tail (written with two gotos; a `bool remove` flag spelling gives the same tail at md=415 but flips one x87 pair).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR
#include "engine/C2DVector.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"

namespace std {
template <class _InputIter, class _Tp> inline _InputIter __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
template <class _RandomAccessIter, class _Tp> _RandomAccessIter __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val, const random_access_iterator_tag&);
template <class _Tp, class _Traits> inline bidirectional_iterator_tag __iterator_category(const _List_iterator<_Tp, _Traits>&) { return bidirectional_iterator_tag(); }
template <class _Tp> inline random_access_iterator_tag __iterator_category(_Tp* const&) { return random_access_iterator_tag(); }
template <class _InputIter, class _Tp> inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, __iterator_category(__first));
}
}


void NUISystem::CComponent::Update(float deltaTime)
{
    Time += deltaTime;
    UpdateZoom(deltaTime);
    UpdatePosition(deltaTime);
    UpdateColour(deltaTime);

    for (std::set<CComponent*>::iterator it = PositionChildren.begin(); it != PositionChildren.end(); ++it) {
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
                goto remove_child;
            } else if (deletion.Method == CDeletion::UI_DELETE_THIS_INSTANCE) {
                CDeletion new_deletion;
                new_deletion.Method = CDeletion::UI_DO_NOT_DELETE;
                Children[i]->SetDeletion(new_deletion);
                RemoveChild(i);
            } else if (deletion.Method == CDeletion::UI_DELETE_THIS_INSTANCE_PER_PARENT) {
                {
                    std::list<CComponent*>::iterator it = std::find(deletion.AssociatedParents.begin(), deletion.AssociatedParents.end(), this);
                    if (it == deletion.AssociatedParents.end())
                        goto next_child;
                    deletion.AssociatedParents.erase(it);
                    if (deletion.AssociatedParents.empty()) {
                        CDeletion new_deletion;
                        new_deletion.Method = CDeletion::UI_DO_NOT_DELETE;
                        Children[i]->SetDeletion(new_deletion);
                    }
                }
remove_child:
                RemoveChild(i);
            }
next_child:
            ;
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
