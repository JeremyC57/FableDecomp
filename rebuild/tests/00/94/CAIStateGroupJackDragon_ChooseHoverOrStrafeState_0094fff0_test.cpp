#include <cstdio>
// ---- test model: a thing's interface bitset + sorted {type, component} table, intelligent pointers ----
class CThing;
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
class CThing { public:
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
class CAIBrain { public: CThing* PCreature; };

#include <cmath>
class C3DVector { public: float x, y, z; };
class C2DVector { public: float x, y, z; float Length() const { return (float)std::sqrt((double)(x * x + y * y)); } };
static int g_released = 0;
class CJackDragonDef { public: long refs; float HoverRange; };
struct CDefPointer { CJackDragonDef* m_p; ~CDefPointer() { if (m_p && --m_p->refs == 0) ++g_released; } };
static CJackDragonDef g_def = { 0, 10.0f };

struct CAIStateGroup_JackDragon {
    C3DVector m_enemyPos, m_landedPos;
    long ChooseHoverOrStrafeState()
    {
        const C3DVector& landed = m_landedPos;
        const C3DVector& target = m_enemyPos;
        C2DVector delta;
        delta.x = target.x - landed.x;
        delta.y = target.y - landed.y;
        CDefPointer def; def.m_p = &g_def; ++g_def.refs;
        if (delta.Length() < def.m_p->HoverRange)
            return 2;
        return 4;
    }
};

int main()
{
    CAIStateGroup_JackDragon d;
    d.m_landedPos.x = 0; d.m_landedPos.y = 0; d.m_landedPos.z = 0;
    d.m_enemyPos.x = 6; d.m_enemyPos.y = 8; d.m_enemyPos.z = 100;   // XY distance 10, z ignored
    bool ok = d.ChooseHoverOrStrafeState() == 4;
    d.m_enemyPos.y = 7.9f; ok = ok && d.ChooseHoverOrStrafeState() == 2;
    ok = ok && g_released == 2 && g_def.refs == 0;
    if (!ok) { std::printf("0094fff0_TEST FAIL\n"); return 1; }
    std::printf("0094fff0_TEST PASS\n");
    return 0;
}