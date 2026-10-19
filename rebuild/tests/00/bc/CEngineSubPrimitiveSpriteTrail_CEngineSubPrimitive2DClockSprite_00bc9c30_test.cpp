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
// CEngineSubPrimitiveSpriteTrail::CEngineSubPrimitiveSpriteTrail(parent) -- stores parent and empty chain link.
// Retail00bc9c30 corrected34bytes. VT012a69d4 slot0=00bca060; base ctor0099a2f0
// writes CBaseClassNonCopyable VT0129a7c4 only. PDB CEngineSubPrimitiveBase size24,
// RenderFlags+4 Alpha+8 Parent+12 NextSubPrimitive+16 Manager+20; no engine header exists.
struct CEngineInternalPrimitiveBase; struct CEnginePrimitiveManager;
struct CBaseClassNonCopyable {
 CBaseClassNonCopyable(); //0099a2f0
 virtual ~CBaseClassNonCopyable(); //0099a300
};
struct CEngineSubPrimitiveBase : CBaseClassNonCopyable {
 // Native pooled primitive deallocator00beb550 is a no-op (single RET).
 static void operator delete(void*) {}
 unsigned int RenderFlags; unsigned char Alpha;
 CEngineInternalPrimitiveBase* Parent;
 CEngineSubPrimitiveBase* NextSubPrimitive;
 CEnginePrimitiveManager* Manager;
 CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* p):Parent(p),NextSubPrimitive(0){}
};
struct CEngineSubPrimitiveSpriteTrail : CEngineSubPrimitiveBase {
 CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveBase* p);
};
CEngineSubPrimitiveSpriteTrail::CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveBase* p):CEngineSubPrimitiveBase(p){}



#include <stdio.h>
#include <stddef.h>
#include <new>
#ifdef _MSC_VER
typedef char TrailSize[(sizeof(CEngineSubPrimitiveSpriteTrail)==24)?1:-1];
typedef char TrailParentOffset[(offsetof(CEngineSubPrimitiveSpriteTrail,Parent)==12)?1:-1];
#endif
static int ctorCalls,dtorCalls,bad;
CBaseClassNonCopyable::CBaseClassNonCopyable(){++ctorCalls;}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){++dtorCalls;}
struct CEngineInternalPrimitiveBase{int tag;};
int main(){CEngineInternalPrimitiveBase parent;
 {CEngineSubPrimitiveSpriteTrail x(&parent);if(x.Parent!=&parent||x.NextSubPrimitive!=0||ctorCalls!=1)bad=1;}
 {CEngineSubPrimitiveSpriteTrail x(0);if(x.Parent!=0||x.NextSubPrimitive!=0||ctorCalls!=2)bad=1;}
 if(dtorCalls!=2)bad=1;
 union Storage {long double alignment;unsigned char bytes[sizeof(CEngineSubPrimitiveSpriteTrail)];} storage;
 CEngineSubPrimitiveSpriteTrail* p=::new(storage.bytes) CEngineSubPrimitiveSpriteTrail(&parent);
 CBaseClassNonCopyable* b=p;delete b; // native arena delete is intentionally a no-op
 if(dtorCalls!=3||ctorCalls!=3)bad=1;
 printf("00bc9c30_TEST %s\n",bad?"FAIL":"PASS");return bad;
}