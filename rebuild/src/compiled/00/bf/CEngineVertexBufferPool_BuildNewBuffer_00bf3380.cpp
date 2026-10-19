// CEngineVertexBufferPool::BuildNewBuffer -- allocate dynamic vertex buffer and retry Initialise twice after pool releases.
// Retail00bf3380 183bytes. Shared donor CVertexBufferWin32 is opaque34; local semantic model exposes base fields.
// Donor CVertexBufferWin32 is opaque34 and does not express the polymorphic CBase inheritance and constructor; Native view exposes PDB base fields.
// CBase ctor0099a2f0, vertexInitialise00a63150, poolRelease00bf32b0; dynamic-resource vtable012a81a0 has deleting dtor00bf3490.
#ifdef _MSC_VER
#include "engine/CVertexBufferWin32.h"
#endif
#ifndef _MSC_VER
#define __declspec(x)
#endif
struct CBase { CBase(); virtual ~CBase(); };
struct CVertexCompressionParams {
 float PosScaleX,PosScaleY,PosScaleZ,PosScaleW,PosBiasX,PosBiasY,PosBiasZ,PosBiasW;
 CVertexCompressionParams():PosScaleX(1),PosScaleY(1),PosScaleZ(1),PosScaleW(1),PosBiasX(0),PosBiasY(0),PosBiasZ(0),PosBiasW(0){}
};
struct __declspec(novtable) CVertexBufferWin32Native : CBase {
 void* PVertexBuffer;unsigned long Length;int Type;unsigned char Stride;unsigned char Reserved[3];CVertexCompressionParams CompressionParams;
 CVertexBufferWin32Native():Length(0),Type(0){PVertexBuffer=0;Stride=0;}
 virtual ~CVertexBufferWin32Native(); //00b564c0
 bool Initialise(int,long,long,unsigned long,int); //00a63150
};
struct CEngineDynamic3DResource : CVertexBufferWin32Native {unsigned long Key[3];CEngineDynamic3DResource* PoolNext;CEngineDynamic3DResource():PoolNext(0){} };
struct CEngineVertexBufferPool {void ReleaseResources();CEngineDynamic3DResource* BuildNewBuffer(unsigned long*,int,long,long,unsigned long,int);};
CEngineDynamic3DResource* CEngineVertexBufferPool::BuildNewBuffer(unsigned long* key,int type,long count,long stride,unsigned long usage,int pool){
 CEngineDynamic3DResource* buffer=new CEngineDynamic3DResource;
 for(int n=0;n<2;++n){if(buffer->Initialise(type,count,stride,usage,pool))return buffer;ReleaseResources();}
 return 0;
}