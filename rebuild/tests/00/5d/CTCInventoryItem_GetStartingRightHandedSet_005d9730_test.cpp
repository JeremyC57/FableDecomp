// behaviour test for 005d9730: CTCInventoryItem::GetStartingRightHandedSet(long) -- a cached def maps its Orientation
// through GetRHSetFromOrientation; a failed cache returns the default set unchanged
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};
static const CRightHandedSet g_DefaultRHSet(C3DVector(0, 0, 1), C3DVector(0, 1, 0));
class CInventoryItemDef { public: int Orientation; };
static CInventoryItemDef g_defs[3];
static const CInventoryItemDef* s_CachedItemDef = 0;
static int g_mapped = -1;
static bool CacheDef(long defIndex)
{
    if (defIndex < 0 || defIndex >= 3) return false;
    s_CachedItemDef = &g_defs[defIndex];
    return true;
}
static CRightHandedSet GetRHSetFromOrientation(int orientation)
{
    g_mapped = orientation;
    return CRightHandedSet(C3DVector((float)orientation, 0, 0), C3DVector(0, 0, (float)-orientation));
}
static CRightHandedSet GetStartingRightHandedSet(long defIndex)
{
    if (CacheDef(defIndex))
        return GetRHSetFromOrientation(s_CachedItemDef->Orientation);
    return g_DefaultRHSet;
}
int main()
{
    int bad = 0;
    if (sizeof(CRightHandedSet) != 24) ++bad;
    g_defs[2].Orientation = 5;
    CRightHandedSet a = GetStartingRightHandedSet(2);
    if (g_mapped != 5 || a.Up.X != 5.0f || a.Forward.Z != -5.0f) ++bad;
    g_mapped = -1;
    CRightHandedSet b = GetStartingRightHandedSet(-4);
    if (g_mapped != -1) ++bad;
    if (b.Up.X != 0 || b.Up.Y != 0 || b.Up.Z != 1 || b.Forward.X != 0 || b.Forward.Y != 1 || b.Forward.Z != 0) ++bad;
    if (bad) { std::printf("005d9730_TEST FAIL\n"); return 1; }
    std::printf("005d9730_TEST PASS\n");
    return 0;
}