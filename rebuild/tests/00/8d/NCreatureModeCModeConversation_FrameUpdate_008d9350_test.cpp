#include <cstdio>
#include <cstring>
// Behaviour test for NCreatureMode::CModeConversation::FrameUpdate (retail 0x008D9350): when the talk component's
// (interface 15) attitude changes, the mode records it, switches its filter group to the attitude's animation group and
// asks for the idle animation to be reset; then the base update.
class CTCBase { public: int tag; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThingLookup {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry m_store[9];
    void ResetInterfaces()
    {
        unsigned int w = 0;
        while (w < 9) { m_presentBits[w] = 0; m_store[w].type = 0x7fff; m_store[w].component = 0; ++w; }
        m_interfaces.first = m_store;
        m_interfaces.last = m_store;
    }
    void AddInterface(long type, CTCBase* component)
    {
        CInterfaceMap::SEntry* it = m_interfaces.last;
        while (it != m_interfaces.first && (it - 1)->type > type) { *it = *(it - 1); --it; }
        it->type = type;
        it->component = component;
        ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
};
class CThing : public CThingLookup { public: };
enum EConversationAttitude { ATTITUDE_NEUTRAL, ATTITUDE_HAPPY, ATTITUDE_ANGRY };
const char* GetAnimGroupName(EConversationAttitude attitude)
{
    return attitude == ATTITUDE_HAPPY ? "HAPPY" : attitude == ATTITUDE_ANGRY ? "ANGRY" : "NEUTRAL";
}
class CTCTalk : public CTCBase { public:
    EConversationAttitude Attitude;
    EConversationAttitude GetAttitude() { return Attitude; }
};
class CAnimFilterOfGroup { public:
    char Group[16];
    void SetGroup(const char* name) { std::strcpy(Group, name); }
};
static int g_baseUpdates;
struct CModeBase {
    CThing* ParentThing;
    void FrameUpdate() { ++g_baseUpdates; }
};
struct CModeConversation : CModeBase {
    EConversationAttitude CurrentAttitude;
    CAnimFilterOfGroup Filter;
    bool ResetIdleAnim;
    void FrameUpdate()
    {
        CTCTalk* talk;
        if (ParentThing->GetInterface(&talk, 15)) {
            EConversationAttitude attitude = talk->GetAttitude();
            if (attitude != CurrentAttitude) {
                CurrentAttitude = attitude;
                Filter.SetGroup(GetAnimGroupName(attitude));
                ResetIdleAnim = true;
            }
        }
        CModeBase::FrameUpdate();
    }
};

int main()
{
    CTCTalk talk;
    talk.Attitude = ATTITUDE_ANGRY;
    CThing villager;
    villager.ResetInterfaces();
    villager.AddInterface(15, &talk);
    CModeConversation m;
    m.ParentThing = &villager; m.CurrentAttitude = ATTITUDE_NEUTRAL; m.ResetIdleAnim = false;
    std::strcpy(m.Filter.Group, "NEUTRAL");
    g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = m.CurrentAttitude == ATTITUDE_ANGRY && std::strcmp(m.Filter.Group, "ANGRY") == 0 && m.ResetIdleAnim;
    m.ResetIdleAnim = false;
    m.FrameUpdate();
    pass = pass && !m.ResetIdleAnim;
    villager.ResetInterfaces();
    talk.Attitude = ATTITUDE_HAPPY;
    m.FrameUpdate();
    pass = pass && m.CurrentAttitude == ATTITUDE_ANGRY && !m.ResetIdleAnim && g_baseUpdates == 3;
    if (!pass) { std::printf("008d9350_TEST FAIL\n"); return 1; }
    std::printf("008d9350_TEST PASS\n");
    return 0;
}