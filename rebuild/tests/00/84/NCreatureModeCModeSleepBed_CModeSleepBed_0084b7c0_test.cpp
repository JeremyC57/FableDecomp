// Behaviour test for 0084b7c0 (NCreatureMode::CModeSleepBed::CModeSleepBed): the base takes the thing, the filter takes the group (third argument), the name copies the second argument, the remove-action pointer starts null.
#include <cstdio>

struct CThing { long m_id; };
struct SCountBlock;
struct CCreatureActionBase;

struct CCharString {
    const char* m_pData;
    CCharString(const char* text) : m_pData(text) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { }
};

struct CAnimFilterOfGroup {
    CCharString   Group;
    unsigned long GroupCRC;
    CAnimFilterOfGroup(const CCharString& group) : Group(group), GroupCRC(0) { }
};

template <class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

struct CModeBase {
    CThing* ParentThing;
    CModeBase(CThing* thing) : ParentThing(thing) { }
    virtual ~CModeBase() { }
};

struct CModeSleepBed : CModeBase {
    CAnimFilterOfGroup                    Filter;
    CCharString                           SleepAnimName;
    CCountedPointer<CCreatureActionBase>  PRemoveAction;
    CModeSleepBed(CThing* thing, const CCharString& sleepAnimName, const CCharString& group)
        : CModeBase(thing)
        , Filter(group)
        , SleepAnimName(sleepAnimName)
    {
    }
};

int main()
{
    CThing thing = { 1 };
    CCharString anim("SLEEP_ANIM"), group("BED_GROUP");
    CModeSleepBed mode(&thing, anim, group);
    if (mode.ParentThing != &thing || mode.Filter.Group.m_pData != group.m_pData || mode.SleepAnimName.m_pData != anim.m_pData
        || mode.PRemoveAction.m_pObject != 0 || mode.PRemoveAction.m_pCount != 0) { std::printf("0084b7c0_TEST FAIL\n"); return 1; }
    std::printf("0084b7c0_TEST PASS\n");
    return 0;
}