// Behaviour test for 0x007e0b20 CTCHeroSpecialMovement::~CTCHeroSpecialMovement() (implicit destructor): the PHeroSpecialMovementDef def pointer is released -- the pointee's
// count goes down, the pointee is deleted when it reaches zero, the pointer is cleared -- and then CTCBase's destructor runs.
#include <cstdio>

static int g_deleted = 0;
static int g_baseDtors = 0;
static void* g_lastObject = (void*)1;

class CDefPointeeBase {
public:
    CDefPointeeBase() : IVRefCount(1) { }
    virtual ~CDefPointeeBase() { }
    virtual void Delete() const { ++g_deleted; delete this; }
    mutable unsigned int IVRefCount;
};
class CDef : public CDefPointeeBase { };

template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) { }
    explicit CDefPointer(const T* object) : Object((CDefPointeeBase*)object) { if (Object) ++Object->IVRefCount; }
    ~CDefPointer() { Release(); g_lastObject = Object; }
    void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;
};

class CTCBase { public: virtual ~CTCBase() { ++g_baseDtors; } };
class CTCHeroSpecialMovement : public CTCBase {
public:
    explicit CTCHeroSpecialMovement(const CDef* def) : PHeroSpecialMovementDef(def) { }
    CDefPointer<const CDef> PHeroSpecialMovementDef;
};

int main()
{
    CDef* shared = new CDef;                   // count 1 (the owner)
    CTCBase* a = new CTCHeroSpecialMovement(shared);             // count 2
    delete a;                                  // count 1: not deleted
    if (g_deleted != 0 || shared->IVRefCount != 1 || g_lastObject != 0 || g_baseDtors != 1) {
        std::printf("007e0b20_TEST FAIL (shared)\n");
        return 1;
    }
    shared->IVRefCount = 0;                    // hand the only reference to the component
    CTCBase* b = new CTCHeroSpecialMovement(shared);             // count 1
    delete b;                                  // count 0: Delete()
    if (g_deleted != 1 || g_baseDtors != 2) {
        std::printf("007e0b20_TEST FAIL (last)\n");
        return 1;
    }
    CTCBase* c = new CTCHeroSpecialMovement(0);                  // null def: nothing to release
    delete c;
    if (g_deleted != 1 || g_baseDtors != 3) {
        std::printf("007e0b20_TEST FAIL (null)\n");
        return 1;
    }
    std::printf("007e0b20_TEST PASS\n");
    return 0;
}