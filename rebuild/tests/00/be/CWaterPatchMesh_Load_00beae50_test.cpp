// Complete unmodified production definitions below; native external boundaries are recording doubles.
#include <stdio.h>
#include <string.h>
#include <vector>
#include <limits.h>
enum EWaterType { WATER_TYPE_NULL,WATER_TYPE_LAKE,WATER_TYPE_RIVER,WATER_TYPE_SEA,WATER_TYPE_REFLECTIVE_SEA,WATER_TYPE_NON_REFLECTIVE_SEA,WATER_TYPE_OLD,WATER_TYPE_DUMMY_SHORE_POINT,WATER_TYPE_ICE,WATER_TYPE_COUNT };
enum EVertexType {VERTEX_NULL};
enum { WATER_PATCH_VERTEX_COUNT=17*17,WATER_PATCH_PACKED_VERTEX_SIZE=0x42 };
struct CTVertexWaterForeground {unsigned short X,Y;float Z;unsigned short SwellData0,SwellData1;float Depth,ShoreUCoord,ShoreLookUpArray[12];};
static bool bad,allocationFail;static int phase,arraysAlive,arrayCtors,arrayDtors,reads,usageCalls,allocationCalls,lockCalls,unlockCalls,decompressCalls,chunkSize,chunkCrossings;static unsigned int usageValue;static const unsigned char* compressedPointer;static unsigned char* uncompressedPointer;
static unsigned int Rng(unsigned int&x){x=x*1664525u+1013904223u;return x;}
static unsigned int FiniteBits(unsigned int&x){unsigned int b=Rng(x);return (b&0x7f800000u)==0x7f800000u?b&0xff7fffffu:b;}
#ifndef _MSC_VER
#define __int64 long long
#endif
class CDataInputStream {
 const unsigned char* data;unsigned long length;
public:
 unsigned long position,StreamPos,StreamSize,SourceChunkStreamPos;unsigned char* CurrentSourcePtr;long SourceChunkBytesRemaining;
 CDataInputStream(const void*p,unsigned long n):data((const unsigned char*)p),length(n),position(0),StreamPos(0),StreamSize(n),SourceChunkStreamPos(0),CurrentSourcePtr((unsigned char*)p),SourceChunkBytesRemaining(chunkSize&&n>(unsigned long)chunkSize?chunkSize:long(n)){}
 virtual~CDataInputStream(){}
 void ReadWithSrcChunkOverflow(unsigned char* dst,long n){
  if(StreamPos+(unsigned long)n>length){bad=true;return;}
  long done=0;
  while(done<n){
   if(!SourceChunkBytesRemaining){SourceChunkStreamPos=StreamPos;CurrentSourcePtr=(unsigned char*)data+StreamPos;unsigned long available=length-StreamPos;SourceChunkBytesRemaining=chunkSize&&available>(unsigned long)chunkSize?chunkSize:long(available);}
   long take=n-done;if(take>SourceChunkBytesRemaining){take=SourceChunkBytesRemaining;++chunkCrossings;}
   memcpy(dst+done,CurrentSourcePtr,take);CurrentSourcePtr+=take;SourceChunkBytesRemaining-=take;StreamPos+=take;done+=take;
  }
 }
 void Read(void* dst,long no_bytes){
  ++reads;
  {

        if(no_bytes>0) {
            __int64 overflow_check=(__int64)(StreamPos+no_bytes);
            if(overflow_check<0 || overflow_check>0x7fffffff)return;
            if(no_bytes<=SourceChunkBytesRemaining) {
                memcpy(dst,CurrentSourcePtr,no_bytes);
                CurrentSourcePtr+=no_bytes;
                SourceChunkBytesRemaining-=no_bytes;
                StreamPos+=no_bytes;
            } else ReadWithSrcChunkOverflow((unsigned char*)dst,no_bytes);
        }
    
  }
  position=StreamPos;
 }
 template<class T>T ReadType(){T val;Read(&val,sizeof(T));return val;}
 float ReadFloat(){return ReadType<float>();}
 long ReadSLONG(){int v;Read(&v,4);return(long)v;}
};
class CMemoryDataInputStream:public CDataInputStream {public:CMemoryDataInputStream(const void*p,unsigned long n):CDataInputStream(p,n){if(phase!=4||p!=uncompressedPointer||n!=19074)bad=true;phase=5;}~CMemoryDataInputStream(){if(phase!=7)bad=true;phase=8;}};
template<class T>class CArray {std::vector<T> values;int id;public:explicit CArray(unsigned int n):values(n),id(++arrayCtors){++arraysAlive;if(phase!=2||id>2||(id==1&&n!=19074))bad=true;}~CArray(){if(phase!=8&&phase!=9)bad=true;if(id!=(arrayDtors?1:2))bad=true;++arrayDtors;--arraysAlive;phase=9;}unsigned long size()const{return (unsigned long)values.size();}T*GetAsCArray(){return values.empty()?(T*)this:&values[0];}};
static CTVertexWaterForeground verts[WATER_PATCH_VERTEX_COUNT];
class CVertexBufferWin32 {public:void*Lock(long a,long b,long c){++lockCalls;if(phase!=5||a||b||c)bad=true;phase=6;return verts;}void Unlock(){++unlockCalls;if(phase!=6)bad=true;phase=7;}};static CVertexBufferWin32 buffer,oldBuffer;
class CWaterPatchMesh;static CWaterPatchMesh* expectedOwner;
class CEngineResourceManager {public:unsigned long GetVertexBufferUsage(){++usageCalls;if(phase!=1)bad=true;return usageValue;}};static CEngineResourceManager resource;
class CEngineLandscapeMemoryManager {public:CVertexBufferWin32*AllocateVertexBuffer(void*owner,EVertexType type,long count,long stride,unsigned long usage){++allocationCalls;if(phase!=1||owner!=expectedOwner||type!=VERTEX_NULL||count!=289||stride!=68||usage!=usageValue||usageCalls!=1)bad=true;phase=2;return allocationFail?0:&buffer;}};static CEngineLandscapeMemoryManager memory;
class CEngineLandscapeRenderer {public:CEngineLandscapeMemoryManager*GetMemoryManager(){if(phase)bad=true;phase=1;return &memory;}};static CEngineLandscapeRenderer landscape;
namespace NEngineInternal {static CEngineLandscapeRenderer*GPLandscapeRenderer=&landscape;static CEngineResourceManager*GPResourceManager=&resource;}
class CRangeCompressor {public:void Decompress(const unsigned char* p,long width,long height,void*out){++decompressCalls;if(phase!=2||width!=289||height!=66||arraysAlive!=2)bad=true;phase=4;compressedPointer=p;uncompressedPointer=(unsigned char*)out;memcpy(out,p,19074);}};
class CWaterPatchMesh {public:float WaterBodySpan;EWaterType WaterType;CVertexBufferWin32*VertexBuffer;CWaterPatchMesh():WaterBodySpan(-1),WaterType(WATER_TYPE_COUNT),VertexBuffer(&oldBuffer){}void SetTargetVertexDepth(CTVertexWaterForeground&,short);bool Load(CDataInputStream&);};

inline void CWaterPatchMesh::SetTargetVertexDepth(CTVertexWaterForeground& vertex, short depth)
{
    float value = (float)depth * (2.0f / 32767.0f);
    if (value < 0.0f)
        value = 0.0f;
    if (value > 2.0f)
        value = 2.0f;
    vertex.Depth = value;
}

bool CWaterPatchMesh::Load(CDataInputStream& stream)
{
    WaterBodySpan = stream.ReadFloat();
    long type = stream.ReadSLONG();
    switch (type) {
    case WATER_TYPE_ICE: WaterType = WATER_TYPE_ICE; break;
    case WATER_TYPE_RIVER: WaterType = WATER_TYPE_RIVER; break;
    case WATER_TYPE_REFLECTIVE_SEA: WaterType = WATER_TYPE_REFLECTIVE_SEA; break;
    case WATER_TYPE_NON_REFLECTIVE_SEA: WaterType = WATER_TYPE_NON_REFLECTIVE_SEA; break;
    case WATER_TYPE_SEA: WaterType = WATER_TYPE_SEA; break;
    case WATER_TYPE_OLD: WaterType = WATER_TYPE_OLD; break;
    case WATER_TYPE_NULL:
    case WATER_TYPE_LAKE:
    case WATER_TYPE_DUMMY_SHORE_POINT: WaterType = WATER_TYPE_LAKE; break;
    default: WaterType = WATER_TYPE_LAKE; break;
    }

    CEngineLandscapeMemoryManager* memory = NEngineInternal::GPLandscapeRenderer->GetMemoryManager();
    VertexBuffer = memory->AllocateVertexBuffer(this, VERTEX_NULL, WATER_PATCH_VERTEX_COUNT, sizeof(CTVertexWaterForeground),
                                                NEngineInternal::GPResourceManager->GetVertexBufferUsage());
    if (!VertexBuffer)
        return false;

    long compressedSize = stream.ReadSLONG();
    CArray<unsigned char> uncomp_buffer(WATER_PATCH_VERTEX_COUNT * WATER_PATCH_PACKED_VERTEX_SIZE);
    CArray<unsigned char> comp_buffer(compressedSize);
    stream.Read(comp_buffer.GetAsCArray(), compressedSize);
    CRangeCompressor compressor;
    compressor.Decompress(comp_buffer.GetAsCArray(), WATER_PATCH_VERTEX_COUNT, WATER_PATCH_PACKED_VERTEX_SIZE, uncomp_buffer.GetAsCArray());

    CMemoryDataInputStream vertex_stream(uncomp_buffer.GetAsCArray(), uncomp_buffer.size());
    CTVertexWaterForeground* vertices = (CTVertexWaterForeground*)VertexBuffer->Lock(0, 0, 0);
    for (long i = 0; i < WATER_PATCH_VERTEX_COUNT; i++) {
        vertices[i].X = vertex_stream.ReadType<unsigned short>();
        vertices[i].Y = vertex_stream.ReadType<unsigned short>();
        vertices[i].Z = vertex_stream.ReadFloat();
        vertices[i].SwellData0 = vertex_stream.ReadType<unsigned short>();
        vertices[i].SwellData1 = vertex_stream.ReadType<unsigned short>();
        SetTargetVertexDepth(vertices[i], vertex_stream.ReadType<short>());
        vertices[i].ShoreUCoord = vertex_stream.ReadFloat();
        vertex_stream.Read(vertices[i].ShoreLookUpArray, sizeof(vertices[i].ShoreLookUpArray));
    }
    VertexBuffer->Unlock();
    return true;
}


static void Put(std::vector<unsigned char>&a,const void*p,unsigned int n){const unsigned char*b=(const unsigned char*)p;a.insert(a.end(),b,b+n);}
static bool Bits(float a,float b){return memcmp(&a,&b,4)==0;}
static int Fail(int id,const char*p){printf("00beae50_TEST FAIL %d %s\n",id,p);return 1;}
int main(){
 if(sizeof(unsigned int)!=4||sizeof(short)!=2||sizeof(CTVertexWaterForeground)!=68)return Fail(-1,"host scalar model");
 const int types[]={INT_MIN,-100,-1,0,1,2,3,4,5,6,7,8,9,10,100,INT_MAX};const int chunks[]={0,1,2,3,4,7,64};const short limits[]={-32768,-32767,-1,0,1,8192,16383,16384,32767};int id=0;
 for(int t=0;t<16;++t)for(int variant=0;variant<8;++variant)for(int ch=0;ch<7;++ch)for(int fail=0;fail<2;++fail,++id){
  unsigned int seed=0x7919u+(unsigned int)id*31u;std::vector<unsigned char>input;CTVertexWaterForeground expected[289];float span;unsigned int spanBits=FiniteBits(seed);memcpy(&span,&spanBits,4);Put(input,&span,4);Put(input,&types[t],4);int size=19074;Put(input,&size,4);
  for(int i=0;i<289;++i){CTVertexWaterForeground&v=expected[i];v.X=(unsigned short)Rng(seed);v.Y=(unsigned short)Rng(seed);unsigned int bits=FiniteBits(seed);memcpy(&v.Z,&bits,4);v.SwellData0=(unsigned short)Rng(seed);v.SwellData1=(unsigned short)Rng(seed);short depth=i<9?limits[(i+variant)%9]:(short)((int)(Rng(seed)&65535u)-32768);v.Depth=(float)depth*(2.0f/32767.0f);if(v.Depth<0)v.Depth=0;if(v.Depth>2)v.Depth=2;bits=FiniteBits(seed);memcpy(&v.ShoreUCoord,&bits,4);for(int k=0;k<12;++k){bits=FiniteBits(seed);memcpy(&v.ShoreLookUpArray[k],&bits,4);}Put(input,&v.X,2);Put(input,&v.Y,2);Put(input,&v.Z,4);Put(input,&v.SwellData0,2);Put(input,&v.SwellData1,2);Put(input,&depth,2);Put(input,&v.ShoreUCoord,4);Put(input,v.ShoreLookUpArray,48);}
  bad=false;allocationFail=fail!=0;phase=arraysAlive=arrayCtors=arrayDtors=reads=usageCalls=allocationCalls=lockCalls=unlockCalls=decompressCalls=chunkCrossings=0;chunkSize=chunks[ch];usageValue=Rng(seed);compressedPointer=0;uncompressedPointer=0;memset(verts,0xa5,sizeof(verts));CWaterPatchMesh mesh;expectedOwner=&mesh;CDataInputStream stream(&input[0],(unsigned long)input.size());bool result=mesh.Load(stream);
  EWaterType want=(types[t]==2||types[t]==3||types[t]==4||types[t]==5||types[t]==6||types[t]==8)?(EWaterType)types[t]:WATER_TYPE_LAKE;
  if(bad||result==allocationFail||!Bits(mesh.WaterBodySpan,span)||mesh.WaterType!=want||usageCalls!=1||allocationCalls!=1)return Fail(id,"common state and allocation boundary");
  if(fail){if(mesh.VertexBuffer||stream.position!=8||reads!=2||arrayCtors||arraysAlive||arrayDtors||decompressCalls||lockCalls||unlockCalls||phase!=2)return Fail(id,"early allocation failure");continue;}
  if(mesh.VertexBuffer!=&buffer||stream.position!=input.size()||reads!=2316||arrayCtors!=2||arrayDtors!=2||arraysAlive||decompressCalls!=1||lockCalls!=1||unlockCalls!=1||phase!=9)return Fail(id,"complete call/lifetime protocol");
  if(chunkSize==1&&chunkCrossings==0)return Fail(id,"source chunk coverage");
  for(int i=0;i<289;++i){const CTVertexWaterForeground&a=verts[i],&b=expected[i];if(a.X!=b.X||a.Y!=b.Y||a.SwellData0!=b.SwellData0||a.SwellData1!=b.SwellData1||!Bits(a.Z,b.Z)||!Bits(a.Depth,b.Depth)||!Bits(a.ShoreUCoord,b.ShoreUCoord)||memcmp(a.ShoreLookUpArray,b.ShoreLookUpArray,48))return Fail(id,"all decoded fields including float payloads");}
 }
 if(id!=1792)return Fail(id,"scenario coverage");
 puts("00beae50_TEST PASS");return 0;
}