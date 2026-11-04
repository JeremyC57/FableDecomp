// Behaviour test for 0x00441EE0 CPrimitiveHandleSet_00441ee0::~CPrimitiveHandleSet_00441ee0: the member-wise destructor
// over a portable model of the retail logic -- vectors destroy their handles, each handle's destructor notifies the
// primitive when it holds the last reference (count 1) and then releases it (Delete at count 0).
#include <cstdio>
#include <vector>
static int g_notified = 0, g_deleted = 0;
struct Prim { long RefCount; void NotifyReleased() { ++g_notified; } void Delete() { ++g_deleted; } };
struct Ptr {
    Prim* Object;
    Ptr() : Object(0) { }
    void Release() { Prim* o = Object; if (o) { if (--o->RefCount == 0) o->Delete(); Object = 0; } }
    void Reset(Prim* p) { Release(); Object = p; if (p) ++p->RefCount; }
    unsigned long GetRefCount() const { return Object ? Object->RefCount : 0; }
    ~Ptr() { Release(); }
};
struct Handle {
    Ptr Data;
    Handle() { }
    Handle(const Handle& o) { Data.Reset(o.Data.Object); }
    void Clear() { if (Data.GetRefCount() == 1) Data.Object->NotifyReleased(); Data.Reset(0); }
    ~Handle() { Clear(); }
};
struct Set {
    std::vector<Handle> Handles1c;
    Handle Handle2c, Handle34;
    std::vector<Handle> Handles3c, Handles48;
};
int main() {
    bool ok = true;
    {
        Prim a = { 0 }, b = { 0 }, c = { 0 };
        {
            Set s;
            s.Handle2c.Data.Reset(&a);                       // sole holder: notified, then deleted
            s.Handle34.Data.Reset(&b); b.RefCount++;         // shared (count 2): not notified, not deleted
            s.Handles3c.resize(1); s.Handles3c[0].Data.Reset(&c);
        }
        ok = ok && g_notified == 2 && g_deleted == 2 && b.RefCount == 1;
    }
    { Set e; }
    ok = ok && g_notified == 2 && g_deleted == 2;
    std::printf(ok ? "00441ee0_TEST PASS\n" : "00441ee0_TEST FAIL\n");
    return ok ? 0 : 1;
}