// Behaviour test for 006ca680 CIVCountedPointer::operator=: on a different pointee, drop a reference from the old one
// (deleting it at zero), take the new one and add a reference; self/same assignment is a no-op.
#include <cstdio>
struct CIVCountedPointeeBase {
    int deleted;
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : deleted(0), IVRefCount(0) {}
    virtual void Destroy(int) {}
    virtual void Delete() { ++deleted; }
    virtual ~CIVCountedPointeeBase() {}
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            CIVCountedPointeeBase* old = Object;
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};
int main()
{
    CIVCountedPointeeBase x, y; x.IVRefCount = 1; y.IVRefCount = 2;
    CIVCountedPointer px; px.Object = &x;
    CIVCountedPointer py; py.Object = &y;
    CIVCountedPointer pn; pn.Object = 0;
    int ok = 1;
    if (&(px = px) != &px || x.IVRefCount != 1) ok = 0;
    px = py;                        /* x drops to 0 and is deleted, y gains one */
    if (px.Object != &y || x.deleted != 1 || y.IVRefCount != 3) ok = 0;
    px = pn;                        /* y drops to 2, not deleted */
    if (px.Object != 0 || y.IVRefCount != 2 || y.deleted) ok = 0;
    pn = py;                        /* from null */
    if (pn.Object != &y || y.IVRefCount != 3) ok = 0;
    if (!ok) { std::printf("006ca680_TEST FAIL\n"); return 1; }
    std::printf("006ca680_TEST PASS\n");
    return 0;
}
