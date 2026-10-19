#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
// CPairedIntelligentPointerRecord::DeletingDestructor -- destroys two intelligent pointers then optionally frees record.
// Retail0078e1f0 corrected45bytes. Concrete template owner remains unknown;
// CEngineSubPrimitiveText label is contradicted by two A01C10 base-intelligent-pointer destructors.
// Native pointers at+4 and+16, each base vptr+0/PData+4. Middle dword+12 is untouched.
// A01C10 decrements PData references+4; deletes data only if refs0 AND object+0 null.
// Member nesting preserves reverse destruction order without synthetic virtual slots.
struct CBaseObjectPointer {void* Object;unsigned References;};
struct CBaseIntelligentPointer {
 virtual ~CBaseIntelligentPointer(); //00a01c10
 CBaseObjectPointer* PData;
};
struct CPairedIntelligentPointerPayload {CBaseIntelligentPointer First;unsigned UnrelatedValue;CBaseIntelligentPointer Second;};
struct CPairedIntelligentPointerRecord {unsigned UnrelatedPrefix;CPairedIntelligentPointerPayload Pointers;};
void DeletePairedRecord(CPairedIntelligentPointerRecord* p){delete p;}



#include <stdio.h>
#include <stddef.h>
#ifdef _MSC_VER
typedef char PointerSize[(sizeof(CBaseIntelligentPointer)==8)?1:-1];
typedef char FirstOffset[(offsetof(CPairedIntelligentPointerRecord,Pointers)==4)?1:-1];
typedef char SecondOffset[(offsetof(CPairedIntelligentPointerPayload,Second)==12)?1:-1];
#endif
static int bad,freed,dtors;static CBaseIntelligentPointer* expected[2];
CBaseIntelligentPointer::~CBaseIntelligentPointer(){if(dtors<2&&this!=expected[dtors])bad=1;++dtors;if(PData){--PData->References;if(PData->References==0&&PData->Object==0){++freed;delete PData;}PData=0;}}
static void Expect(CPairedIntelligentPointerRecord&x){expected[0]=&x.Pointers.Second;expected[1]=&x.Pointers.First;dtors=0;}
int main(){
 {CPairedIntelligentPointerRecord x;x.Pointers.First.PData=0;x.Pointers.Second.PData=0;Expect(x);}if(dtors!=2||freed)bad=1;
 CBaseObjectPointer* shared=new CBaseObjectPointer;shared->Object=0;shared->References=2;
 CPairedIntelligentPointerRecord* x=new CPairedIntelligentPointerRecord;x->Pointers.First.PData=shared;x->Pointers.Second.PData=shared;Expect(*x);DeletePairedRecord(x);if(dtors!=2||freed!=1)bad=1;
 int live;CBaseObjectPointer* kept=new CBaseObjectPointer;kept->Object=&live;kept->References=1;
 {CPairedIntelligentPointerRecord y;y.Pointers.First.PData=kept;y.Pointers.Second.PData=0;Expect(y);}if(kept->References!=0||kept->Object!=&live||freed!=1)bad=1;delete kept;
 CBaseObjectPointer* a=new CBaseObjectPointer;CBaseObjectPointer*b=new CBaseObjectPointer;a->Object=b->Object=0;a->References=b->References=1;
 x=new CPairedIntelligentPointerRecord;x->Pointers.First.PData=a;x->Pointers.Second.PData=b;Expect(*x);DeletePairedRecord(x);if(dtors!=2||freed!=3)bad=1;
 DeletePairedRecord(0);printf("0078e1f0_TEST %s\n",bad?"FAIL":"PASS");return bad;
}