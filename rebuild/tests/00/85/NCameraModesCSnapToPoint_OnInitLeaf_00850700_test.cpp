#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CTCBase { long dummy; };
struct SEntry { long type; CTCBase* component; };
struct CInterfaceMap {
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> bool PeekTC(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCCameraPointFixedPoint {
    CTCBase   base;
    long      type;
    bool      TrackThing;
    C3DVector LookVector;
    long GetCameraPointType() const { return type; }
};
struct CSnapToPoint {
    const CThing* camPoint;
    bool      TrackThing;
    C3DVector LookDirection;
    void OnInitLeaf()
    {
        const CTCCameraPointFixedPoint* point;
        camPoint->PeekTC(&point, 0x3f);
        const CTCCameraPointFixedPoint* fixedPoint = 0;
        if (point->GetCameraPointType() == 0) fixedPoint = point;
        TrackThing = fixedPoint->TrackThing;
        if (!TrackThing)
            LookDirection = fixedPoint->LookVector;
    }
};
int main()
{
    CTCCameraPointFixedPoint fp; fp.type = 0; fp.TrackThing = false;
    fp.LookVector.X = 0.0f; fp.LookVector.Y = 1.0f; fp.LookVector.Z = 0.5f;
    SEntry entries[2]; entries[0].type = 0x3f; entries[0].component = &fp.base; entries[1].type = 0x50; entries[1].component = 0;
    CThing t; int i = 0;
    for (i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_presentBits[1] = 1UL << 31;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 2;
    CSnapToPoint s; s.camPoint = &t; s.TrackThing = true; s.LookDirection.X = s.LookDirection.Y = s.LookDirection.Z = 7.0f;
    s.OnInitLeaf();
    if (s.TrackThing || s.LookDirection.Y != 1.0f || s.LookDirection.Z != 0.5f) { std::printf("00850700_TEST FAIL\n"); return 1; }
    fp.TrackThing = true; s.LookDirection.X = 7.0f;
    s.OnInitLeaf();
    if (!s.TrackThing || s.LookDirection.X != 7.0f) { std::printf("00850700_TEST FAIL\n"); return 1; }
    std::printf("00850700_TEST PASS\n");
    return 0;
}