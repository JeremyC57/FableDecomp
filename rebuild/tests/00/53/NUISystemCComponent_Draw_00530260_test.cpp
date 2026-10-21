// Behaviour test for NUISystem::CComponent::Draw (0x00530260): render-layer selection and the child filters
// (owned or bastard child, not drawn from a viewport) over Children then ChildrenToDelete.
#include <cstdio>

struct CIEngine { int dummy; };
struct CEnginePrimitiveHandle { int dummy; };
struct CComponent;
struct CCountedComponentPointer {
    CComponent* Object;
    void* Info;
    CComponent* operator->() const { return Object; }
};
struct CComponentPointerVector {
    CCountedComponentPointer* _M_start;
    CCountedComponentPointer* _M_finish;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CCountedComponentPointer& operator[](unsigned int i) { return _M_start[i]; }
};
struct CComponent {
    CComponentPointerVector Children, ChildrenToDelete;
    unsigned char Flags, FlagsSetTwo;
    char Layer;
    CComponent* Parent;
    int  DrawCalls;
    long LastLayer;
    CComponent* LastDrawer;
    virtual ~CComponent() {}
    virtual CComponent* GetParent() { return Parent; }
    virtual bool IsBastardChild() { return (Flags >> 7) & 1; }
    virtual bool IsIndependent() { return (Flags >> 6) & 1; }
    virtual bool IsLayerIndependent() { return (Flags >> 5) & 1; }
    virtual bool IsDrawFromViewport() { return FlagsSetTwo & 1; }
    virtual void Draw(CIEngine& engine, CEnginePrimitiveHandle* primitive, long layer, long* count, CComponent* drawer);
};
struct CLeaf : CComponent {
    virtual void Draw(CIEngine&, CEnginePrimitiveHandle*, long layer, long*, CComponent* drawer)
    { ++DrawCalls; LastLayer = layer; LastDrawer = drawer; }
};

void CComponent::Draw(CIEngine& engine, CEnginePrimitiveHandle* primitive, long layer, long* count, CComponent* drawer)
{
    bool independent = IsIndependent();
    bool layerIndependent = IsLayerIndependent();
    long renderLayer;
    if (!independent && !layerIndependent)
        renderLayer = Layer + layer;
    else
        renderLayer = Layer;
    unsigned int i;
    for (i = 0; i < Children.size(); ++i) {
        if (this == Children[i]->GetParent() || Children[i]->IsBastardChild()) {
            if (!Children[i]->IsDrawFromViewport() && !Children[i]->IsDrawFromViewport())
                Children[i]->Draw(engine, primitive, renderLayer, count, this);
        }
    }
    for (i = 0; i < ChildrenToDelete.size(); ++i) {
        if (this == ChildrenToDelete[i]->GetParent() || Children[i]->IsBastardChild()) {
            if (!ChildrenToDelete[i]->IsDrawFromViewport() && !ChildrenToDelete[i]->IsDrawFromViewport())
                ChildrenToDelete[i]->Draw(engine, primitive, renderLayer, count, this);
        }
    }
}

static void Init(CComponent& c) { c.Children._M_start = c.Children._M_finish = 0; c.ChildrenToDelete = c.Children;
    c.Flags = c.FlagsSetTwo = 0; c.Layer = 0; c.Parent = 0; c.DrawCalls = 0; c.LastLayer = -99; c.LastDrawer = 0; }

int main()
{
    CComponent root; CLeaf a, b, c, d;
    Init(root); Init(a); Init(b); Init(c); Init(d);
    root.Layer = -2;
    a.Parent = &root;                       // owned: drawn
    b.Parent = 0; b.Flags = 0x80;           // bastard child: drawn
    c.Parent = &root; c.FlagsSetTwo = 1;    // drawn from viewport: skipped
    d.Parent = &root;                       // pending deletion, still drawn
    CCountedComponentPointer kids[3] = { { &a, 0 }, { &b, 0 }, { &c, 0 } };
    CCountedComponentPointer dying[1] = { { &d, 0 } };
    root.Children._M_start = kids; root.Children._M_finish = kids + 3;
    root.ChildrenToDelete._M_start = dying; root.ChildrenToDelete._M_finish = dying + 1;
    CIEngine engine; long count = 0;
    root.Draw(engine, 0, 10, &count, 0);
    bool ok = a.DrawCalls == 1 && a.LastLayer == 8 && a.LastDrawer == &root
        && b.DrawCalls == 1 && c.DrawCalls == 0 && d.DrawCalls == 1 && d.LastLayer == 8;
    root.Flags = 0x40;                      // independent: own layer only
    root.Draw(engine, 0, 10, &count, 0);
    ok = ok && a.LastLayer == -2 && a.DrawCalls == 2;
    root.Flags = 0x20;                      // layer-independent: own layer only
    root.Draw(engine, 0, 30, &count, 0);
    ok = ok && b.LastLayer == -2 && b.DrawCalls == 3;
    if (!ok) { std::printf("00530260_TEST FAIL\n"); return 1; }
    std::printf("00530260_TEST PASS\n");
    return 0;
}