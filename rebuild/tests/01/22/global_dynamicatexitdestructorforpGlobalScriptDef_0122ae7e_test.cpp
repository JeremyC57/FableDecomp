// Behaviour test for the atexit destructor of NGlobalScriptDefs::pGlobalScriptDef (0x0122AE7E): drop the def's reference,
// destroy it (virtual slot 1) on the last one, and clear the pointer; a null pointer does nothing.
#include <stdio.h>
static int destroyed;
struct Def { virtual void v00() {} virtual void Destroy() { ++destroyed; } long m_refCount; };
template <class T> struct Ptr {
    T* Object;
    void Release() {
        if (Object) {
            T* object = Object;
            const long& count = Object->m_refCount;
            --Object->m_refCount;
            if (count == 0) object->Destroy();
            Object = 0;
        }
    }
};
int main()
{
    Def d; d.m_refCount = 2; Ptr<Def> p; p.Object = &d;
    p.Release();
    if (p.Object != 0 || d.m_refCount != 1 || destroyed) { printf("0122ae7e_TEST FAIL\n"); return 1; }
    p.Object = &d; p.Release();
    if (p.Object != 0 || d.m_refCount != 0 || destroyed != 1) { printf("0122ae7e_TEST FAIL\n"); return 1; }
    p.Release();
    if (destroyed != 1) { printf("0122ae7e_TEST FAIL\n"); return 1; }
    printf("0122ae7e_TEST PASS\n"); return 0;
}