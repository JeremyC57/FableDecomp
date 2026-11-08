// Behaviour test for 0x00531EC0 NUISystem::CComponent::Update(float): the row body (copied from differ/00531ec0.cpp) runs against a
// model component whose virtuals record what they receive. Checked: the clock advance and the three own updates; for every combination
// of position-independent / zoom-independent / relative-position flags, the parent zoom, relative parent zoom, parent position and
// relative parent position pushed to a position child, an owned child and a dying child; the colour + Update push to owned children only
// (not to a foreign child whose PParent is someone else); SetParent(this) for parentless children; the four deletion methods (none, all
// instances, this instance -> SetDeletion(do-not-delete) + RemoveChild, per parent -> drop this parent, reset to do-not-delete when no
// parent is left, RemoveChild; a per-parent list without this parent does nothing); and a dying child in state 2 that just changed
// state being reset to state 0, unparented and dropped from ChildrenToDelete.
#include <stdio.h>
#include <list>
#include <set>
#include <vector>
#include <algorithm>
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
    const C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
};
struct CRGBColour { unsigned long Value; };
template<class T> struct CCountedPointer {
    T* P;
    CCountedPointer(T* p) : P(p) { }
    T* operator->() const { return P; }
    bool operator==(const CCountedPointer& o) const { return P == o.P; }
};
namespace NUISystem {
class CComponent;
struct CUIScreenUnitConverter { static C2DVector ConvertVector(C2DVector v) { return C2DVector(v.X * 2.0f, v.Y * 3.0f); } };
class CDeletion { public:
    enum EDeletionMethod { UI_DO_NOT_DELETE = 0, UI_DELETE_THIS_INSTANCE = 1, UI_DELETE_ALL_INSTANCES = 2, UI_DELETE_THIS_INSTANCE_PER_PARENT = 3 };
    EDeletionMethod Method;
    std::list<CComponent*> AssociatedParents;
    CDeletion() : Method(UI_DO_NOT_DELETE) { }
};
class CComponent { public:
    CComponent() : Time(0), PParent(0), PPositionParent(0), Flags(0), Got(0), Updates(0), OwnUpdates(0), SetDeletions(0), StateChanged(false),
        StateIndex(0), ChangedTo(-1), SetParentCalls(0) { ParentPosition = C2DVector(0, 0); }
    virtual ~CComponent() { }
    // the flags the row reads (descriptive bits of this model only)
    enum { POS_INDEP = 1, ZOOM_INDEP = 2, USE_REL = 4, BASTARD = 8 };
    virtual bool IsPositionIndependent() const { return (Flags & POS_INDEP) != 0; }
    virtual bool IsZoomIndependent() const { return (Flags & ZOOM_INDEP) != 0; }
    virtual bool IsUsingRelativePosition() const { return (Flags & USE_REL) != 0; }
    virtual bool IsBastardChild() const { return (Flags & BASTARD) != 0; }
    virtual void UpdateZoom(float) { ++OwnUpdates; }
    virtual void UpdatePosition(float) { ++OwnUpdates; }
    virtual void UpdateColour(float) { ++OwnUpdates; }
    virtual void SetParentZoom(C2DVector& v) { GotParentZoom = v; Got |= 1; }
    virtual void SetRelativeParentZoom(C2DVector& v) { GotRelativeParentZoom = v; Got |= 2; }
    virtual void SetParentPosition(C2DVector& v) { GotParentPosition = v; Got |= 4; }
    virtual void SetRelativeParentPosition(C2DVector& v) { GotRelativeParentPosition = v; Got |= 8; }
    virtual void SetParentColour(CRGBColour& c) { GotColour = c; Got |= 16; }
    virtual CComponent* GetParent() const { return PParent; }
    virtual void SetParent(CComponent* p) { PParent = p; ++SetParentCalls; }
    virtual CComponent* GetPositionParent() { return PPositionParent; }
    virtual CDeletion& GetDeletion() { return Deletion; }
    virtual void SetDeletion(CDeletion d) { Deletion = d; ++SetDeletions; }
    virtual void RemoveChild(unsigned long i) { Removed.push_back(i); }
    virtual bool ChangedState() { return StateChanged; }
    virtual unsigned long GetCurrentStateIndex() { return StateIndex; }
    virtual void ChangeState(unsigned long s) { ChangedTo = (long)s; }
    virtual void Update(float deltaTime);
    float Time;
    C2DVector Position, ParentPosition, ParentZoom, RenderZoom, RelativeRenderZoom, RelativeParentPosition, RelativeParentZoom;
    CRGBColour RenderColour;
    std::vector<CCountedPointer<CComponent> > Children, ChildrenToDelete;
    CComponent* PParent;
    CDeletion Deletion;
    CComponent* PPositionParent;
    std::set<CComponent*> PositionChildren;
    unsigned Flags, Got;
    int Updates, OwnUpdates, SetDeletions;
    bool StateChanged; unsigned long StateIndex; long ChangedTo; int SetParentCalls;
    C2DVector GotParentZoom, GotRelativeParentZoom, GotParentPosition, GotRelativeParentPosition;
    CRGBColour GotColour;
    std::vector<unsigned long> Removed;
};
// children record Update instead of recursing
class CLeaf : public CComponent { public: virtual void Update(float) { ++Updates; } };
}
using namespace NUISystem;

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

static int fails;
static void check(bool ok, int line, int combo) { if (!ok) { if (fails < 10) printf("fail line %d combo %d\n", line, combo); ++fails; } }
static bool eq(const C2DVector& a, float x, float y) { return a.X == x && a.Y == y; }
int main()
{
    for (unsigned combo = 0; combo < 8; ++combo) {
        CComponent root;
        root.Flags = combo;
        root.Time = 1.0f;
        root.Position = C2DVector(1, 2); root.ParentPosition = C2DVector(10, 20); root.ParentZoom = C2DVector(2, 3);
        root.RelativeParentPosition = C2DVector(100, 200); root.RelativeParentZoom = C2DVector(5, 7);
        root.RenderZoom = C2DVector(0.5f, 0.25f); root.RelativeRenderZoom = C2DVector(0.125f, 4);
        root.RenderColour.Value = 0x11223344;
        CLeaf posChild, own, foreign, dying, other;
        root.PositionChildren.insert(&posChild);
        own.PParent = 0;                                   // gets SetParent(this), then counts as owned
        foreign.PParent = &other;                          // not ours, no position parent: nothing pushed
        dying.PParent = &root; dying.StateChanged = true; dying.StateIndex = 2;
        root.Children.push_back(CCountedPointer<CComponent>(&own));
        root.Children.push_back(CCountedPointer<CComponent>(&foreign));
        root.ChildrenToDelete.push_back(CCountedPointer<CComponent>(&dying));
        root.Update(0.5f);
        check(root.Time == 1.5f && root.OwnUpdates == 3, __LINE__, combo);
        bool pi = (combo & 1) != 0, zi = (combo & 2) != 0, ur = (combo & 4) != 0;
        float ppx = pi ? (zi ? 1 : 2) : (zi ? 11 : 12), ppy = pi ? (zi ? 2 : 6) : (zi ? 22 : 26);
        float cx = ur ? 2 : 1, cy = ur ? 6 : 2;            // ConvertVector model scales by (2, 3)
        float rx = zi ? cx : cx * 5, ry = zi ? cy : cy * 7;
        if (!pi) { rx += 100; ry += 200; }
        CComponent* got[3] = { &posChild, &own, &dying };
        for (int k = 0; k < 3; ++k) {
            check((got[k]->Got & 15) == 15, __LINE__, combo);
            check(eq(got[k]->GotParentZoom, 0.5f, 0.25f) && eq(got[k]->GotRelativeParentZoom, 0.125f, 4), __LINE__, combo);
            check(eq(got[k]->GotParentPosition, ppx, ppy), __LINE__, combo);
            check(eq(got[k]->GotRelativeParentPosition, rx, ry), __LINE__, combo);
        }
        check(posChild.Updates == 0 && (posChild.Got & 16) == 0, __LINE__, combo);
        check(own.PParent == &root && own.SetParentCalls == 1 && own.Updates == 1 && (own.Got & 16) && own.GotColour.Value == 0x11223344, __LINE__, combo);
        check(foreign.Got == 0 && foreign.Updates == 0 && foreign.SetParentCalls == 0, __LINE__, combo);
        check(dying.Updates == 1 && dying.ChangedTo == 0 && dying.PParent == 0 && root.ChildrenToDelete.empty(), __LINE__, combo);
        check(root.Removed.empty(), __LINE__, combo);
    }
    // deletion methods on owned children
    {
        CComponent root; CLeaf a, b, c, d, e, other;
        CLeaf* kids[5] = { &a, &b, &c, &d, &e };
        for (int k = 0; k < 5; ++k) { kids[k]->PParent = &root; kids[k]->PPositionParent = &other; root.Children.push_back(CCountedPointer<CComponent>(kids[k])); }
        a.Deletion.Method = CDeletion::UI_DELETE_ALL_INSTANCES;
        b.Deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
        c.Deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE_PER_PARENT; c.Deletion.AssociatedParents.push_back(&other); c.Deletion.AssociatedParents.push_back(&root);
        d.Deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE_PER_PARENT; d.Deletion.AssociatedParents.push_back(&root);
        e.Deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE_PER_PARENT; e.Deletion.AssociatedParents.push_back(&other);
        root.Update(0.25f);
        check(root.Removed.size() == 4 && root.Removed[0] == 0 && root.Removed[1] == 1 && root.Removed[2] == 2 && root.Removed[3] == 3, __LINE__, 9);
        check(a.SetDeletions == 0 && a.Deletion.Method == CDeletion::UI_DELETE_ALL_INSTANCES, __LINE__, 9);
        check(b.SetDeletions == 1 && b.Deletion.Method == CDeletion::UI_DO_NOT_DELETE, __LINE__, 9);
        check(c.SetDeletions == 0 && c.Deletion.AssociatedParents.size() == 2, __LINE__, 9);   // the row edits its own copy
        check(d.SetDeletions == 1 && d.Deletion.Method == CDeletion::UI_DO_NOT_DELETE && d.Deletion.AssociatedParents.empty(), __LINE__, 9);
        check(e.SetDeletions == 0, __LINE__, 9);
        for (int k = 0; k < 5; ++k) check(kids[k]->Updates == 1 && kids[k]->Got == 16, __LINE__, 9);
    }
    // a dying child that did not change state, or is not in state 2, stays
    {
        CComponent root; CLeaf x, y;
        x.PParent = &root; x.StateChanged = false; x.StateIndex = 2; x.Deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
        y.PParent = &root; y.StateChanged = true; y.StateIndex = 1;
        root.ChildrenToDelete.push_back(CCountedPointer<CComponent>(&x));
        root.ChildrenToDelete.push_back(CCountedPointer<CComponent>(&y));
        root.Update(0.0f);
        check(root.ChildrenToDelete.size() == 2 && x.ChangedTo == -1 && y.ChangedTo == -1 && x.PParent == &root, __LINE__, 10);
        x.StateChanged = true;
        root.Update(0.0f);
        check(root.ChildrenToDelete.size() == 1 && x.ChangedTo == -1 && x.PParent == 0, __LINE__, 11);   // method != do-not-delete: no ChangeState
    }
    if (fails) { printf("00531ec0_TEST FAIL\n"); return 1; }
    printf("00531ec0_TEST PASS\n");
    return 0;
}
