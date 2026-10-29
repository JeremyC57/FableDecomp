// Behaviour test for CThingDeadCreature::~CThingDeadCreature (0x00835B10): the member dtors run in reverse order
// (body reorienter deleted, killer pointer released, anim-filter counted pointer released with its delete function on the
// last reference), then the CThingGameObject base dtor.
#include <cstdio>
static int g_order[8];
static int g_n = 0;
static void Log(int what) { if (g_n < 8) g_order[g_n++] = what; }

namespace boost {
template <class T> inline void checked_delete(T* x) { delete x; }
template <class T> class scoped_ptr { public:
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { boost::checked_delete(ptr); }
private:
    T* ptr;
};
}
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer() { Log(2); }
    void* PData;
    CBaseIntelligentPointer() : PData(0) { }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};
class CThingBodyReorienter { public:
    ~CThingBodyReorienter() { Log(1); }
};
class CAnimFilterBase { public: int Value; };
static void DeleteFilter(void* data) { Log(3); delete (CAnimFilterBase*)data; }
class CThingCreatureBase;
class CThingGameObject { public:
    virtual ~CThingGameObject() { Log(4); }
};
class CThingDeadCreature : public CThingGameObject { public:
    CCountedPointer<CAnimFilterBase>              PAnimFilter;
    CIntelligentPointer<const CThingCreatureBase> PKiller;
    long                                          BodyDieTimer;
    boost::scoped_ptr<CThingBodyReorienter>       BodyReorienter;
    bool                                          StartedDecay;
    CThingDeadCreature(CThingBodyReorienter* r) : BodyDieTimer(0), BodyReorienter(r), StartedDecay(false) { }
    virtual ~CThingDeadCreature() { }
};

int main()
{
    CThingDeadCreature* dead = new CThingDeadCreature(new CThingBodyReorienter);
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 1; info->DeleteFunc = DeleteFilter; info->Data = new CAnimFilterBase;
    dead->PAnimFilter.Data = (CAnimFilterBase*)info->Data; dead->PAnimFilter.Info = info;
    delete dead;
    const int want[] = { 1, 2, 3, 4 };
    if (g_n != 4) { std::printf("00835b10_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (g_order[i] != want[i]) { std::printf("00835b10_TEST FAIL\n"); return 1; }
    // a shared filter is only released, not deleted
    g_n = 0;
    CThingDeadCreature* other = new CThingDeadCreature(0);
    CCPPointerInfo shared; shared.RefCount = 2; shared.DeleteFunc = DeleteFilter; shared.Data = 0;
    other->PAnimFilter.Info = &shared;
    delete other;
    if (shared.RefCount != 1 || g_n != 2 || g_order[0] != 2 || g_order[1] != 4) { std::printf("00835b10_TEST FAIL\n"); return 1; }
    std::printf("00835b10_TEST PASS\n");
    return 0;
}