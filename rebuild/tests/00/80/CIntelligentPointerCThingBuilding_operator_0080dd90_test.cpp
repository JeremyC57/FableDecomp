// Behaviour test for 0x0080DD90 CIntelligentPointer<CThingBuilding>::operator== (model copy).
#include <cstdio>
class CBaseObject { public: int id; };
class CThingBuilding : public CBaseObject {};
class CBaseIntelligentPointer {
public:
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) {}
    virtual void v00() {}
    CBaseObject* GetPItem() const { return PData; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00() {}
    bool operator==(const CIntelligentPointer<T>& other) const
    {
        CBaseObject* item = other.GetPItem();
        return item == GetPItem() ? true : false;
    }
};
int main()
{
    CThingBuilding a, b;
    CIntelligentPointer<CThingBuilding> p, q, r, n1, n2;
    p.PData = &a; q.PData = &a; r.PData = &b;
    if (!(p == q) || p == r || r == p || p == n1 || !(n1 == n2)) { std::printf("0080dd90_TEST FAIL\n"); return 1; }
    std::printf("0080dd90_TEST PASS\n");
    return 0;
}