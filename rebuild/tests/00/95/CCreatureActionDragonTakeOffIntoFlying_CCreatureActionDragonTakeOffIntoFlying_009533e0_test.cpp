// Behaviour test for 0x009533E0 CCreatureAction_DragonTakeOffIntoFlying constructor: base gets owner, code 250 and the
// SWORD_SWING sound pair; the movement points are copied, UsePreviousPoint is set and the end-action pointer is empty.
#include <cstdio>
#include <cstring>

class CThingCreatureBase { public: int id; };
class CCreatureActionBase;
class CCPPointerInfo;

static int g_liveStrings = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
static CSoundPair GetSoundPair(CThingCreatureBase* owner, const CCharString& name)
{
    CSoundPair p;
    p.FirstSound = owner->id;
    p.LastSound = std::strcmp(name.m_pData, "SWORD_SWING") == 0 ? 77 : -1;
    return p;
}
class C3DVector { public: float X, Y, Z; };
struct CPointVector {                       // stand-in for the 12-byte vector: a fixed copy of up to 4 points
    C3DVector pts[4]; int count;
    CPointVector() : count(0) { }
    CPointVector(const CPointVector& o) : count(o.count) { for (int i = 0; i < count; ++i) pts[i] = o.pts[i]; }
};
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CCombatActionBase { public:
    CThingCreatureBase* m_owner; long m_code; CSoundPair m_sound; int m_stringsAlive;
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
        : m_owner(owner), m_code(actionCode), m_sound(sound), m_stringsAlive(g_liveStrings) { }
};
class CCreatureAction_DragonTakeOffIntoFlying : public CCombatActionBase { public:
    CPointVector MovementPoints;
    unsigned char UsePreviousPoint;
    CCountedPointer<CCreatureActionBase> PEndAction;
    CCreatureAction_DragonTakeOffIntoFlying(CThingCreatureBase* owner, const CPointVector& points)
        : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING")), MovementPoints(points),
          UsePreviousPoint(1), PEndAction() { }
};

int main()
{
    CThingCreatureBase owner = { 4 };
    CPointVector pts;
    pts.count = 2; pts.pts[0].X = 1.0f; pts.pts[1].Z = 3.5f;
    CCreatureAction_DragonTakeOffIntoFlying a(&owner, pts);
    if (a.m_owner != &owner || a.m_code != 250 || a.m_sound.LastSound != 77 || a.m_stringsAlive != 1 ||
        g_liveStrings != 0 || a.MovementPoints.count != 2 || a.MovementPoints.pts[0].X != 1.0f ||
        a.MovementPoints.pts[1].Z != 3.5f || a.UsePreviousPoint != 1 || a.PEndAction.Data != 0 || a.PEndAction.Info != 0) {
        std::printf("009533e0_TEST FAIL\n"); return 1;
    }
    std::printf("009533e0_TEST PASS\n");
    return 0;
}