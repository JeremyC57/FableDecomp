// Behaviour test for 0x00702FF0 CThingObject::GetProperties(): returns the def's Property word through PDef.
#include <cstdio>
class CThingObjectDef {
public:
    unsigned char _pad_0x00[0x90];
    unsigned long Property;
};
template <class T> class CDefPointer {
public:
    T* PDef;
    T* operator->() const { return PDef; }
};
class CThingObject {
public:
    virtual unsigned long GetProperties() const;
    unsigned char _pad_0x04[0xd0 - 0x04];
    CDefPointer<const CThingObjectDef> PDef;
};
unsigned long CThingObject::GetProperties() const
{
    return PDef->Property;
}
int main()
{
    CThingObjectDef def; def.Property = 0x1234;
    CThingObject object; object.PDef.PDef = &def;
    const CThingObject& ref = object;
    bool ok = ref.GetProperties() == 0x1234 && (char*)&object.PDef - (char*)&object == 0xd0 && (char*)&def.Property - (char*)&def == 0x90;
    def.Property = 0; ok = ok && object.GetProperties() == 0;
    if (!ok) { std::printf("00702ff0_TEST FAIL\n"); return 1; }
    std::printf("00702ff0_TEST PASS\n");
    return 0;
}
