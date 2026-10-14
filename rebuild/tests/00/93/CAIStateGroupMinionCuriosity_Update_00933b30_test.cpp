#include <cstdio>
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last; SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; } };
struct CThing { unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char _pad1[0x45]; unsigned char m_flags91;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) { long type = interfaceType; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last; *ppOut = (T*)it->component; return true; } return false; } };
struct CAIBrain { unsigned char _pad[0x18]; CThing* Creature; };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; long ScriptAIPriority; void* Name_PStringData; unsigned char Terminated, Priority, Enabled, _pad_0x13; };
#define SETUP(TYPE, COMP) CInterfaceMap::SEntry e[2] = { { 0x10, 0 }, { TYPE, (CTCBase*)&COMP } }; CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_interfaces.first = e; t.m_interfaces.last = e + 2; CAIBrain b; b.Creature = &t;
#define ENABLE(TYPE) t.m_presentBits[(TYPE) >> 5] |= 1UL << ((TYPE) & 31)
struct CTCPerceiveThing { float r; float GetExtendedSightRadius() { return r; } };
static float g_radius = 0; static int g_term = 0;
struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase { float x, y, z; bool MoveToCuriosityProximity(float radius) { g_radius = radius; return true; } void Terminate() { ++g_term; } virtual void Update(); };
void CAIStateGroup_MinionCuriosity::Update() { CTCPerceiveThing* p; PBrain->Creature->GetInterface(&p, 0x34); MoveToCuriosityProximity(p->GetExtendedSightRadius()); Terminate(); }
int main() { CTCPerceiveThing pt = { 12.5f }; SETUP(0x34, pt); ENABLE(0x34); CAIStateGroup_MinionCuriosity g; g.PBrain = &b;
    g.Update(); if (g_radius != 12.5f || g_term != 1) { std::printf("00933b30_TEST FAIL\n"); return 1; }
    std::printf("00933b30_TEST PASS\n"); return 0; }