#include <cstdio>
// Behaviour test for 0x005D92F0: the def is held across the conversion of its Orientation, then released.
static int g_destroyed = 0; static long g_refDuring = -1;
class CRightHandedSet { public: float m_v[6]; };
class CInventoryItemDef { public:
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long refCount;
    long Orientation;
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};
static CInventoryItemDef* g_def = 0;
static CRightHandedSet GetRHSetFromOrientation(long orientation)
{
    g_refDuring = g_def->refCount;
    CRightHandedSet s; for (int i = 0; i < 6; ++i) s.m_v[i] = (float)(orientation * 10 + i); return s;
}
struct CInventoryItemDefPointer {
    CInventoryItemDef* Object;
    CInventoryItemDefPointer(CInventoryItemDef* p) : Object(p) {}
    CInventoryItemDefPointer(const CInventoryItemDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CInventoryItemDefPointer() { Object->Release(); }
    CInventoryItemDef* operator->() const { return Object; }
};
class CTCBase { public: virtual void v00() {} void* ParentThing; };
class CTCInventoryItem : public CTCBase { public:
    CInventoryItemDefPointer PDef;
    CTCInventoryItem(CInventoryItemDef* d) : PDef(d) {}
    CRightHandedSet GetStartingRightHandedSet();
};
CRightHandedSet CTCInventoryItem::GetStartingRightHandedSet()
{
    CInventoryItemDefPointer def(PDef);
    return GetRHSetFromOrientation(def->Orientation);
}
int main()
{
    CInventoryItemDef def; def.refCount = 1; def.Orientation = 3; g_def = &def;
    CTCInventoryItem item(&def);
    CRightHandedSet s = item.GetStartingRightHandedSet();
    if (s.m_v[0] != 30.0f || s.m_v[5] != 35.0f || g_refDuring != 2 || def.refCount != 1 || g_destroyed != 0) { std::printf("005d92f0_TEST FAIL\n"); return 1; }
    std::printf("005d92f0_TEST PASS\n");
    return 0;
}