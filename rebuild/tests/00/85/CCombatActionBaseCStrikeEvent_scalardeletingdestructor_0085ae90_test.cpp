// Behaviour test for 0x0085AE90 CCombatActionBase::CStrikeEvent::`scalar deleting destructor' (chunk 402): deleting a
// strike event frees the struck-thing list's buffer, destroys the weapon pointer and the helper-point name (in reverse
// member order) and frees the event; a destructor call without delete leaves the storage alone.
#include <cstdio>
#include <cstdlib>
#include <new>

static int gEvents[8];
static int gEventCount = 0;
static void Log(int what) { if (gEventCount < 8) gEvents[gEventCount++] = what; }
enum { LOG_FREE_LIST = 1, LOG_POINTER = 2, LOG_NAME = 3 };

class CCharString {
public:
    const char* Text;
    CCharString() : Text("") { }
    ~CCharString() { Log(LOG_NAME); }
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    virtual ~CBaseIntelligentPointer() { Log(LOG_POINTER); }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CThing;
class CThingGameObject;
namespace stl402 {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        if (_M_start) {
            free(_M_start);
            Log(LOG_FREE_LIST);
        }
    }
};
}
class CCombatActionBase {
public:
    class CStrikeEvent {
    public:
        bool                                  Legacy;
        bool                                  PrimaryLegacy;
        bool                                  Weapon;
        CCharString                           HelperPointName;
        long                                  StrikeEventIndex;
        long                                  StartFrame;
        long                                  EndFrame;
        CIntelligentPointer<const CThing>     PWeapon;
        stl402::vector<CThingGameObject*>     StruckThingList;
    };
};

static void DeleteStrikeEvent(CCombatActionBase::CStrikeEvent* strikeEvent)
{
    delete strikeEvent;
}

int main()
{
    int failures = 0;
    CCombatActionBase::CStrikeEvent* e = new CCombatActionBase::CStrikeEvent;
    e->StruckThingList._M_start = (CThingGameObject**)malloc(4 * sizeof(CThingGameObject*));
    e->StruckThingList._M_finish = e->StruckThingList._M_start;
    e->StruckThingList._M_end_of_storage = e->StruckThingList._M_start + 4;
    DeleteStrikeEvent(e);
    if (gEventCount != 3 || gEvents[0] != LOG_FREE_LIST || gEvents[1] != LOG_POINTER || gEvents[2] != LOG_NAME) {
        printf("  wrong destruction sequence (%d events)\n", gEventCount); ++failures;
    }
    gEventCount = 0;
    {
        CCombatActionBase::CStrikeEvent local;              // empty list: nothing to free
    }
    if (gEventCount != 2 || gEvents[0] != LOG_POINTER || gEvents[1] != LOG_NAME) {
        printf("  wrong sequence for an empty list\n"); ++failures;
    }
    if (failures == 0) {
        printf("0085ae90_TEST PASS\n");
        return 0;
    }
    printf("0085ae90_TEST FAIL\n");
    return 1;
}