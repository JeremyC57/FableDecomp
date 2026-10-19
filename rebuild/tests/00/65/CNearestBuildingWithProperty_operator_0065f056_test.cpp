#include <stdio.h>
// Behaviour test for CNearestBuildingWithProperty::operator() (0x0065F056): keeps the nearest building whose
// sim definition has the property, and releases the temporary def pointer.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { float s = Z * Z; s += X * X; s += Y * Y; return s; }
};
struct CRefCounted { long Count; void DecRef() { --Count; } long GetRefCount() const { return Count; } };
static int g_destroyed = 0;
struct CBuildingSimDefinition {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    CRefCounted Ref;
    unsigned char _pad08[0x3c - 0x08];
    unsigned long BuildingProperties;
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(T* p) : Object(p) { if (p) ++p->Ref.Count; }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
class CThing { public: C3DVector Position; CBuildingSimDefinition* Def;
    CThing() : Position(0, 0, 0) {}
    const C3DVector& GetPosition() const { return Position; } };
class CThingBuilding : public CThing { public:
    CDefPointer<CBuildingSimDefinition> PeekPSimDefinition() const { return CDefPointer<CBuildingSimDefinition>(Def); } };
class CBaseIntelligentPointer { public: void* m_pItem; CThing* GetPItem() const { return (CThing*)m_pItem; } };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
struct CNearestBuildingWithProperty {
    unsigned long BuildingProperty; C3DVector Pos; CThingBuilding* NearestBuilding; float BestDistanceSquared;
    CNearestBuildingWithProperty() : Pos(0, 0, 0) {}
    void operator()(const CIntelligentPointer<CThing>& thing);
};
void CNearestBuildingWithProperty::operator()(const CIntelligentPointer<CThing>& thing)
{
    CDefPointer<CBuildingSimDefinition> def = ((CThingBuilding*)thing.GetPItem())->PeekPSimDefinition();
    if (def.Object) {
        if (def->BuildingProperties & BuildingProperty) {
            float distanceSquared = (thing.GetPItem()->GetPosition() - Pos).GetLengthSquared();
            if (NearestBuilding == 0 || distanceSquared < BestDistanceSquared) {
                NearestBuilding = (CThingBuilding*)thing.GetPItem();
                BestDistanceSquared = distanceSquared;
            }
        }
    }
}
int main()
{
    int ok = 1;
    CBuildingSimDefinition d1, d2; d1.Ref.Count = 1; d1.BuildingProperties = 4; d2.Ref.Count = 2; d2.BuildingProperties = 6;
    CThingBuilding a, b, c; a.Def = &d1; a.Position = C3DVector(3, 0, 0); b.Def = &d2; b.Position = C3DVector(1, 1, 1); c.Def = 0;
    CNearestBuildingWithProperty f; f.BuildingProperty = 2; f.Pos = C3DVector(0, 0, 0); f.NearestBuilding = 0; f.BestDistanceSquared = 0;
    CIntelligentPointer<CThing> p; p.m_pItem = &a; f(p);
    if (f.NearestBuilding != 0 || d1.Ref.Count != 1) ok = 0;
    p.m_pItem = &c; f(p);
    p.m_pItem = &b; f(p);
    if (f.NearestBuilding != &b || f.BestDistanceSquared != 3.0f || d2.Ref.Count != 2) ok = 0;
    f.BuildingProperty = 4; f(p);
    p.m_pItem = &a; f(p);
    if (f.NearestBuilding != &b) ok = 0;
    d1.BuildingProperties = 0; f.BestDistanceSquared = 100.0f; p.m_pItem = &a; f(p);
    if (f.NearestBuilding != &b || g_destroyed != 0) ok = 0;
    if (ok) { printf("0065f056_TEST PASS\n"); return 0; }
    printf("0065f056_TEST FAIL\n"); return 1;
}