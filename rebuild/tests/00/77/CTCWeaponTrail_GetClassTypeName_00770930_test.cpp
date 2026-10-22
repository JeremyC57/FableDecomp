#include <cstdio>
struct CCharString {
    char* p;
    static int last;
    CCharString(const char* s, int n);
};
int CCharString::last=0;
CCharString::CCharString(const char* s, int n){ p=(char*)s; last=n; }
struct CTCWeaponTrail { CCharString GetClassTypeName(); };
int main(){
    CTCWeaponTrail a;
    CCharString r = a.GetClassTypeName();
    if(r.p==(char*)0x123b46c && CCharString::last==-1) printf("00770930_TEST PASS\n");
    else printf("FAIL %p %d\n", (void*)r.p, CCharString::last);
    return 0;
}