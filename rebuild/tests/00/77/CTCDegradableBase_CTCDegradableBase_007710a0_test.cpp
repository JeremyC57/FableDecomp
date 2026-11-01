// Behaviour test for 007710a0 CTCDegradableBase::~CTCDegradableBase(): the def pointer member is released (count decremented, Delete at zero, nothing on null),
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
class CTCBase {
public:
    virtual ~CTCBase() { ++g_baseDtors; }
};
class CTCDegradableBase : public CTCBase {
public:
    CDefPointer<const CDef> PDefinition;
};
int main()
{
    CDefPointeeBase def;
    def.IVRefCount = 2;
    { CTCDegradableBase thing; thing.PDefinition.Object = &def; }
    if (def.IVRefCount != 1 || g_deletes != 0 || g_baseDtors != 1) { std::printf("007710a0_TEST FAIL\n"); return 1; }
    { CTCDegradableBase thing; thing.PDefinition.Object = &def; }
    if (def.IVRefCount != 0 || g_deletes != 1 || g_baseDtors != 2) { std::printf("007710a0_TEST FAIL\n"); return 1; }
    { CTCDegradableBase thing; }                                   // null def: no release
    if (g_deletes != 1 || g_baseDtors != 3) { std::printf("007710a0_TEST FAIL\n"); return 1; }
    CTCBase* base = new CTCDegradableBase;
    delete base;                                    // through the virtual destructor
    if (g_baseDtors != 4) { std::printf("007710a0_TEST FAIL\n"); return 1; }
    std::printf("007710a0_TEST PASS\n");
    return 0;
}