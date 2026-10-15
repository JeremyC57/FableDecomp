// Behaviour test for 0075d8f0 (CTCCameraPointShapeActivationBase::IsLeafCriteriaMet): true only when camera points are enabled, the parent has interface 0x44 and that component's flag is set.
#include <cstdio>

static bool g_cameraPointsEnabled = false;

struct CTCCameraShape {
    bool m_isActive;
};

struct CInterfaceMap {
    struct SEntry { long type; CTCCameraShape* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCBase { CThing* ParentThing; };

struct CTCCameraPointShapeActivationBase : CTCBase {
    bool IsLeafCriteriaMet()
    {
        if (g_cameraPointsEnabled) {
            CTCCameraShape* shape;
            if (ParentThing->GetInterface(0x44, &shape))
                return shape->m_isActive;
        }
        return false;
    }
};

int main()
{
    CTCCameraShape shape; shape.m_isActive = true;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 8; entries[0].component = 0;
    entries[1].type = 0x44; entries[1].component = &shape;
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CTCCameraPointShapeActivationBase cp; cp.ParentThing = &thing;
    bool ok = true;
    ok = ok && !cp.IsLeafCriteriaMet();                 // disabled
    g_cameraPointsEnabled = true;
    ok = ok && !cp.IsLeafCriteriaMet();                 // no interface bit
    thing.m_presentBits[2] |= 1UL << 4;                 // type 0x44
    ok = ok && cp.IsLeafCriteriaMet();
    shape.m_isActive = false;
    ok = ok && !cp.IsLeafCriteriaMet();
    if (!ok) { std::printf("0075d8f0_TEST FAIL\n"); return 1; }
    std::printf("0075d8f0_TEST PASS\n");
    return 0;
}