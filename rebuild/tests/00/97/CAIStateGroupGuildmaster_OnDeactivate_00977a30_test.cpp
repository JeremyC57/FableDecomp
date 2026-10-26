#include <cstdio>
// Behaviour model of CAIStateGroup_Guildmaster::OnDeactivate @ 0x00977A30: remove mode 0x3B only when present.
class CTCBase { public: int x; };
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; } };
class CThing { public: unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) { long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type); if (it == map->last || interfaceType < it->type) it = map->last; *ppOut = (T*)it->component; return true; }
        return false; } };
static bool g_has = true; static long g_removed = 0;
class CTCCreatureModeManager : public CTCBase { public: bool HasMode(unsigned long) { return g_has; } void RemoveMode(unsigned long m) { g_removed = m; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThing* PCreature; };
struct CAIStateGroup_Guildmaster { CAIBrain* PBrain;
    void OnDeactivate() { CTCCreatureModeManager* p = 0; PBrain->PCreature->GetInterface(&p, 0x31); if (p->HasMode(0x3B)) p->RemoveMode(0x3B); } };
int main()
{
    CTCCreatureModeManager modes; CInterfaceMap::SEntry entries[2] = { { 0x31, &modes }, { 0x40, 0 } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0; thing.m_presentBits[1] |= 1UL << 17;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CAIBrain brain; brain.PCreature = &thing; CAIStateGroup_Guildmaster g; g.PBrain = &brain;
    g.OnDeactivate(); bool ok = g_removed == 0x3B;
    g_removed = 0; g_has = false; g.OnDeactivate(); ok = ok && g_removed == 0;
    if (!ok) { std::printf("00977a30_TEST FAIL\n"); return 1; }
    std::printf("00977a30_TEST PASS\n");
    return 0;
}