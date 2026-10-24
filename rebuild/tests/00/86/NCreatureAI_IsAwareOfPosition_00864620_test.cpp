// Behaviour test for 0x00864620: IsAwareOfPosition = perceive component present, 2D distance < extended sight radius,
// position in FOV, and navigation line of sight.
#include <cstdio>

class CTCBase;

class C3DVector { public: float X, Y, Z; };

struct CInterfaceMap
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type) const
    {
        SEntry* it = first;
        while (it != last && it->type < type) ++it;
        return it;
    }
};

class CTCPerceiveThing
{
public:
    float radius;
    float GetExtendedSightRadius() const { return radius; }
};

class CTCCreatureNavigation
{
public:
    bool los;
    bool IsPosNavLineOfSightToPos(const C3DVector&, const C3DVector&) const { return los; }
};

class CThing
{
public:
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;
    C3DVector     m_pos;

    const C3DVector& GetPosition() const { return m_pos; }
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1U << (type & 31)))
            return true;
        return false;
    }
    CInterfaceMap::SEntry* FindEntry(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end) sel = end;
        else if (it->type <= type) sel = it;
        else sel = end;
        return sel;
    }
    template <class T> bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1U << (type & 31)))
        {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing
{
public:
    bool fov;
    bool IsPosInFOV(const C3DVector&, bool flag) const { return fov && flag; }
};

bool IsAwareOfPosition(const CThingCreatureBase* creature, const C3DVector& pos)
{
    if (creature && creature->HasTC(0x34))
    {
        float radius = ((const CTCPerceiveThing*)creature->FindEntry(0x34)->component)->GetExtendedSightRadius();
        const C3DVector& here = creature->GetPosition();
        float dx = here.X - pos.X;
        float dy = here.Y - pos.Y;
        if (dx * dx + dy * dy < radius * radius && creature->IsPosInFOV(pos, true))
        {
            const CTCCreatureNavigation* navigation;
            creature->GetInterface(7, &navigation);
            if (navigation->IsPosNavLineOfSightToPos(creature->GetPosition(), pos))
                return true;
        }
    }
    return false;
}

int main()
{
    CTCPerceiveThing perceive; perceive.radius = 10.0f;
    CTCCreatureNavigation nav; nav.los = true;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 7;    entries[0].component = (CTCBase*)&nav;
    entries[1].type = 0x34; entries[1].component = (CTCBase*)&perceive;
    CThingCreatureBase c;
    for (int i = 0; i < 9; ++i) c.m_presentBits[i] = 0;
    c.m_presentBits[0] = 0x80; c.m_presentBits[1] = 0x100000;
    c.m_interfaces.first = entries; c.m_interfaces.last = entries + 2;
    c.m_pos.X = 0; c.m_pos.Y = 0; c.m_pos.Z = 0; c.fov = true;
    C3DVector near; near.X = 3; near.Y = 4; near.Z = 100;     // 2D distance 5, Z ignored
    C3DVector far;  far.X = 8;  far.Y = 8;  far.Z = 0;        // 2D distance ~11.3
    int fails = 0;
    if (!IsAwareOfPosition(&c, near)) ++fails;
    if (IsAwareOfPosition(&c, far)) ++fails;
    if (IsAwareOfPosition(0, near)) ++fails;
    c.fov = false;
    if (IsAwareOfPosition(&c, near)) ++fails;
    c.fov = true; nav.los = false;
    if (IsAwareOfPosition(&c, near)) ++fails;
    nav.los = true; c.m_presentBits[1] = 0;
    if (IsAwareOfPosition(&c, near)) ++fails;
    if (fails) { std::printf("00864620_TEST FAIL\n"); return 1; }
    std::printf("00864620_TEST PASS\n");
    return 0;
}