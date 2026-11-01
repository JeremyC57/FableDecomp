// Behaviour test for 00529ad0 CThingHolySite::~CThingHolySite(): the def pointer member is released (count decremented, Delete at zero, nothing on null),
// then the base destructor runs.
#include <cstdio>
static int g_baseDtors = 0;
static int g_deletes = 0;
class CDefPointeeBase {
public:
    virtual ~CDefPointeeBase() { }
    virtual void Delete() const { ++g_deletes; }
    long Release() const { return --IVRefCount; }
    mutable unsigned int IVRefCount;
};
template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) { }
    ~CDefPointer() { Release(); }
    void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;
};
class CDef;
class CThingGameObject {
public:
    virtual ~CThingGameObject() { ++g_baseDtors; }
};
class CThingHolySite : public CThingGameObject {
public:
    CDefPointer<const CDef> Def;
};
int main()
{
    CDefPointeeBase def;
    def.IVRefCount = 2;
    { CThingHolySite thing; thing.Def.Object = &def; }
    if (def.IVRefCount != 1 || g_deletes != 0 || g_baseDtors != 1) { std::printf("00529ad0_TEST FAIL\n"); return 1; }
    { CThingHolySite thing; thing.Def.Object = &def; }
    if (def.IVRefCount != 0 || g_deletes != 1 || g_baseDtors != 2) { std::printf("00529ad0_TEST FAIL\n"); return 1; }
    { CThingHolySite thing; }                                   // null def: no release
    if (g_deletes != 1 || g_baseDtors != 3) { std::printf("00529ad0_TEST FAIL\n"); return 1; }
    CThingGameObject* base = new CThingHolySite;
    delete base;                                    // through the virtual destructor
    if (g_baseDtors != 4) { std::printf("00529ad0_TEST FAIL\n"); return 1; }
    std::printf("00529ad0_TEST PASS\n");
    return 0;
}