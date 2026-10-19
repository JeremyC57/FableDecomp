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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
void* operator new(std::size_t n) throw(std::bad_alloc){void* p=std::malloc(n);if(!p)throw std::bad_alloc();std::memset(p,0xa5,n);return p;}
void operator delete(void* p) throw(){std::free(p);}
#include <cstddef>
static int calls,releases,failures,dtors,baseConstructors;static bool ok=true;static CEngineVertexBufferPool* expectedPool;static CVertexBufferWin32Native* allocated;
CBase::CBase(){++baseConstructors;} CBase::~CBase(){} CVertexBufferWin32Native::~CVertexBufferWin32Native(){++dtors;}
bool CVertexBufferWin32Native::Initialise(int type,long count,long stride,unsigned long usage,int pool){
 ok=ok&&type==7&&count==23&&stride==19&&usage==91&&pool==4;
 if(calls==0){allocated=this;ok=ok&&!PVertexBuffer&&Length==0&&Type==0&&Stride==0&&CompressionParams.PosScaleX==1&&CompressionParams.PosScaleY==1&&CompressionParams.PosScaleZ==1&&CompressionParams.PosScaleW==1&&CompressionParams.PosBiasX==0&&CompressionParams.PosBiasY==0&&CompressionParams.PosBiasZ==0&&CompressionParams.PosBiasW==0&&!static_cast<CEngineDynamic3DResource*>(this)->PoolNext;}
 for(unsigned int k=0;k<sizeof(Reserved);++k)ok=ok&&Reserved[k]==0xa5;
 unsigned char* keyBytes=reinterpret_cast<unsigned char*>(static_cast<CEngineDynamic3DResource*>(this)->Key);for(unsigned int k=0;k<sizeof(static_cast<CEngineDynamic3DResource*>(this)->Key);++k)ok=ok&&keyBytes[k]==0xa5;
 if(calls!=0)ok=ok&&this==allocated;
 ++calls;return calls>failures;
}
void CEngineVertexBufferPool::ReleaseResources(){ok=ok&&this==expectedPool&&calls==releases+1;++releases;}
int main(){CEngineVertexBufferPool p;expectedPool=&p;unsigned long key[3]={10,20,30};
#ifdef _MSC_VER
 ok=ok&&sizeof(CVertexBufferWin32Native)==0x34&&sizeof(CEngineDynamic3DResource)==0x44;
#endif
 for(failures=0;failures<=2;++failures){calls=releases=0;allocated=0;CEngineDynamic3DResource* r=p.BuildNewBuffer(key,7,23,19,91,4);
 ok=ok&&calls==(failures==0?1:2)&&releases==failures&&key[0]==10&&key[1]==20&&key[2]==30;
 ok=ok&&(failures==2?r==0:r==allocated);ok=ok&&dtors==failures&&baseConstructors==failures+1;CBase* base=allocated;delete base;}
 ok=ok&&dtors==3;std::printf("00bf3380_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;}