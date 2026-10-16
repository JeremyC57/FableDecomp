// Behaviour test for 0x005C024A CTCInventoryBase::AddCategory
#include <cstdio>
static int g_destroyed = 0;
struct CDef { long RefCount; void Destroy() { ++g_destroyed; } };
struct CDefPointer {
    CDef* Object;
    CDefPointer(CDef* o) : Object(o) { if (o) ++o->RefCount; }
    CDefPointer(const CDefPointer& x) : Object(x.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
};
struct CDefPointerSlot { CDef* Object; };
static CDef g_def = { 0 };
static bool GetDef(long idx, CDefPointerSlot* out) { if (idx != 7) return false; ++g_def.RefCount; out->Object = &g_def; return true; }
struct CCategory {
    CDefPointer def; long idx; void* owner;
    CCategory(CDefPointer d, long i) : def(d), idx(i), owner(0) {}
    void SetOwner(void* o) { owner = o; }
};
struct CInv {
    CCategory* cats[4]; int n;
    void AddCategory(long defIndex, void* owner)
    {
        CDefPointerSlot def;
        def.Object = 0;
        bool found = GetDef(defIndex, &def);
        CDef* object = def.Object;
        if (found) {
            cats[n++] = new CCategory(CDefPointer(object), defIndex);
            cats[n - 1]->SetOwner(owner);
        }
        if (object) {
            if (--object->RefCount == 0)
                g_destroyed += 100;
        }
    }
};
int main()
{
    CInv inv; inv.n = 0; int owner;
    inv.AddCategory(3, &owner);
    inv.AddCategory(7, &owner);
    bool ok = inv.n == 1 && inv.cats[0]->idx == 7 && inv.cats[0]->owner == &owner && g_def.RefCount == 1 && g_destroyed == 0;
    delete inv.cats[0];
    if (g_destroyed != 1) ok = false;
    if (!ok) { std::printf("005c024a_TEST FAIL\n"); return 1; }
    std::printf("005c024a_TEST PASS\n");
    return 0;
}
