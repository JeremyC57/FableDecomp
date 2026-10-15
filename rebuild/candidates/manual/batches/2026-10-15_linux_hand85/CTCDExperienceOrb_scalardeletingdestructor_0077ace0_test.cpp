// Test for 0077ace0 CTCDExperienceOrb scalar deleting destructor: def released once (body PDef = 0, then the member's
// own release finds null), CTCBase destructor last, storage freed through delete.
#include <cstdio>
struct CThing { int id; };
static int g_baseDtor = 0, g_destroyed = 0;
struct CDefPointeeBase {
    long RefCount;
    virtual ~CDefPointeeBase() {}
    virtual void Destroy() { ++g_destroyed; }
};
struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    void Release()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    CDefPointer& operator=(int) { Release(); Object = 0; return *this; }
    ~CDefPointer() { Release(); }
};
struct CTCBase {
    CThing* ParentThing;
    CTCBase(CThing* parent) : ParentThing(parent) {}
    virtual ~CTCBase() { ++g_baseDtor; }
};
struct CTCDExperienceOrb : CTCBase {
    CDefPointer PDef;
    long ExperiencePoints;
    CTCDExperienceOrb(CThing* parent) : CTCBase(parent), ExperiencePoints(0) {}
    virtual ~CTCDExperienceOrb() { PDef = 0; }
};
int main()
{
    CThing t = { 1 };
    CDefPointeeBase def; def.RefCount = 2;
    CTCBase* a = new CTCDExperienceOrb(&t);
    ((CTCDExperienceOrb*)a)->PDef.Object = &def;
    delete a;
    if (def.RefCount != 1 || g_destroyed != 0 || g_baseDtor != 1) { std::printf("0077ace0_TEST FAIL\n"); return 1; }
    CTCBase* b = new CTCDExperienceOrb(&t);
    ((CTCDExperienceOrb*)b)->PDef.Object = &def;
    delete b;
    if (def.RefCount != 0 || g_destroyed != 1 || g_baseDtor != 2) { std::printf("0077ace0_TEST FAIL\n"); return 1; }
    std::printf("0077ace0_TEST PASS\n");
    return 0;
}
