// behaviour test for 0x007E3020 CTCCameraCollision::CTCCameraCollision(CThing&) (model copy of the source; CTCBase's ctor is a stub)
#include <cstdio>
#include <cstring>
#include <new>
class CThing { public: int id; };
static CThing* g_baseThing = 0;
class CTCBase {
public:
    CTCBase(CThing& thing) : ParentThing(&thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { g_baseThing = &thing; }
    virtual ~CTCBase() { }
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
class CCameraCollisionDef;
class CDefPointeeBase;
template <class T> class CDefPointer {                    // Ego_r.pdb: CDefPointeeBase* Object
public:
    CDefPointeeBase* Object;                             // +0x00
    CDefPointer() : Object(0) { }
};
class CTCCameraCollision : public CTCBase {
public:
    CTCCameraCollision(CThing& thing);
    CDefPointer<const CCameraCollisionDef> CameraCollisionDef;   // +0x0C
};
CTCCameraCollision::CTCCameraCollision(CThing& thing)
    : CTCBase(thing)
{
}
int main()
{
    CThing t; t.id = 1;
    static double raw[(sizeof(CTCCameraCollision) + 7) / 8];
    std::memset(raw, 0xCD, sizeof(raw));
    CTCCameraCollision* p = new (raw) CTCCameraCollision(t);
    int ok = g_baseThing == &t && p->ParentThing == &t && p->CameraCollisionDef.Object == 0;
    if (!ok) { std::printf("007e3020_TEST FAIL\n"); return 1; }
    std::printf("007e3020_TEST PASS\n");
    return 0;
}