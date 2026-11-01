// Behaviour test for 0x007E3370 CTCCameraCollision::~CTCCameraCollision: the implicit destructor releases the def pointer
// (count decrement, Delete at zero, member nulled) and then runs the CTCBase destructor.
#include <cstdio>
static int g_deleted = 0, g_baseDtors = 0;
class CDefPointee { public:
    unsigned long RefCount;
    CDefPointee() : RefCount(1) { }
    virtual ~CDefPointee() { }
    virtual void Delete() { ++g_deleted; }
    unsigned long Release() { return --RefCount; }
};
template <class T> class CDefPointer { public:
    CDefPointee* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            CDefPointee* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
};
class CTCBase { public:
    virtual ~CTCBase() { ++g_baseDtors; }
};
class CCameraCollisionDef;
class CTCCameraCollision : public CTCBase { public:
    CDefPointer<const CCameraCollisionDef> CameraCollisionDef;
};
int main()
{
    CDefPointee shared;
    shared.RefCount = 2;
    {
        CTCCameraCollision tc;
        tc.CameraCollisionDef.Object = &shared;
    }
    if (shared.RefCount != 1 || g_deleted != 0 || g_baseDtors != 1) { std::printf("007e3370_TEST FAIL\n"); return 1; }
    {
        CTCCameraCollision tc;
        tc.CameraCollisionDef.Object = &shared;
    }
    if (shared.RefCount != 0 || g_deleted != 1 || g_baseDtors != 2) { std::printf("007e3370_TEST FAIL\n"); return 1; }
    {
        CTCCameraCollision tc;                             // null def: nothing released
    }
    if (g_deleted != 1 || g_baseDtors != 3) { std::printf("007e3370_TEST FAIL\n"); return 1; }
    std::printf("007e3370_TEST PASS\n");
    return 0;
}