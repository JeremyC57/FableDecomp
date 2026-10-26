// Behaviour test for CThingAICreature::GetSleepTimes (0x00833390): with a village the age picks the village def's
// child/adult/elderly pair (unknown ages fall back to adult) and the def reference is balanced; without the
// village-member interface or without a village the definition manager's default def supplies the pair.
#include <cstdio>

static int g_destroyed = 0;

class CDefPointeeBase { public:
    long m_refCount;
    virtual void Destroy() { ++g_destroyed; }
    virtual ~CDefPointeeBase() {}
};
class CVillageDef : public CDefPointeeBase { public:
    long SleepAdultStartTime, SleepAdultEndTime, SleepChildStartTime, SleepChildEndTime,
         SleepElderlyStartTime, SleepElderlyEndTime;
};
struct CDefaultVillageTimes {
    long SleepAdultStartTime, SleepAdultEndTime, SleepChildStartTime, SleepChildEndTime,
         SleepElderlyStartTime, SleepElderlyEndTime;
};
class CDefinitionManager { public: CDefaultVillageTimes* DefaultVillageDef; };

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (--Object->m_refCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CTCVillage { public: CDefPointer<CVillageDef> PDef; };
class CTCVillageMember { public:
    CTCVillage* village;
    CTCVillage* GetPVillage() const { return village; }
};
class CThingCreatureDef { public: long Age; };

static CDefinitionManager g_manager;

class CThing { public:
    unsigned long m_presentBits[9];
    CTCVillageMember* m_member;
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> T* GetTC(long) const { return (T*)m_member; }
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
};

class CThingAICreature : public CThing { public:
    CThingCreatureDef* PDef;
    void GetSleepTimes(long& start, long& end);
};

void CThingAICreature::GetSleepTimes(long& start, long& end)
{
    if (HasTC(0x23)) {
        CTCVillage* village = GetTC<CTCVillageMember>(0x23)->GetPVillage();
        if (village) {
            CDefPointer<CVillageDef> def = village->PDef;
            switch (PDef->Age) {
            case 1: start = def->SleepChildStartTime; end = def->SleepChildEndTime; return;
            case 2: start = def->SleepAdultStartTime; end = def->SleepAdultEndTime; return;
            case 3: start = def->SleepElderlyStartTime; end = def->SleepElderlyEndTime; return;
            default: start = def->SleepAdultStartTime; end = def->SleepAdultEndTime; return;
            }
        }
    }
    CDefinitionManager* manager = GetDefinitionManager();
    CDefaultVillageTimes* def = manager->DefaultVillageDef;
    switch (PDef->Age) {
    case 1: start = def->SleepChildStartTime; end = def->SleepChildEndTime; return;
    case 2: start = def->SleepAdultStartTime; end = def->SleepAdultEndTime; return;
    case 3: start = def->SleepElderlyStartTime; end = def->SleepElderlyEndTime; return;
    default: start = def->SleepAdultStartTime; end = def->SleepAdultEndTime; return;
    }
}

int main()
{
    CVillageDef vdef;
    vdef.m_refCount = 1;
    vdef.SleepAdultStartTime = 22; vdef.SleepAdultEndTime = 6; vdef.SleepChildStartTime = 19;
    vdef.SleepChildEndTime = 7; vdef.SleepElderlyStartTime = 21; vdef.SleepElderlyEndTime = 5;
    CDefaultVillageTimes dflt = { 23, 8, 20, 9, 18, 4 };
    g_manager.DefaultVillageDef = &dflt;
    CTCVillage village; village.PDef.Object = &vdef; ++vdef.m_refCount;
    CTCVillageMember member; member.village = &village;
    CThingCreatureDef cdef;
    CThingAICreature c;
    for (int i = 0; i < 9; ++i) c.m_presentBits[i] = 0;
    c.m_presentBits[1] = 1UL << 3;                       // interface 0x23
    c.m_member = &member;
    c.PDef = &cdef;
    long s = 0, e = 0;
    int ok = 1;
    cdef.Age = 1; c.GetSleepTimes(s, e); ok = ok && s == 19 && e == 7;
    cdef.Age = 2; c.GetSleepTimes(s, e); ok = ok && s == 22 && e == 6;
    cdef.Age = 3; c.GetSleepTimes(s, e); ok = ok && s == 21 && e == 5;
    cdef.Age = 7; c.GetSleepTimes(s, e); ok = ok && s == 22 && e == 6;
    ok = ok && vdef.m_refCount == 2 && g_destroyed == 0;
    member.village = 0;                                  // no village: default def
    cdef.Age = 1; c.GetSleepTimes(s, e); ok = ok && s == 20 && e == 9;
    cdef.Age = 3; c.GetSleepTimes(s, e); ok = ok && s == 18 && e == 4;
    member.village = &village;
    c.m_presentBits[1] = 0;                              // no village-member interface: default def
    cdef.Age = 2; c.GetSleepTimes(s, e); ok = ok && s == 23 && e == 8;
    cdef.Age = 0; c.GetSleepTimes(s, e); ok = ok && s == 23 && e == 8;
    if (!ok) { std::printf("00833390_TEST FAIL\n"); return 1; }
    std::printf("00833390_TEST PASS\n");
    return 0;
}