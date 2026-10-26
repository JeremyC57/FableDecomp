// behaviour test for 0x005DA890 CTCWeapon::GetTargetingRange() const: the def's field comes back, the def's reference count is restored, and
// the def is deleted only when the copy held its last reference
#include <cstdio>
static int gDeleted = 0;
struct Def { int RefCount; int Field; void Delete() const { ++gDeleted; } };
struct DefPtr {
    Def* Object;
    DefPtr(const DefPtr& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    explicit DefPtr(Def* d) : Object(d) { }
    ~DefPtr() { if (Object) { if (--Object->RefCount == 0) Object->Delete(); Object = 0; } }
};
struct Component {
    DefPtr PDef;
    explicit Component(Def* d) : PDef(d) { }
    int Get() const { DefPtr def(PDef); return def.Object->Field; }   // the function under test
};
int main()
{
    int fail = 0;
    Def d; d.RefCount = 1; d.Field = 0x1234;
    Component c(&d);
    if (c.Get() != 0x1234 || d.RefCount != 1 || gDeleted != 0) fail |= 1;
    d.Field = -7;
    if (c.Get() != -7 || d.RefCount != 1) fail |= 2;
    Def e; e.RefCount = 0; e.Field = 5;                       // a def nobody else holds: the copy's release deletes it
    Component c2(&e);
    if (c2.Get() != 5 || gDeleted != 1 || e.RefCount != 0) fail |= 4;
    c2.PDef.Object = 0;
    c.PDef.Object = 0;
    if (fail) { std::printf("005da890_TEST FAIL %d\n", fail); return 1; }
    std::printf("005da890_TEST PASS\n");
    return 0;
}