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
struct CTCBoss { long phase; long GetCurrentPhase() { return phase; } };
struct CAIStateGroup_MazeRangedCombat : CAIStateGroupBase { unsigned char _pad14[0xc]; long LastCompletedPhase; virtual void OnActivate(); };
void CAIStateGroup_MazeRangedCombat::OnActivate() { CTCBoss* p; PBrain->Creature->GetInterface(&p, 0xAD); LastCompletedPhase = p->GetCurrentPhase(); }
int main() { CTCBoss boss = { 3 }; SETUP(0xAD, boss); ENABLE(0xAD); CAIStateGroup_MazeRangedCombat g; g.PBrain = &b; g.LastCompletedPhase = 0;
    g.OnActivate(); if (g.LastCompletedPhase != 3) { std::printf("00954630_TEST FAIL\n"); return 1; }
    std::printf("00954630_TEST PASS\n"); return 0; }