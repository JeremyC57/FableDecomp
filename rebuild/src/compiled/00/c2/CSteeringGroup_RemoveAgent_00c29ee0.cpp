// CSteeringGroup::RemoveAgent(CSteeringWorld* world, CSteeringAgent* agent) -- unlink an agent from the group's singly linked member list (head +0x08, linked through the agent's +0x34), clear the agent's next and group words; when the group is left empty, tell the world's listener (+0x104, virtual slot 8), reset the group's index word (+0x04 = -1) and hand the group to the world's group list (+0x2c)
// Leaf: RemoveAgent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29EE0, length 112 bytes (101 + int3 padding), `ret 8`.
// Caller: the steering agent's OnCreate(world) @ 0x00C21620 (call 0x00C2165B: `if (agent->m_pGroup)
//   agent->m_pGroup->RemoveAgent(world, agent)` before the agent registers with the world's listeners and list).
// Callees: the world listener's virtual slot 8 (+0x20, the same slot OnCreate calls with the agent), the world group list's
//   AddGroup(group) @ 0x00C222B0 (on world +0x2c; it forwards to a container at +0x318 of that object @ 0x00C22B20).
// Name: no PDB covers the 0x00C1xxxx-0x00C2xxxx AI steering library, so the class and member names are descriptive; the
//   landed caller 0x00C21620 declares this callee as "CSteeringRegistry::Register" (its body is a removal).
// Shape: the head test first (`cmp eax,ecx ; jne`), the predecessor walk as a rotated top-tested while, the two clears from
//   one zero register, then the empty test; the index reset sits between the push and the call of AddGroup (source order:
//   listener, index, AddGroup).
struct CSteeringGroup;

class CSteeringAgent { public:
    unsigned char      _pad_0x00[0x30];
    CSteeringGroup*    m_pGroup;            // +0x30 the group this agent belongs to
    CSteeringAgent*    m_pNextInGroup;      // +0x34 next member of the group
};

struct CSteeringListener {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Notify(void* object);      // slot 8 (+0x20)
};

struct CSteeringGroupList {
    void AddGroup(CSteeringGroup* group);   // 0x00C222B0
};

struct CSteeringWorld {
    unsigned char      _pad_0x000[0x2c];
    CSteeringGroupList m_groups;            // +0x2c
    unsigned char      _pad_0x030[0xd4];
    CSteeringListener  m_listener;          // +0x104
};

struct CSteeringGroup {
    void*              __vftable;           // +0x00
    long               m_index;             // +0x04 (-1 = not placed)
    CSteeringAgent*    m_pFirst;            // +0x08 first member
    void RemoveAgent(CSteeringWorld* world, CSteeringAgent* agent);
};

void CSteeringGroup::RemoveAgent(CSteeringWorld* world, CSteeringAgent* agent)
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