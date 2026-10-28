#include <stdio.h>
#include <string.h>
#include <new>
// CUnprovenThreeByteState_0041a780::CUnprovenThreeByteState_0041a780() -- Initialize the observed default state.
// Leaf: ??0CUnprovenThreeByteState_0041a780@@QAE@XZ
// Identity: descriptive. No true callers found: raw xref005817ac is the rel32 displacement of an unrelated call, not an address-taking reference. Three byte writes0/1/2 and constructor return-this proven; original field/class identity unknown.
// Layout: own exact writes; no vptr or base stores observed. SDK scalar widths portable in host tests; unused union alternatives omitted while preserving original reserved arrays and extents. No engine owner header exists for this unproven identity.
class CUnprovenThreeByteState_0041a780{public:unsigned char First,Second,Third;CUnprovenThreeByteState_0041a780();};CUnprovenThreeByteState_0041a780::CUnprovenThreeByteState_0041a780():First(0),Second(0),Third(0){}
#ifdef _MSC_VER
typedef char ExactOwnerSize[sizeof(CUnprovenThreeByteState_0041a780)==3?1:-1];
#endif
int main(){typedef CUnprovenThreeByteState_0041a780 Owner;union Storage{double align;unsigned char bytes[sizeof(Owner)];}storage;memset(storage.bytes,0x5a,sizeof(storage.bytes));unsigned char expected[sizeof(Owner)];memcpy(expected,storage.bytes,sizeof(expected));Owner* p=new(storage.bytes)Owner;bool ok=p->First==0&&p->Second==0&&p->Third==0;expected[0]=expected[1]=expected[2]=0;ok=ok&&memcmp(expected,p,sizeof(Owner))==0;p->~Owner();puts(ok?"0041a780_TEST PASS":"0041a780_TEST FAIL");return ok?0:1;}