// Behaviour test for 0x007E24D0 CTCFishingRod::~CTCFishingRod (implicit): the def pointer drops one reference, calls the
// pointee's Delete only when the count reaches zero, nulls itself, then ~CTCBase runs.
#include <cstdio>
static int g_deletes = 0;
static int g_baseDtors = 0;
static void* g_seenObject = (void*)1;
class CDefPointeeBase {
public:
    CDefPointeeBase(unsigned long refs) : IVRefCount(refs) {}
    virtual ~CDefPointeeBase() {}
    virtual void Delete() const { ++g_deletes; }
    long Release() const { return --IVRefCount; }
    mutable unsigned long IVRefCount;
};
template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) {}
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
class CFishingRodDef;
class CTCBase {
public:
    CTCBase() : ParentThing(0), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
    virtual ~CTCBase();
    void* ParentThing;
    bool  ToBeRemoved;
    bool  IsInGlobalUpdateTCs;
};
class CTCFishingRod : public CTCBase {
public:
    CDefPointer<const CFishingRodDef> PDef;
};
CTCBase::~CTCBase()
{
    ++g_baseDtors;
    g_seenObject = static_cast<CTCFishingRod*>(this)->PDef.Object;   // the member is already released (and nulled)
}
int main()
{
    CDefPointeeBase shared(2), last(1);
    CTCFishingRod* a = new CTCFishingRod; a->PDef.Object = &shared;
    delete a;
    if (shared.IVRefCount != 1 || g_deletes != 0 || g_baseDtors != 1 || g_seenObject != 0) { std::printf("007e24d0_TEST FAIL\n"); return 1; }
    CTCFishingRod* b = new CTCFishingRod; b->PDef.Object = &last;
    delete b;
    if (last.IVRefCount != 0 || g_deletes != 1 || g_baseDtors != 2) { std::printf("007e24d0_TEST FAIL\n"); return 1; }
    CTCFishingRod* c = new CTCFishingRod;     // null def: nothing released
    delete c;
    if (g_deletes != 1 || g_baseDtors != 3) { std::printf("007e24d0_TEST FAIL\n"); return 1; }
    std::printf("007e24d0_TEST PASS\n");
    return 0;
}