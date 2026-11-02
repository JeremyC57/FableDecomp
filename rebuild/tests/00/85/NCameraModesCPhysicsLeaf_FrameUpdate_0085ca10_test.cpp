// Behaviour test for CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelRigid, CPhysicsCoordRigid>::FrameUpdate (0x0085CA10):
// force then coordinate frame update, then the owner's go-to physics state is reset (CullPhysics off, both attractor lists empty).
#include <cstdio>
static int order[8]; static int n = 0;
struct Node { Node* next; Node* prev; float x, y, z, w; };
class AttractorList {                                   // a sentinel list like STLport's
public:
    Node head;
    int frees;
    AttractorList() : frees(0) { head.next = head.prev = &head; }
    void push(Node* p) { p->next = &head; p->prev = head.prev; head.prev->next = p; head.prev = p; }
    bool empty() const { return head.next == &head; }
    void clear()
    {
        Node* cur = head.next;
        while (cur != &head) { Node* t = cur; cur = cur->next; ++frees; (void)t; }
        head.next = head.prev = &head;
    }
};
class CPhysicsState {
public:
    void Reset() { CullPhysics = false; Attractors.clear(); IncidentalAttractors.clear(); }
    AttractorList Attractors, IncidentalAttractors;
    bool CullPhysics;
};
class CBase { public: CPhysicsState GoTo; CPhysicsState& GetGoToPhase() { return GoTo; } };
class CSubComponentBase {
public:
    virtual void FrameUpdate() = 0;
    virtual ~CSubComponentBase() { }
protected:
    CBase& GetOwner() { return *Owner; }
public:
    CBase* Owner;
};
class CPhysicsTargetModelGoTo : public CSubComponentBase {
public:
    CPhysicsState& GetPhysicsState() { return GetOwner().GetGoToPhase(); }
};
class CForce { public: virtual void FrameUpdate() { order[n++] = 1; } virtual ~CForce() { } };
class CCoord { public: virtual void FrameUpdate() { order[n++] = 2; } virtual ~CCoord() { } };
template <class T> class scoped_ptr { public: T* ptr; T* operator->() const { return ptr; } };
template <class TTarget>
class CPhysicsLeaf : public TTarget {
public:
    virtual void FrameUpdate()
    {
        OurForce->FrameUpdate();
        OurCoords->FrameUpdate();
        this->GetPhysicsState().Reset();
    }
    scoped_ptr<CForce> OurForce;
    scoped_ptr<CCoord> OurCoords;
};
int main()
{
    CBase owner; owner.GoTo.CullPhysics = true;
    Node a, b, c; owner.GoTo.Attractors.push(&a); owner.GoTo.Attractors.push(&b); owner.GoTo.IncidentalAttractors.push(&c);
    CForce f; CCoord co;
    CPhysicsLeaf<CPhysicsTargetModelGoTo> leaf; leaf.Owner = &owner; leaf.OurForce.ptr = &f; leaf.OurCoords.ptr = &co;
    CSubComponentBase* s = &leaf;
    s->FrameUpdate();
    if (n != 2 || order[0] != 1 || order[1] != 2 || owner.GoTo.CullPhysics || !owner.GoTo.Attractors.empty()
        || !owner.GoTo.IncidentalAttractors.empty() || owner.GoTo.Attractors.frees != 2 || owner.GoTo.IncidentalAttractors.frees != 1) {
        std::printf("0085ca10_TEST FAIL\n"); return 1;
    }
    std::printf("0085ca10_TEST PASS\n");
    return 0;
}