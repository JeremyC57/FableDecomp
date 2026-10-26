// CSteeringGroup::AddAgent(CSteeringAgent* agent) -- link an agent in at the head of the group's member list: the agent's group word (+0x30) = this, its next word (+0x34) = the old head (+0x08), the head = the agent
// Leaf: ?AddAgent@CSteeringGroup@@QAEXPAVCSteeringAgent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29EC0, 19 bytes, __thiscall, `ret 4`:
//   mov eax,[esp+4] ; mov [eax+30h],ecx ; mov edx,[ecx+8] ; mov [eax+34h],edx ; mov [ecx+8],eax ; ret 4
// Naming (sweep1, 2026-10-25): was landed as CEngineInternalPrimitive::Update (a propagated manifest label);
//   descriptive (no PDB covers this library). FINDINGS chunk 436: it is the inverse of the landed
//   CSteeringGroup::RemoveAgent 0x00C29EE0, maintaining the same list (head +0x08 of the group, vtable 0x012B40A4, linked
//   through the agent's +0x34, the agent's group word +0x30); callers 0x00C13285 and 0x00C29F91.
struct CSteeringGroup;

class CSteeringAgent { public:
    unsigned char      _pad_0x00[0x30];
    CSteeringGroup*    m_pGroup;                // +0x30 the group this agent belongs to
    CSteeringAgent*    m_pNextInGroup;          // +0x34 next member of the group
};

struct CSteeringGroup {
    void*              __vftable;               // +0x00 (0x012B40A4)
    long               m_index;                 // +0x04 (-1 = not placed)
    CSteeringAgent*    m_pFirst;                // +0x08 first member
    void AddAgent(CSteeringAgent* agent);
};

void CSteeringGroup::AddAgent(CSteeringAgent* agent)
{
    agent->m_pGroup = this;
    agent->m_pNextInGroup = m_pFirst;
    m_pFirst = agent;
}
