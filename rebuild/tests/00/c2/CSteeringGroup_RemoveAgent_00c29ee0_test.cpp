// Behaviour test for CSteeringGroup::RemoveAgent @ 0x00C29EE0 (unlink a member; an emptied group is announced and recycled).
#include <cstdio>

struct CSteeringGroup;

class CSteeringAgent { public:
    CSteeringGroup* m_pGroup;
    CSteeringAgent* m_pNextInGroup;
};

struct CSteeringListener {
    int notifications;
    void* last;
    void Notify(void* object) { ++notifications; last = object; }
};

struct CSteeringGroupList {
    int added;
    CSteeringGroup* last;
    void AddGroup(CSteeringGroup* group) { ++added; last = group; }
};

struct CSteeringWorld {
    CSteeringGroupList m_groups;
    CSteeringListener  m_listener;
};

struct CSteeringGroup {
    long            m_index;
    CSteeringAgent* m_pFirst;
    void RemoveAgent(CSteeringWorld* world, CSteeringAgent* agent)
    {
        if (m_pFirst == agent) {
            m_pFirst = agent->m_pNextInGroup;
        } else {
            CSteeringAgent* prev = m_pFirst;
            while (prev->m_pNextInGroup != agent)
                prev = prev->m_pNextInGroup;
            prev->m_pNextInGroup = agent->m_pNextInGroup;
        }
        agent->m_pNextInGroup = 0;
        agent->m_pGroup = 0;
        if (m_pFirst == 0) {
            world->m_listener.Notify(this);
            m_index = -1;
            world->m_groups.AddGroup(this);
        }
    }
};

int main()
{
    CSteeringWorld world;
    world.m_groups.added = 0; world.m_groups.last = 0;
    world.m_listener.notifications = 0; world.m_listener.last = 0;
    CSteeringGroup group;
    group.m_index = 3;
    CSteeringAgent a, b, c;
    a.m_pGroup = b.m_pGroup = c.m_pGroup = &group;
    group.m_pFirst = &a; a.m_pNextInGroup = &b; b.m_pNextInGroup = &c; c.m_pNextInGroup = 0;
    bool ok = true;
    group.RemoveAgent(&world, &b);                          // middle
    if (a.m_pNextInGroup != &c || b.m_pGroup != 0 || b.m_pNextInGroup != 0 || world.m_listener.notifications != 0) ok = false;
    group.RemoveAgent(&world, &a);                          // head
    if (group.m_pFirst != &c || group.m_index != 3 || world.m_groups.added != 0) ok = false;
    group.RemoveAgent(&world, &c);                          // last member: group emptied
    if (group.m_pFirst != 0 || group.m_index != -1) ok = false;
    if (world.m_listener.notifications != 1 || world.m_listener.last != &group) ok = false;
    if (world.m_groups.added != 1 || world.m_groups.last != &group || c.m_pGroup != 0) ok = false;
    if (!ok) { std::printf("00c29ee0_TEST FAIL\n"); return 1; }
    std::printf("00c29ee0_TEST PASS\n");
    return 0;
}