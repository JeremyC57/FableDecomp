#include <cstdio>
#include <stddef.h>
#include <new>
struct CThing { int x; }; struct CDecapitationDef_Calls;
struct CTCBase_Calls { virtual void v00() { } CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; CTCBase_Calls(CThing* p) : ParentThing(p), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { } };
template<class T> struct CDefPointer { T* m_pDef; CDefPointer() : m_pDef(0) { } };
struct CTCDecapitation : CTCBase_Calls {
    bool Decapitated; CDefPointer<const CDecapitationDef_Calls> PDef; virtual void v00() { }
    CTCDecapitation(CThing* p) : CTCBase_Calls(p), Decapitated(false), PDef() { }
};
int main()
{
    CThing thing; unsigned char raw[sizeof(CTCDecapitation)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    CTCDecapitation* d = new (raw) CTCDecapitation(&thing);
    if (d->ParentThing != &thing || d->Decapitated || d->PDef.m_pDef != 0 || offsetof(CTCDecapitation, Decapitated) != 0xc || offsetof(CTCDecapitation, PDef) != 0x10) { std::printf("004db207_TEST FAIL\n"); return 1; }
    std::printf("004db207_TEST PASS\n"); return 0;
}
