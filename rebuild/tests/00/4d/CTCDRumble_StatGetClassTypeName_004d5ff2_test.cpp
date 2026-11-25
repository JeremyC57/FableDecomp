// Behaviour test for 0x004D5FF2 (CTCDRumble class-type name, stand-in self CCharString::StatGetClassTypeName_8): links the row
// object (declared here, not defined) and asserts what it hands to the CCharString constructor: the retail text "CTCDRumble",
// length -1, one call on the same object, which it returns.
#include <cstdio>
#include <string.h>
static const char* seen_s = 0; static int seen_n = 0; static int calls = 0; static void* seen_this = 0;
class CCharString { public:
  char* p;
  CCharString* ctorB(const char* s, int n);   // the row's callee (0x0099EBF0), defined here
  CCharString* StatGetClassTypeName_8();      // definition linked from the row object
};
CCharString* CCharString::ctorB(const char* s, int n){ seen_s=s; seen_n=n; calls++; seen_this=this; return this; }
int main(){
  CCharString c; c.p=0;
  CCharString* r = c.StatGetClassTypeName_8();
  if(r==&c && calls==1 && seen_this==(void*)&c && seen_s!=0 && strcmp(seen_s, "CTCDRumble")==0 && seen_n==-1)
    printf("OK_004d5ff2\n");
  else { printf("FAIL s=%s n=%d calls=%d ret=%p\n", seen_s ? seen_s : "(null)", seen_n, calls, (void*)r); return 1; }
  return 0;
}
