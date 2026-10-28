#include <stdio.h>
// CAudioObject::SetUnprovenPairE4(void* first,void* second) -- Replace the stored pair.
// Leaf: ?SetUnprovenPairE4@CAudioObject@@QAEXPAX0@Z
// Identity: Own wrapper00c35bf0 forwards two words to228/232. Role and original parameter types unresolved; represented as opaque pointers. Descriptive libpk owner/API spellings, not original PDB identifiers.
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
void CAudioObject::SetUnprovenPairE4(void* first,void* second){UnprovenE4=first;UnprovenE8=second;}
static int calls;static CAudioObject* seenObject;static unsigned long seenFlags;static void* seenContext;static void __stdcall Callback(CAudioObject* p,unsigned long flags,void* context){++calls;seenObject=p;seenFlags=flags;seenContext=context;}
int main(){CAudioObject object;int token;void* first=&token;object.SetUnprovenPairE4(first,&token);bool ok=object.UnprovenE4==first&&object.UnprovenE8==&token;object.SetUnprovenPairE4(0,0);ok=ok&&!object.UnprovenE4&&!object.UnprovenE8;puts(ok?"00c43350_TEST PASS":"00c43350_TEST FAIL");return ok?0:1;}

CUnprovenLibraryObject_012B5868::~CUnprovenLibraryObject_012B5868(){}CVtable_012B5870::~CVtable_012B5870(){}