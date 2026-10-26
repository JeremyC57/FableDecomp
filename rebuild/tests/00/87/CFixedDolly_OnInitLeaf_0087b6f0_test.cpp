// Behaviour test for 0x0087B6F0 CFixedDolly::OnInitLeaf: the target's type-0x3f component of kind 1 supplies DollyVector.
// (The retail fallback arm reads a never-assigned pointer and the absent-interface path reads garbage, so only the defined path is exercised.)
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CTCBase { public:
    int kind; C3DVector centre;
    int GetKind() { return kind; }
    C3DVector GetCentre() { return centre; }
};
class CThing { public:
    unsigned int m_presentBits[9]; CTCBase* m_component;
    template<class T> bool GetInterface(T** ppOut, int type)
    {
        if (m_presentBits[type >> 5] & (1u << (type & 31))) { *ppOut = (T*)m_component; return true; }
        return false;
    }
};
struct CFixedDolly {
    CThing* m_target; C3DVector DollyVector;
    void OnInitLeaf()
    {
        CTCBase* pComponent = 0;
        m_target->GetInterface(&pComponent, 0x3f);
        CTCBase* pFallback = pComponent;
        DollyVector = (pComponent->GetKind() == 1 ? pComponent : pFallback)->GetCentre();
    }
};
int main()
{
    CTCBase comp; comp.kind = 1; comp.centre.X = 1.0f; comp.centre.Y = 2.0f; comp.centre.Z = 3.0f;
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_presentBits[1] = 0x80000000u; t.m_component = &comp;
    CFixedDolly d; d.m_target = &t; d.DollyVector.X = d.DollyVector.Y = d.DollyVector.Z = 0.0f;
    d.OnInitLeaf();
    if (d.DollyVector.X != 1.0f || d.DollyVector.Y != 2.0f || d.DollyVector.Z != 3.0f) { std::printf("0087b6f0_TEST FAIL\n"); return 1; }
    std::printf("0087b6f0_TEST PASS\n");
    return 0;
}