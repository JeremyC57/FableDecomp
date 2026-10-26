// Behaviour test for 0x006A7590 CThingFilter_ObjectHasProperty::operator(): true when the object's properties share a bit with the filter's.
#include <cstdio>
static unsigned long g_properties = 0;
enum EObjectProperty { };
class CThingObject {
public:
    virtual void v00() { }
    virtual void v01() { }
    virtual void v02() { }
    virtual void v03() { }
    virtual void v04() { }
    virtual void v05() { }
    virtual void v06() { }
    virtual void v07() { }
    virtual void v08() { }
    virtual void v09() { }
    virtual void v10() { }
    virtual void v11() { }
    virtual void v12() { }
    virtual void v13() { }
    virtual void v14() { }
    virtual void v15() { }
    virtual void v16() { }
    virtual void v17() { }
    virtual void v18() { }
    virtual void v19() { }
    virtual void v20() { }
    virtual void v21() { }
    virtual void v22() { }
    virtual void v23() { }
    virtual void v24() { }
    virtual void v25() { }
    virtual void v26() { }
    virtual void v27() { }
    virtual void v28() { }
    virtual void v29() { }
    virtual void v30() { }
    virtual void v31() { }
    virtual void v32() { }
    virtual void v33() { }
    virtual void v34() { }
    virtual void v35() { }
    virtual void v36() { }
    virtual void v37() { }
    virtual void v38() { }
    virtual void v39() { }
    virtual void v40() { }
    virtual void v41() { }
    virtual void v42() { }
    virtual void v43() { }
    virtual void v44() { }
    virtual void v45() { }
    virtual void v46() { }
    virtual void v47() { }
    virtual unsigned long GetProperties() const { return g_properties; }
};
class CThingFilter_ObjectHasProperty {
public:
    EObjectProperty Property;
    bool operator()(const CThingObject& object) const;
};
bool CThingFilter_ObjectHasProperty::operator()(const CThingObject& object) const
{
    unsigned long property = Property;
    return (property & object.GetProperties()) != 0;
}
int main()
{
    CThingObject object;
    CThingFilter_ObjectHasProperty filter;
    filter.Property = (EObjectProperty)0x0F;
    g_properties = 0x02;   bool r1 = filter(object);    // overlap
    g_properties = 0x10;   bool r2 = filter(object);    // no overlap
    g_properties = 0x00;   bool r3 = filter(object);    // none
    filter.Property = (EObjectProperty)0;
    g_properties = 0xFFFF; bool r4 = filter(object);    // empty filter
    filter.Property = (EObjectProperty)0x80000000;
    g_properties = 0x80000001; bool r5 = filter(object); // high bit
    if (r1 && !r2 && !r3 && !r4 && r5) { std::printf("006a7590_TEST PASS\n"); return 0; }
    std::printf("006a7590_TEST FAIL %d %d %d %d %d\n", r1, r2, r3, r4, r5);
    return 1;
}
