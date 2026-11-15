// Full target body verbatim, plus all actual shared template helper bodies.
// LP64 fixture uses typed pointers/size_t and small long values; native layouts are
// proved separately. All empty vectors have real backing storage, avoiding null subtraction.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "fable_compiler.h"
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#define FABLE_DUAL_PATH_TEST
#else
#define FABLE_FASTCALL
#define __fastcall
#define __cdecl
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_PASS22_FLAREBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_COMPARE
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_BODY
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_PASS22_COPY_BACKWARD_FASTCALL
#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
#define FABLE_RETAIL_CSHADERRENDERMANAGER_SELECTVERTEXBUFFER
#define FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS
#define FABLE_RETAIL_CGENERICVERTEX_PASS22_SETU1V1

#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
static int LiveBuffers,DestroyedBuffers,CreatedBuffers,InitialiseCalls,LockCalls,UnlockCalls;
static bool SwitchManagerOnInitialise;
static void StartVertexLifetimes(unsigned char*,long,long);
class CVertexBufferWin32;
struct IDirect3DVertexBuffer9 {CVertexBufferWin32* Owner;};
class IDirect3DDevice9 {
public:
 int* OwnerCalls;CVertexBufferWin32** OwnerLast;
 unsigned int LastStream,LastOffset,LastStride;IDirect3DVertexBuffer9* LastBuffer;
 IDirect3DDevice9(int* calls,CVertexBufferWin32** last):OwnerCalls(calls),OwnerLast(last),LastStream(99),LastOffset(99),LastStride(99),LastBuffer(0){}
 long SetStreamSource(unsigned int stream,IDirect3DVertexBuffer9* buffer,unsigned int offset,unsigned int stride){
  ++*OwnerCalls;*OwnerLast=buffer?buffer->Owner:0;LastStream=stream;LastBuffer=buffer;LastOffset=offset;LastStride=stride;return 0;
 }
};
class CShaderRenderManager {
public:
 int Calls;CVertexBufferWin32* Last;IDirect3DDevice9 Device;IDirect3DDevice9* D3DDevice;
 CShaderRenderManager():Calls(0),Last(0),Device(&Calls,&Last),D3DDevice(&Device){}
 void SelectVertexBuffer(CVertexBufferWin32& buffer);
};
namespace NEngineInternal {CShaderRenderManager* GPShaderRenderManager;}
static CShaderRenderManager ManagerA,ManagerB;
class CVertexBufferWin32 {
public:
 unsigned char* Bytes;unsigned long Length;long Type;unsigned char Stride;
 IDirect3DVertexBuffer9 Handle;IDirect3DVertexBuffer9* PVertexBuffer;
 CVertexBufferWin32():Bytes(0),Length(0),Type(0),Stride(0),PVertexBuffer(0){Handle.Owner=this;++LiveBuffers;++CreatedBuffers;}
 void MarkPreexisting(){PVertexBuffer=&Handle;Stride=32;}
 virtual ~CVertexBufferWin32(){if(Bytes)free(Bytes);--LiveBuffers;++DestroyedBuffers;}
 bool Initialise(EVertexType type,long count,long stride,unsigned long usage,EVertexBufferPool pool){
  assert(type==VERTEX_NULL&&count>=2&&count<=2048&&stride==32&&usage==0&&pool==VB_POOL_MANAGED);
  ++InitialiseCalls;PVertexBuffer=&Handle;Length=(unsigned long)count;Type=type;Stride=(unsigned char)stride;
  Bytes=(unsigned char*)malloc((size_t)count*(size_t)stride);assert(Bytes);memset(Bytes,0xA5,(size_t)count*(size_t)stride);StartVertexLifetimes(Bytes,count,stride);
  if(SwitchManagerOnInitialise)NEngineInternal::GPShaderRenderManager=&ManagerB;
  return true;
 }
 unsigned char* Lock(unsigned long a,unsigned long b,unsigned long c){assert(a==0&&b==0&&c==0);++LockCalls;assert(Bytes);return Bytes;}
 void Unlock(){++UnlockCalls;}
};
void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& buffer) { D3DDevice->SetStreamSource(0, buffer.PVertexBuffer, 0, buffer.Stride); }
class C3DVector {
public:float X,Y,Z;
 C3DVector(){}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
};
#include "test_helpers_sol2.inc"
static void StartVertexLifetimes(unsigned char* bytes,long count,long stride){for(long i=0;i<count;++i)new(bytes+i*stride)CTVertexNormTex1;}
#include "engine/GFFastSin.h"
float GFastCosTable[1025];
typedef std::pair<unsigned long,CCountedPointer<CVertexBufferWin32> > Pair;
typedef std::vector<Pair> BufferVector;
// Access-only fixture subclass; all vector behavior bodies remain original.
struct FixtureVector:BufferVector {
 void Back(unsigned int count,unsigned int capacity);
 void Overflow(Pair* p,const Pair& x,unsigned int n,bool end){_M_insert_overflow(p,x,std::__false_type(),n,end);}
};
class CEngineOcclusionManager {
public:
 FixtureVector FlareVertexBuffers;
 void SelectFlareTestVertexBuffer(unsigned long circle_segments);
};
namespace {
struct MapperPred {
    template<class T> bool operator()(const std::pair<unsigned long,T>& value,unsigned long key)const {return value.first<key;}
};
}
void CEngineOcclusionManager::SelectFlareTestVertexBuffer(unsigned long circle_segments)
{
    CCountedPointer<CVertexBufferWin32> vertexbuffer;
    std::vector<std::pair<unsigned long,CCountedPointer<CVertexBufferWin32> > >::iterator found=
        std::lower_bound(FlareVertexBuffers.begin(),FlareVertexBuffers.end(),circle_segments,MapperPred());
    if(found==FlareVertexBuffers.end()||found->first!=circle_segments) {
        vertexbuffer.Reset(new CVertexBufferWin32);
        vertexbuffer->Initialise(VERTEX_NULL,circle_segments+2,sizeof(CTVertexNormTex1),0,VB_POOL_MANAGED);
        CTVertexNormTex1* verts=reinterpret_cast<CTVertexNormTex1*>(vertexbuffer->Lock(0,0,0));
        CTVertexNormTex1& vert0=verts[0];
        vert0.SetPosition(C3DVector(0,0,0));
        float anglefactor=1.0f/(float)circle_segments;
        for(unsigned long c_vert=0;c_vert<circle_segments+1;++c_vert) {
            CTVertexNormTex1& vert=verts[c_vert+1];
            vert.SetPosition(C3DVector(GFFastCos(c_vert*anglefactor),-GFFastSin(c_vert*anglefactor),0));
            vert.SetNormal(C3DVector(0,0,0));
            vert.SetU1V1(0,0);
        }
        vertexbuffer->Unlock();
        FlareVertexBuffers.insert(found,std::make_pair(circle_segments,vertexbuffer));
    } else {
        vertexbuffer=found->second;
    }
    NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*vertexbuffer);
}

static unsigned int Bits(float v){unsigned int b;memcpy(&b,&v,4);return b;}
static long RefNearest(float v){double d=floor((double)v);double f=(double)v-d;long result=(long)d;if(f>0.5||(f==0.5&&(result&1)))++result;return result;}
static float RefTrig(float angle,bool sine){float pos=angle*1024.0f;long n=RefNearest(pos-0.5f);float f=pos-(float)n;unsigned int i=(unsigned int)(n-(sine?256:0))&1023u;return (1.0f-f)*GFastCosTable[i]+f*GFastCosTable[i+1];}
void FixtureVector::Back(unsigned int count,unsigned int capacity){
 FixtureVector& v=*this;
 assert(capacity>=count&&capacity>0);assert(!v._M_start);
 v._M_start=(Pair*)malloc(sizeof(Pair)*capacity);assert(v._M_start);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+capacity;
 for(unsigned int i=0;i<count;++i){CCountedPointer<CVertexBufferWin32> p(new CVertexBufferWin32);p->MarkPreexisting();new(v._M_finish)Pair((i+1)*4,p);++v._M_finish;}
}
static void BackVector(FixtureVector& v,unsigned int count,unsigned int capacity){v.Back(count,capacity);}
static void VerifyFan(CVertexBufferWin32* buffer,unsigned long segments){
 assert(buffer&&buffer->Length==segments+2&&buffer->Stride==32&&buffer->Type==VERTEX_NULL);
 const CTVertexNormTex1* vertices=(const CTVertexNormTex1*)buffer->Bytes;
 assert(Bits(vertices[0].X)==0&&Bits(vertices[0].Y)==0&&Bits(vertices[0].Z)==0);
 for(unsigned int i=12;i<32;++i)assert(buffer->Bytes[i]==0xA5); // center's normal/UV are not written natively
 float factor=1.0f/(float)segments;
 for(unsigned long i=0;i<=segments;++i){
  const CTVertexNormTex1& v=vertices[i+1];float angle=(float)i*factor;
  assert(fabs((double)v.X-RefTrig(angle,false))<0.000001);
  assert(fabs((double)v.Y+RefTrig(angle,true))<0.000001);
  assert(Bits(v.Z)==0&&Bits(v.NormalX)==0&&Bits(v.NormalY)==0&&Bits(v.NormalZ)==0&&Bits(v.U1)==0&&Bits(v.V1)==0);
 }
}
static void OneScenario(unsigned int oldcount,unsigned int spare,unsigned long segments,bool switch_manager){
 const int before=LiveBuffers,created=CreatedBuffers,initialised=InitialiseCalls,locked=LockCalls,unlocked=UnlockCalls;
 {
  CEngineOcclusionManager node;BackVector(node.FlareVertexBuffers,oldcount,oldcount+spare+(oldcount==0?1:0));
  bool hit=false;CVertexBufferWin32* expected=0;
  for(unsigned int i=0;i<oldcount;++i)if(node.FlareVertexBuffers[i].first==segments){hit=true;expected=node.FlareVertexBuffers[i].second.Get();}
  ManagerA.Calls=0;ManagerB.Calls=0;ManagerA.Last=0;ManagerB.Last=0;
  NEngineInternal::GPShaderRenderManager=&ManagerA;SwitchManagerOnInitialise=switch_manager;
  node.SelectFlareTestVertexBuffer(segments);
  assert(CreatedBuffers-created==(int)oldcount+(hit?0:1));
  assert(InitialiseCalls-initialised==(hit?0:1));assert(LockCalls-locked==(hit?0:1));assert(UnlockCalls-unlocked==(hit?0:1));
  CVertexBufferWin32* selected=switch_manager&&!hit?ManagerB.Last:ManagerA.Last;
  assert(ManagerA.Calls==(switch_manager&&!hit?0:1)&&ManagerB.Calls==(switch_manager&&!hit?1:0));
  if(hit)assert(selected==expected);else VerifyFan(selected,segments);
  IDirect3DDevice9& device=switch_manager&&!hit?ManagerB.Device:ManagerA.Device;
  assert(device.LastStream==0&&device.LastOffset==0&&device.LastStride==selected->Stride&&device.LastBuffer==selected->PVertexBuffer);
  assert(node.FlareVertexBuffers.size()==oldcount+(hit?0:1));
  for(unsigned int i=0;i<node.FlareVertexBuffers.size();++i){
   if(i)assert(node.FlareVertexBuffers[i-1].first<node.FlareVertexBuffers[i].first);
   assert(node.FlareVertexBuffers[i].second.GetReferenceCount()==1);
   if(node.FlareVertexBuffers[i].first==segments)assert(node.FlareVertexBuffers[i].second.Get()==selected);
  }
  // Cache repeat is a hit, preserving allocation, reference counts and all bytes.
  int c=CreatedBuffers,l=LockCalls;node.SelectFlareTestVertexBuffer(segments);assert(CreatedBuffers==c&&LockCalls==l);
  for(unsigned int i=0;i<node.FlareVertexBuffers.size();++i)assert(node.FlareVertexBuffers[i].second.GetReferenceCount()==1);
 }
 assert(LiveBuffers==before);
}
typedef FixtureVector ExposedVector;
static void CheckHelpers(){
 // Lower-bound boundary/duplicate keys with real backing, including empty range.
 {
  FixtureVector v;BackVector(v,4,6);v[1].first=4;v[2].first=8;v[3].first=12;
  for(unsigned long key=0;key<=16;++key){Pair* actual=std::lower_bound(v.begin(),v.end(),key,MapperPred());Pair* ref=v.begin();while(ref!=v.end()&&ref->first<key)++ref;assert(actual==ref);}
  assert(std::lower_bound(v.begin(),v.begin(),0UL,MapperPred())==v.begin());
 }
 // Direct full Reset/assignment/deleter lifetimes, shared-owner and self assignments.
 {
  CCountedPointer<CVertexBufferWin32> a(new CVertexBufferWin32),b;
  a=a;assert(a.GetReferenceCount()==1);b=a;assert(a.GetReferenceCount()==2&&a.Get()==b.Get());b=b;assert(a.GetReferenceCount()==2);
  b.Reset(new CVertexBufferWin32);assert(a.GetReferenceCount()==1&&b.GetReferenceCount()==1);a=b;assert(b.GetReferenceCount()==2);a.Reset(0);assert(b.GetReferenceCount()==1);
  CCountedPointer<CVertexBufferWin32>::DeleteData(new CVertexBufferWin32);
 }
 // Full counted backward-copy body with overlap and pair assignment/destruction.
 {
  FixtureVector v;BackVector(v,5,6);Pair* finish=std::__copy_backward(v.begin(),v.begin()+4,v.end(),std::random_access_iterator_tag(),(ptrdiff_t*)0);assert(finish==v.begin()+1);
  unsigned long keys[]={4,4,8,12,16};for(int i=0;i<5;++i)assert(v[i].first==keys[i]);
 }
 // Non-POD uninitialized-copy/fill and destroy_aux use actual shared bodies.
 {
  FixtureVector v;BackVector(v,3,4);Pair* data=(Pair*)malloc(sizeof(Pair)*6);assert(data);
  Pair* p=std::__uninitialized_copy(v.begin(),v.end(),data,std::__false_type());assert(p==data+3);
  for(int i=0;i<3;++i)assert(v[i].second.GetReferenceCount()==2&&data[i].first==v[i].first);
  std::__destroy_aux(data,data+3,std::__false_type());
  p=std::__uninitialized_fill_n(data,6u,v[1],std::__false_type());assert(p==data+6);assert(v[1].second.GetReferenceCount()==7);
  std::__destroy_aux(data,data+6,std::__false_type());assert(v[1].second.GetReferenceCount()==1);free(data);
 }
 // Every overflow arm: empty/nonempty, prefix/suffix, fill0/1/many, atEnd.
 for(unsigned int count=0;count<=4;++count)for(unsigned int fill=0;fill<=3;++fill)for(unsigned int mode=0;mode<3;++mode){
  if(count==0&&fill==0)continue; // avoid source library's nullptr subtraction on an empty zero-capacity result
  ExposedVector v;BackVector(v,count,count+1);CCountedPointer<CVertexBufferWin32> x(new CVertexBufferWin32);Pair value(99,x);
  unsigned int pos=mode==0?0:mode==1?count/2:count;bool end=mode==2;
  v.Overflow(v.begin()+pos,value,fill,end);assert(v.size()==count+fill);
  for(unsigned int i=0;i<pos;++i)assert(v[i].first==(i+1)*4);
  for(unsigned int i=0;i<fill;++i)assert(v[pos+i].first==99&&v[pos+i].second.Get()==x.Get());
  for(unsigned int i=pos;i<count;++i)assert(v[i+fill].first==(i+1)*4);
  assert(x.GetReferenceCount()==fill+2);
 }
 assert(LiveBuffers==0);
}
static void CheckNear(float value){
 long actual=GFFloatToLongNear(value);long portable=fable_portable::GFFloatToLongNear(value);assert(actual==portable);
 assert(actual>=-2147483647L-1L&&actual<=2147483647L);
}
int main(){
#ifdef _MSC_VER
 _controlfp(_PC_53|_RC_NEAR,_MCW_PC|_MCW_RC);
#endif
 for(unsigned int i=0;i<1025;++i)GFastCosTable[i]=(float)(i%29)*0.03125f-0.4375f; // controlled external table, no retail bytes copied
 CheckHelpers();unsigned int cases=0;
 for(unsigned int count=0;count<=5;++count)for(unsigned int spare=0;spare<=2;++spare)for(unsigned long n=1;n<=26;++n)for(unsigned int sw=0;sw<2;++sw){OneScenario(count,spare,n,sw!=0);++cases;}
 assert(cases==936);
 unsigned long extra[]={31,32,33,63,64,65,127,128,129,255,256,257,1024};
 for(unsigned int i=0;i<sizeof(extra)/sizeof(extra[0]);++i)for(unsigned int sw=0;sw<2;++sw){OneScenario(0,0,extra[i],sw!=0);++cases;}
 assert(cases==962);assert(LiveBuffers==0);
 float nearcases[]={0.0f,-0.0f,0.25f,-0.25f,0.5f,-0.5f,1.5f,-1.5f,2.5f,-2.5f,1023.5f,2147483520.0f,-2147483648.0f,2147483648.0f};
 for(unsigned int i=0;i<sizeof(nearcases)/sizeof(nearcases[0]);++i)CheckNear(nearcases[i]);
 unsigned int state=0x8b8dcd0u;for(int j=0;j<20000;++j){state=state*1664525u+1013904223u;float value;memcpy(&value,&state,4);CheckNear(value);}
 puts("00b8dcd0_TEST PASS");return 0;
}
