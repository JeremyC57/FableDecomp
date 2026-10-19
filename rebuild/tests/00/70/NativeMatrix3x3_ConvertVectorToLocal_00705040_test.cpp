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
// NativeMatrix3x3::TransformVectorPair -- rotate two adjacent vectors with no translation
// Retail00705040,270bytes. Owner identity unresolved: propagated cloth ConvertVectorToLocal label is unsupported;
// actual cloth conversion is00B80AD0. ECX is a bounded36-byte3x3 matrix, argument is24-byte vector pair; RET4 establishes thiscall ABI.
// No actual C3DVector/matrix definition/header found in rebuild/include; native vectors are three floats,12bytes.
// Both input vectors are copied before output stores; the two matrix calculations precede pair assignment.
// The pair parameter is projected as a pointer to two vector elements, permitting defined overlap with matrix columns;
// pointer/reference identity is not recoverable from this ABI. Both are one stack address.
// Leaf: TransformVectorPair
#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif

struct NativeVector3 {float X,Y,Z;NativeVector3(){}NativeVector3(const NativeVector3& v):X(v.X),Y(v.Y),Z(v.Z){}};
struct NativeVectorPair {NativeVector3 First,Second;};
struct NativeMatrix3x3 {NativeVector3 Columns[3];
 __forceinline void Transform(NativeVector3& v)const{float x=v.X,y=v.Y,z=v.Z;v.X=Columns[0].X*x+Columns[1].X*y+Columns[2].X*z;v.Y=Columns[0].Y*x+Columns[1].Y*y+Columns[2].Y*z;v.Z=Columns[0].Z*x+Columns[1].Z*y+Columns[2].Z*z;}
 void TransformVectorPair(NativeVector3*)const;
};
FABLE_STATIC_ASSERT(sizeof(NativeVector3)==12);
FABLE_STATIC_ASSERT(sizeof(NativeVectorPair)==24);
FABLE_STATIC_ASSERT(sizeof(NativeMatrix3x3)==36);
// Native driver links independently verified /O2 production object. Host executes this identical body.
#ifndef _MSC_VER
void NativeMatrix3x3::TransformVectorPair(NativeVector3* pair)const{NativeVector3 first=pair[0];NativeVector3 second=pair[1];Transform(first);Transform(second);pair[0]=first;pair[1]=second;}
#endif



#include <assert.h>
#include <math.h>
static void Set(NativeVector3& v,float x,float y,float z){v.X=x;v.Y=y;v.Z=z;}
static void Reference(const NativeMatrix3x3& m,const NativeVector3& v,NativeVector3& out){out.X=float(double(v.X)*m.Columns[0].X+double(v.Y)*m.Columns[1].X+double(v.Z)*m.Columns[2].X);out.Y=float(double(v.X)*m.Columns[0].Y+double(v.Y)*m.Columns[1].Y+double(v.Z)*m.Columns[2].Y);out.Z=float(double(v.X)*m.Columns[0].Z+double(v.Y)*m.Columns[1].Z+double(v.Z)*m.Columns[2].Z);}
static bool Same(const NativeVector3& a,const NativeVector3& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
int main(){
 for(int mode=0;mode<6;++mode){NativeMatrix3x3 matrix;Set(matrix.Columns[0],2,-1,3);Set(matrix.Columns[1],4,5,-2);Set(matrix.Columns[2],-3,2,6);if(mode==3){Set(matrix.Columns[0],1,0,0);Set(matrix.Columns[1],0,1,0);Set(matrix.Columns[2],0,0,1);}if(mode==4){Set(matrix.Columns[0],0,1,0);Set(matrix.Columns[1],-1,0,0);Set(matrix.Columns[2],0,0,1);}if(mode==5){Set(matrix.Columns[0],0,0,0);Set(matrix.Columns[1],0,0,0);Set(matrix.Columns[2],0,0,0);}
 NativeVector3 storage[4];Set(storage[0],999,998,997);Set(storage[1],1,2,3);Set(storage[2],-4,5,-6);Set(storage[3],888,887,886);NativeVector3* pair=mode==1?matrix.Columns:mode==2?matrix.Columns+1:storage+1;
 NativeMatrix3x3 before=matrix;NativeVector3 expected[2];Reference(before,pair[0],expected[0]);Reference(before,pair[1],expected[1]);matrix.TransformVectorPair(pair);assert(Same(pair[0],expected[0])&&Same(pair[1],expected[1]));if(mode==1)assert(Same(matrix.Columns[2],before.Columns[2]));else if(mode==2)assert(Same(matrix.Columns[0],before.Columns[0]));else for(int j=0;j<3;++j)assert(Same(matrix.Columns[j],before.Columns[j]));assert(storage[0].X==999&&storage[3].Z==886);
 }
 // Native x87 schedules X as (Z+Y)+X, Y/Z as (X+Z)+Y. Host SSE evaluates the source's (X+Y)+Z.
 // This cancellation case explicitly records that compiler-dependent FP scheduling while running the same source.
 NativeMatrix3x3 fp;Set(fp.Columns[0],1e20f,1e20f,1e20f);Set(fp.Columns[1],-1e20f,3,3);Set(fp.Columns[2],3,-1e20f,-1e20f);NativeVector3 pair[2];Set(pair[0],1,1,1);Set(pair[1],1,1,1);fp.TransformVectorPair(pair);
#ifdef _MSC_VER
 assert(pair[0].X==0&&pair[0].Y==3&&pair[0].Z==3&&Same(pair[0],pair[1]));
#else
 assert(pair[0].X==3&&pair[0].Y==0&&pair[0].Z==0&&Same(pair[0],pair[1]));
#endif
 unsigned bits=0x7fc01234;memcpy(&fp.Columns[0].X,&bits,4);Set(pair[0],1,2,3);Set(pair[1],4,5,6);fp.TransformVectorPair(pair);assert(pair[0].X!=pair[0].X&&pair[1].X!=pair[1].X);
 puts("00705040_TEST PASS");return 0;
}