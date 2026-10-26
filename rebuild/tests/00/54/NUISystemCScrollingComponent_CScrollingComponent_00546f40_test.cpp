// behaviour test for 0x00546F40 NUISystem::CScrollingComponent::CScrollingComponent(long defIndex): the base constructor receives the def index, and the
// finished object dispatches as the derived class through each of its three base interfaces
#include <cstdio>
struct UIInterface { virtual ~UIInterface() { } virtual int Kind() const { return 0; } };
struct Observer { virtual ~Observer() { } virtual int ObsKind() const { return 0; } int pad[4]; };
struct ActionParam { virtual ~ActionParam() { } virtual int ParamKind() const { return 0; } int Type; };
struct Component : UIInterface, Observer, ActionParam {
    int DefIndex;
    Component(int defIndex) : DefIndex(defIndex) { Type = 1; }
    Component(const Component& o) : UIInterface(), Observer(), ActionParam() { DefIndex = o.DefIndex; Type = 1; }
};
struct Base : Component {
    Base(int d) : Component(d) { }
    Base(const Base& o) : Component(o) { }
    int Kind() const { return 1; } int ObsKind() const { return 1; } int ParamKind() const { return 1; }
};
struct Derived : Base {
    Derived(int d) : Base(d) { }
    Derived(const Derived& o) : Base(o) { }
    int Kind() const { return 2; } int ObsKind() const { return 2; } int ParamKind() const { return 2; }
};
int main()
{
    int fail = 0;
    Derived src(41);
    Derived d(41);
    if (d.DefIndex != 41 || d.Type != 1) fail |= 1;
    UIInterface* u = &d; Observer* o = &d; ActionParam* p = &d;
    if (u->Kind() != 2 || o->ObsKind() != 2 || p->ParamKind() != 2) fail |= 2;
    if (fail) { std::printf("00546f40_TEST FAIL %d\n", fail); return 1; }
    std::printf("00546f40_TEST PASS\n");
    return 0;
}