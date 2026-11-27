// 00bfd7c0 CEngineWaterBackgroundSubPatch::Load
// Leaf: ?Load@CEngineWaterBackgroundSubPatch@@QAE_NAAVCDataInputStream@@GGJJMM@Z
// Flags: /Gr
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_ACCESSORS
#define FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22
#define FABLE_RETAIL_PASS22_WATER_PATCH_LOAD
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_WATER_SUBPATCH_LOAD_PASS29
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE
#define FABLE_RETAIL_VECTOR_BYTE_RESIZE_VALUE_OWNER_PASS29
#define FABLE_RETAIL_VECTOR_BYTE_RESIZE_ONE_OWNER_PASS29
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
#define FABLE_RETAIL_WATER_BYTE_RESIZE_SPELLINGS_PASS29
#define FABLE_RETAIL_STLP_COPY
#define FABLE_RETAIL_WATER_BYTE_ERASE_OWNER_PASS29
#include "engine/CDataInputStream.h"
#include "engine/CEngineWaterBackgroundSubPatch.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CIndexBuffer.h"
#include "engine/EPrimitiveType.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/CRangeCompressor.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }
using namespace NEngineInternal;

bool CEngineWaterBackgroundSubPatch::Load(CDataInputStream& stream, unsigned short width, unsigned short height, long world_x, long world_y, float min_z, float max_z)
{
    MapX = world_x;
    MapY = world_y;
    Width = width;
    Height = height;
    MinZ = min_z;
    MaxZ = max_z;
    VertexCount = stream.ReadUWORD();
    PolyCount = stream.ReadUWORD();
    if (VertexBuffer) GPLandscapeRenderer->GetMemoryManager()->DeleteVertexBuffer(VertexBuffer);
    WaterType = static_cast<EWaterType>(stream.ReadSLONG());
    long vertex_stride = stream.ReadSLONG();
    VertexBuffer = GPLandscapeRenderer->GetMemoryManager()->AllocateVertexBuffer(this, VERTEX_NULL, VertexCount, vertex_stride, GPResourceManager->GetVertexBufferUsage());
    if (!VertexBuffer) return false;
    unsigned char* vbdata = VertexBuffer->Lock(0,0,0);
    long comp_size = stream.ReadSLONG();
    CArray<unsigned char> comp_buffer(comp_size);
    stream.Read(comp_buffer.GetAsCArray(),comp_size);
    CRangeCompressor compressor;
    compressor.Decompress(comp_buffer.GetAsCArray(),VertexCount,vertex_stride,vbdata);
    VertexBuffer->Unlock();
    if (IndexBuffer) GPLandscapeRenderer->GetMemoryManager()->DeleteIndexBuffer(IndexBuffer);
    const long index_count = PolyCount * 3;
    IndexBuffer = GPLandscapeRenderer->GetMemoryManager()->AllocateIndexBuffer(this,index_count*2,GPResourceManager->GetIndexBufferUsage(),IB_FORMAT_16,PT_TRIANGLE_LIST);
    if (!IndexBuffer) return false;
    if (index_count) {
        unsigned char* ibdata = IndexBuffer->Lock(0,0,0);
        comp_size = stream.ReadSLONG();
        comp_buffer.resize(comp_size);
        stream.Read(comp_buffer.GetAsCArray(),comp_size);
        compressor.Decompress(comp_buffer.GetAsCArray(),index_count,2,ibdata);
        IndexBuffer->Unlock();
    }
    OcclusionID = 0;
    return true;
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
template<class T> union Store { double align; unsigned char data[sizeof(T)]; T* get(){return reinterpret_cast<T*>(data);} };
namespace NEngineInternal { CEngineLandscapeRenderer* GPLandscapeRenderer; CEngineResourceManager* GPResourceManager; }
static CVertexBufferWin32* vb; static CIndexBuffer* ib;
static bool failV,failI;static long vc,ic,vd,id,vlock,ilock,unlocks,dec,overflows;
static unsigned char vert[32],inds[32];static long nverts,ninds,vbytes,ibytes;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* dst,long n) {
    assert(n>0);memcpy(dst,CurrentSourcePtr,n);CurrentSourcePtr+=n;StreamPos+=n;++overflows;
}
unsigned long CEngineResourceManager::GetVertexBufferUsage(){return 17;}
unsigned long CEngineResourceManager::GetIndexBufferUsage(){return 19;}
void CEngineLandscapeMemoryManager::DeleteVertexBuffer(CVertexBufferWin32* p){assert(p==vb);++vd;}
void CEngineLandscapeMemoryManager::DeleteIndexBuffer(CIndexBuffer* p){assert(p==ib);++id;}
static CMovableResource* expectedOwner;
CVertexBufferWin32* CEngineLandscapeMemoryManager::AllocateVertexBuffer(CMovableResource* p,EVertexType v,long count,long stride,unsigned long use) {
    assert(p==expectedOwner && v==VERTEX_NULL && count==nverts && stride==3 && use==17);++vc;return failV?0:vb;
}
CIndexBuffer* CEngineLandscapeMemoryManager::AllocateIndexBuffer(CMovableResource* p,long n,unsigned long use,EIndexBufferFormat fmt,EPrimitiveType prim) {
    assert(p==expectedOwner && n==2*ninds && use==19 && fmt==IB_FORMAT_16 && prim==PT_TRIANGLE_LIST);++ic;return failI?0:ib;
}
unsigned char* CVertexBufferBase::Lock(unsigned long a,unsigned long b,unsigned long c){assert(!a&&!b&&!c);++vlock;return vert;}
void CVertexBufferBase::Unlock(){++unlocks;}
unsigned char* CIndexBuffer::Lock(unsigned long a,unsigned long b,unsigned long c){assert(!a&&!b&&!c);++ilock;return inds;}
void CIndexBuffer::Unlock(){++unlocks;}
void CRangeCompressor::Decompress(const unsigned char* p,long n,long stride,void* dest){
    bool vertex=dest==vert;assert(vertex||dest==inds);assert(n==(vertex?nverts:ninds)&&stride==(vertex?3:2));++dec;
    for(long i=0;i<(vertex?vbytes:ibytes);++i)assert(p[i]==(vertex?7+i:33+i));
}
template<> CArray<unsigned char>::CArray(unsigned int n):std::vector<unsigned char>() {
    _M_start=(unsigned char*)malloc(n?n:1);_M_finish=_M_end_of_storage._M_data=_M_start+n;
}
namespace std {
template<> void vector<unsigned char>::_M_fill_insert(iterator pos,size_type n,const unsigned char& x){
    assert(pos==_M_finish);if(!n)return;
    size_type old=size();_M_start=(unsigned char*)realloc(_M_start,old+n);memset(_M_start+old,x,n);
    _M_finish=_M_end_of_storage._M_data=_M_start+old+n;
}
}
template<class T> static void add(unsigned char*& p,T val){memcpy(p,&val,sizeof(T));p+=sizeof(T);}
int main(){
    Store<CEngineLandscapeRenderer> draw;Store<CEngineLandscapeMemoryManager> mem;
    Store<CEngineResourceManager> res;Store<CVertexBufferWin32> vv;Store<CIndexBuffer> ii;
    GPLandscapeRenderer=draw.get();GPResourceManager=res.get();GPLandscapeRenderer->MemoryManager=mem.get();
    vb=vv.get();ib=ii.get();int cases=0;
    for(int mask=0;mask<128;++mask) for(int shape=0;shape<3;++shape){
        unsigned char bytes[80],*end=bytes;Store<CEngineWaterBackgroundSubPatch> obj;
        Store<CDataInputStream> stream;memset(obj.data,0,sizeof(obj.data));memset(stream.data,0,sizeof(stream.data));
        CEngineWaterBackgroundSubPatch* o=obj.get();expectedOwner=o; o->VertexBuffer=(mask&1)?vb:0;o->IndexBuffer=(mask&2)?ib:0;o->OcclusionID=99;
        failV=(mask&4)!=0;failI=(mask&8)!=0;nverts=(mask&16)?3:0;ninds=(mask&32)?6:0;
        vbytes=shape==0?0:4;ibytes=shape==0?0:shape==1?2:6;
        add(end,(unsigned short)nverts);add(end,(unsigned short)(ninds/3));add(end,3L);add(end,3L);add(end,vbytes);
        for(int a=0;a<vbytes;++a)*end++=(unsigned char)(7+a);
        if(ninds){add(end,ibytes);for(int b=0;b<ibytes;++b)*end++=(unsigned char)(33+b);}
        unsigned char* base=bytes;memcpy(stream.data+12,&base,4);long avail=(mask&64)?0:end-bytes;memcpy(stream.data+20,&avail,4);
        vc=ic=vd=id=vlock=ilock=unlocks=dec=overflows=0;
        bool ok=o->Load(*stream.get(),8,14,-5,6,-0.5f,8.0f);
        assert(ok==(!failV&&!failI)&&o->MapX==-5&&o->MapY==6&&o->MinZ==-0.5f&&o->MaxZ==8&&o->Width==8&&o->Height==14);
        assert(o->VertexCount==nverts&&o->PolyCount==ninds/3&&int(o->WaterType)==3);
        assert(o->OcclusionID==(ok?0:99)&&vc==1&&vd==((mask&1)!=0));
        assert(vlock==!failV&&ic==!failV&&id==(!failV&&((mask&2)!=0)));
        assert(ilock==(ok&&ninds>0)&&unlocks==vlock+ilock&&dec==unlocks);
        assert((overflows>0)==((mask&64)!=0));++cases;
    }
    assert(cases==384);puts("00bfd7c0_TEST PASS");
}