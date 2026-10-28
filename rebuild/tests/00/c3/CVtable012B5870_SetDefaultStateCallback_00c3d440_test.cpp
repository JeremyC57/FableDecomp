#include <stdio.h>
// CVtable_012B5870::SetDefaultStateCallback(TStateCallback first,void* second) -- Replace the stored pair.
// Leaf: ?SetDefaultStateCallback@CVtable_012B5870@@QAEXP6GXPAVCAudioObject@@KPAX@Z1@Z
// Identity: Own global wrapper00c33500 receiver0143bb58; getter00c3d2d0 copies both words into new audio object callbacks at00c42e15. Descriptive libpk owner/API spellings, not original PDB identifiers.
#ifndef _MSC_VER
#define __stdcall
#endif
class CAudioObject;
typedef void (__stdcall* TStateCallback)(CAudioObject*,unsigned long,void*);
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868();};
class CPooledNode:public CUnprovenLibraryObject_012B5868{public:virtual ~CPooledNode(){}CPooledNode* Prev;CPooledNode* Next;};
// Model amendment: Actual table012b5870 first entry00c3cbb0 is deleting destructor invoking00c3cb30; plain destructor stores012b5870 then destroys four20-byte members and finally stores original base012b5868. Original virtual-object base and virtual destructor slot0 are modeled; inactive opaque field regions remain explicit projections.
// Full hierarchy: Full node destructor00c4cf40..70 unlinks Prev/Next and finally resets common base table012b5868 at00c4cf69. Common base deleting destructor00c3c840 only installs012b5868 and conditionally calls library allocator-release00c3c370; no further base transition. Both full original inheritance chains are now represented.
// Layout: own CAudioObject constructor00c42d60 calls CPooledNode ctor00c4cf20, base12. Callback220/context224 proven by own SetState00c43570; last word-pair228/232 role unproven. Global default callback128/context132 copied by00c42e15 into these object fields. No shared headers.
class CAudioObject:public CPooledNode{public:unsigned char Unmodeled0C[208];TStateCallback StateCallback;void* StateContext;void* UnprovenE4;void* UnprovenE8;void SetStateCallback(TStateCallback,void*);void SetUnprovenPairE4(void*,void*);};
struct CVtable_012B5870:public CUnprovenLibraryObject_012B5868{virtual ~CVtable_012B5870();unsigned char Unmodeled04[124];TStateCallback DefaultStateCallback;void* DefaultStateContext;void SetDefaultStateCallback(TStateCallback,void*);};
void CVtable_012B5870::SetDefaultStateCallback(TStateCallback first,void* second){DefaultStateCallback=first;DefaultStateContext=second;}
static int calls;static CAudioObject* seenObject;static unsigned long seenFlags;static void* seenContext;static void __stdcall Callback(CAudioObject* p,unsigned long flags,void* context){++calls;seenObject=p;seenFlags=flags;seenContext=context;}
int main(){CVtable_012B5870 object;int token;TStateCallback first=&Callback;object.SetDefaultStateCallback(first,&token);bool ok=object.DefaultStateCallback==first&&object.DefaultStateContext==&token;CAudioObject audio;object.DefaultStateCallback(&audio,0x1234,object.DefaultStateContext);ok=ok&&calls==1&&seenObject==&audio&&seenFlags==0x1234&&seenContext==&token;object.SetDefaultStateCallback(0,0);ok=ok&&!object.DefaultStateCallback&&!object.DefaultStateContext;puts(ok?"00c3d440_TEST PASS":"00c3d440_TEST FAIL");return ok?0:1;}

CUnprovenLibraryObject_012B5868::~CUnprovenLibraryObject_012B5868(){}CVtable_012B5870::~CVtable_012B5870(){}