#include <cstdio>
// ---- test model: a thing's interface bitset + sorted {type, component} table, intelligent pointers ----
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CInterfaceMap::SEntry m_table[8];
    int m_count;
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_flags91 = 0; m_count = 0; Sync(); }
    void Sync() { m_interfaces.first = m_table; m_interfaces.last = m_table + m_count; }
    void Add(long type, void* comp)       // keep the table sorted
    {
        int i = m_count;
        while (i > 0 && m_table[i - 1].type > type) { m_table[i] = m_table[i - 1]; --i; }
        m_table[i].type = type; m_table[i].component = comp; ++m_count;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        Sync();
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* p) { m_pItem = p; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain { CThing* PCreature; };

struct CTCMarker { CBaseIntelligentPointer PUser; };

struct CAIStateGroup_Gaze {
    CBaseIntelligentPointer GazeMarker;
    void OnReplace()
    {
        if (IS_THING_ALIVE(GazeMarker)) {
            CTCMarker* marker;
            GazeMarker.GetPItem()->GetInterface(&marker, 0x2f);
            marker->PUser.SetPItem(0);
        }
        GazeMarker.SetPItem(0);
    }
};

int main()
{
    CThing markerThing, user;
    markerThing.Init(); user.Init();
    CTCMarker marker; marker.PUser.SetPItem(&user);
    markerThing.Add(0x2f, &marker);
    CAIStateGroup_Gaze g; g.GazeMarker.SetPItem(&markerThing);
    markerThing.m_flags91 = 1;                  // dead marker: user kept
    g.OnReplace();
    if (marker.PUser.GetPItem() != &user || g.GazeMarker.GetPItem() != 0) { std::printf("009183a0_TEST FAIL\n"); return 1; }
    markerThing.m_flags91 = 0; g.GazeMarker.SetPItem(&markerThing);
    g.OnReplace();
    if (marker.PUser.GetPItem() != 0 || g.GazeMarker.GetPItem() != 0) { std::printf("009183a0_TEST FAIL\n"); return 1; }
    std::printf("009183a0_TEST PASS\n");
    return 0;
}
