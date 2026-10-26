// Behaviour test for 0x0075DB90 CTCCameraPointFixedDolly::GetFixedDollyVector: looks up the parent thing's interface
// 0x44 component (bitset + lower-bound map) and returns the vector its GetPrincipalVector fills.
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CTCShapeManager {
public:
    C3DVector m_principal;
    int m_calls;
    bool GetPrincipalVector(C3DVector& out) const
    {
        ++const_cast<CTCShapeManager*>(this)->m_calls;
        out = m_principal;
        return true;
    }
};

struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};

class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool PeekTC(long type, const T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap::SEntry* it = m_interfaces.LowerBound(&key);
            if (it == m_interfaces.last || type < it->type)
                it = m_interfaces.last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCCameraPointFixedDolly { public:
    CThing* ParentThing;
    C3DVector GetFixedDollyVector() const
    {
        const CTCShapeManager* shapes = 0;
        ParentThing->PeekTC(0x44, &shapes);
        C3DVector v;
        shapes->GetPrincipalVector(v);
        return v;
    }
};

int main()
{
    CTCShapeManager other; other.m_principal = C3DVector(9.0f, 9.0f, 9.0f); other.m_calls = 0;
    CTCShapeManager shapes; shapes.m_principal = C3DVector(1.5f, -2.0f, 4.25f); shapes.m_calls = 0;
    CInterfaceMap::SEntry entries[3] = { { 0x10, &other }, { 0x44, &shapes }, { 0x50, &other } };
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[0x44 >> 5] |= 1UL << (0x44 & 31);   // word +0x28 (index 2), bit 0x10
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    CTCCameraPointFixedDolly dolly; dolly.ParentThing = &thing;
    C3DVector r = dolly.GetFixedDollyVector();
    int ok = 1;
    if (r.X != 1.5f || r.Y != -2.0f || r.Z != 4.25f || shapes.m_calls != 1 || other.m_calls != 0) ok = 0;
    if (thing.m_presentBits[2] != 0x10UL) ok = 0;
    if (!ok) { std::printf("0075db90_TEST FAIL\n"); return 1; }
    std::printf("0075db90_TEST PASS\n");
    return 0;
}