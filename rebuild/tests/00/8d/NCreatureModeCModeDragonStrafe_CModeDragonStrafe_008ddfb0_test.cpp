// Behaviour test for 008ddfb0: body copied from src/008ddfb0.cpp (CModeDragonStrafe five-argument ctor), callees stubbed.
#include <new>
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CThing;
class CBaseObject;
class CCharStringData { public: const char* m_pData; unsigned long m_length; };
static int g_strDefault = 0, g_strLiteral = 0, g_strDtor = 0, g_strAssign = 0, g_modeBase = 0;
static const char* g_lastLiteral = 0; static long g_lastLength = 0;
class CCharString { public:
    CCharStringData* m_pData;
    CCharString() : m_pData(0) { ++g_strDefault; }
    CCharString(const char* text, long length = -1) { ++g_strLiteral; g_lastLiteral = text; g_lastLength = length; m_pData = new CCharStringData; m_pData->m_pData = text; m_pData->m_length = (unsigned long)strlen(text); }
    ~CCharString() { ++g_strDtor; delete m_pData; m_pData = 0; }
    const CCharString& operator=(const CCharString& other) { ++g_strAssign; delete m_pData; m_pData = 0; if (other.m_pData) { m_pData = new CCharStringData; *m_pData = *other.m_pData; } return *this; }
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};
static unsigned long g_crcSeed = 99; static const void* g_crcData = 0; static unsigned long g_crcLen = 99;
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length) { g_crcSeed = seed; g_crcData = data; g_crcLen = length; return 0x1234 + length; }
class CAnimFilterOfGroup { public:
    virtual void v00() {}
    CCharString   Group;
    unsigned long GroupCRC;
    CAnimFilterOfGroup(const CCharString& group) : Group() { Group = group; GroupCRC = Calc_004014a0(0, group.GetPString(), group.GetLength()); }
};
struct CBaseIntelligentPointer { virtual void v00() {} CBaseObject* PItem; CBaseIntelligentPointer() : PItem(0) {} void SetPItem(void* item) { PItem = (CBaseObject*)item; } };
struct CModeBase {
    virtual void v00() {}
    unsigned char _body[0x18];
    CModeBase(CThing* thing) { ++g_modeBase; memset(_body, 0x55, sizeof(_body)); memcpy(_body, &thing, sizeof(thing)); }
};
class C3DVector { public: float x, y, z; };
template <class T> struct CArrayAllocProxy { T* p; CArrayAllocProxy(T* v) : p(v) { } };
template <class T> class CArray { public:
    T*                  _M_start;
    T*                  _M_finish;
    CArrayAllocProxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;
    float             Tension;
    bool              EndPointsConnected;
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) { }
};
class CCreatureActionBase; struct SCountBlock;
template <class T> class CCountedPointer { public: T* m_pObject; SCountBlock* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { } };
long g_framesPerSecond = 30;
namespace NCreatureMode {
struct __declspec(novtable) CModeDragonFlyingBase : CModeBase {
    CCardinalSpline FlightSpline;
    float           SplinePercentageComplete;
    float           LegAnimAlpha;
    CModeDragonFlyingBase(CThing* thing) : CModeBase(thing) { }
};

struct CModeDragonStrafe : CModeDragonFlyingBase {
    CAnimFilterOfGroup Filter;
    long               Timer, BurninateStartTime, BurninateDuration;
    bool               Burninated;
    float              Speed;
    virtual void v00() {}
    CModeDragonStrafe(CThing* thing, float duration, float speed, float burninateBeforeEnd, float burninateDuration);
};
CModeDragonStrafe::CModeDragonStrafe(CThing* thing, float duration, float speed, float burninateBeforeEnd, float burninateDuration)
    : CModeDragonFlyingBase(thing)
    , Filter("STRAFE")
{
    Speed = speed;
    Timer = (long)(duration * g_framesPerSecond);
    Burninated = false;
    BurninateStartTime = (long)(Timer - burninateBeforeEnd * g_framesPerSecond);
    BurninateDuration = (long)burninateDuration;
}
}
int main()
{
    using namespace NCreatureMode;
    unsigned char raw[sizeof(CModeDragonStrafe)]; memset(raw, 0xAA, sizeof(raw));
    CThing* thing = (CThing*)0x3000;
    CModeDragonStrafe* m = new (raw) CModeDragonStrafe(thing, 2.5f, 7.0f, 1.0f, 45.9f);
    CHECK(sizeof(CModeDragonStrafe) == 0x58);
    CHECK(g_modeBase == 1 && memcmp(m->_body, &thing, 4) == 0);
    CHECK(m->FlightSpline.ControlPoints._M_start == 0 && m->FlightSpline.ControlPoints._M_end_of_storage.p == 0 && m->FlightSpline.Tension == 0.5f);
    CHECK(g_strLiteral == 1 && g_strAssign == 1 && g_strDtor == 1 && strcmp(g_lastLiteral, "STRAFE") == 0 && m->Filter.GroupCRC == 0x1234 + 6);
    CHECK(m->Speed == 7.0f);
    CHECK(m->Timer == 75);                       // 2.5 s * 30 fps
    CHECK(m->Burninated == false);
    CHECK(m->BurninateStartTime == 45);          // 75 - 1.0 * 30
    CHECK(m->BurninateDuration == 45);           // truncated
    if (fails == 0) printf("008ddfb0_TEST PASS\n"); else printf("008ddfb0_TEST FAIL\n");
    return fails;
}