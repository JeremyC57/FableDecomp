// Behaviour test for 0x00718D50 CCreatureAction_PerformExpression::~CCreatureAction_PerformExpression(): the implicit destructor destroys the intelligent pointer, then
// releases the counted def handle (Delete when the count reaches zero, Object cleared), then runs the action base dtor.
#include <cstdio>

static int g_order[8];
static int g_n = 0;
static int g_deleted = 0;

class CDefPointeeBase {
public:
    CDefPointeeBase() : RefCount(1) { }
    virtual ~CDefPointeeBase() { }
    virtual void Delete() const { ++g_deleted; }
    long Release() const { return --RefCount; }
    void AddRef() const { ++RefCount; }
    mutable long RefCount;
};
template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) { }
    ~CDefPointer() { g_order[g_n++] = 2; Release(); }
    void Release()
    {
        if (Object) {
            const CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    const CDefPointeeBase* Object;
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { g_order[g_n++] = 1; }
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase() { g_order[g_n++] = 3; }
};
class CThing;
class CThingCreatureBase;
class CDefType;
class C3DVector { public: float X, Y, Z; };

class CCreatureAction_PerformExpression : public CCreatureActionBase {
public:
    CDefPointer<const CDefType> PDef;

    CIntelligentPointer<CThing> PPointer;
};

int main()
{
    CDefPointeeBase shared;
    shared.RefCount = 2;
    {
        CCreatureAction_PerformExpression action;
        action.PDef.Object = &shared;
    }
    if (g_n != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || shared.RefCount != 1 || g_deleted != 0) {
        std::printf("00718d50_TEST FAIL\n");
        return 1;
    }
    g_n = 0;
    CCreatureActionBase* p = new CCreatureAction_PerformExpression;
    ((CCreatureAction_PerformExpression*)p)->PDef.Object = &shared;
    delete p;
    if (g_n != 3 || shared.RefCount != 0 || g_deleted != 1) {
        std::printf("00718d50_TEST FAIL\n");
        return 1;
    }
    std::printf("00718d50_TEST PASS\n");
    return 0;
}