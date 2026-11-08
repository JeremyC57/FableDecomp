// Fixture (agent680: target body replaced with src/00b8d420.cpp's).
// Behavior-only fixture. Target, PrepareForRender and buffer-draw bodies are
// pasted unchanged. SDK calls and unrelated update implementations are checked
// boundaries on actual constructed objects; native layouts/slots are not claimed.
#include "fable_compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __stdcall
#endif
static int cases,eventCount,events[32],openCaptures;static unsigned long presetCount;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
static void record(int e){CHECK(eventCount<32);events[eventCount++]=e;}
enum EPrimitiveType{PT_NULL=-1,PT_TRIANGLE_LIST=4};enum D3DPRIMITIVETYPE{D3DPT_TRIANGLELIST=4};
class CMatrix3x4{public:float Data[12];};class CVertexShader{};class CPixelShader{};struct IDirect3DVertexBuffer9{ private:};struct IDirect3DIndexBuffer9{ private:};
static CVertexShader*expectedVS;static CPixelShader*expectedPS;static CMatrix3x4*expectedTransform;static IDirect3DVertexBuffer9*expectedVB;static IDirect3DIndexBuffer9*expectedIB;
struct IDirect3DDevice9{public:virtual~IDirect3DDevice9(){}virtual long __stdcall SetStreamSource(unsigned int,IDirect3DVertexBuffer9*,unsigned int,unsigned int)=0;virtual long __stdcall SetIndices(IDirect3DIndexBuffer9*)=0;virtual long __stdcall DrawIndexedPrimitive(D3DPRIMITIVETYPE,int,unsigned int,unsigned int,unsigned int,unsigned int)=0;};
class CTestDevice:public IDirect3DDevice9{public:long __stdcall SetStreamSource(unsigned int stream,IDirect3DVertexBuffer9*v,unsigned int offset,unsigned int stride){CHECK(stream==0&&v==expectedVB&&offset==0&&stride==12);record(12);return 0;}long __stdcall SetIndices(IDirect3DIndexBuffer9*i){CHECK(i==expectedIB);record(13);return 0;}long __stdcall DrawIndexedPrimitive(D3DPRIMITIVETYPE type,int base,unsigned int first,unsigned int count,unsigned int start,unsigned int primitives){CHECK(type==D3DPT_TRIANGLELIST&&base==0&&first==0&&count==8&&start==0&&primitives==12);record(15);return 0;}};
class CRenderManager{public:void SetColourWriteEnable(bool x){CHECK(!x);record(3);}void SetZBufferWriteEnable(bool x){CHECK(!x);record(4);}void RealiseRenderState(){record(14);}};
class CRenderStateCaptureBlock{bool Open;public:CRenderStateCaptureBlock():Open(true){CHECK(openCaptures==0);++openCaptures;record(0);}~CRenderStateCaptureBlock(){CHECK(Open&&openCaptures==1);--openCaptures;record(17);}};
class CVertexShaderConstant{public:float X,Y,Z,W;};
template<class T>class CArray{public:T*First;unsigned int Count;unsigned int size()const{return Count;}const T*PeekAsCArray()const{return Count?First:reinterpret_cast<const T*>(this);}};
class CShaderConstantRange{public:long Offset;unsigned long Count;};class CVertexShaderConstantLayout{public:CShaderConstantRange Reserved;CArray<CVertexShaderConstant>ReservedPresets;};
class CVertexBufferWin32{public:IDirect3DVertexBuffer9*Buffer;long GetStride()const{return 12;}IDirect3DVertexBuffer9*GetD3DVertexBuffer(){return Buffer;}};
class CIndexBuffer{public:IDirect3DIndexBuffer9*Buffer;IDirect3DIndexBuffer9*GetD3DIndexBuffer(){return Buffer;}EPrimitiveType GetPrimitiveType()const{return PT_TRIANGLE_LIST;}};
class CShaderRenderManager{public:const CVertexShaderConstantLayout*CurrentVSConstantLayout;CRenderManager*RenderManager;IDirect3DDevice9*D3DDevice;bool WorldMatrixIsIdentity;unsigned long UpdateFlags;
 void ApplyVertexShader(const CVertexShader&v,bool flag){CHECK(&v==expectedVS&&!flag);record(1);}void ApplyPixelShader(const CPixelShader&p){CHECK(&p==expectedPS);record(2);}void SetWorldTransform(const CMatrix3x4&t){CHECK(&t==expectedTransform);for(int i=0;i<12;++i)CHECK(t.Data[i]==(float)i);}
 void SetVSConstants(long reg,const void*data,unsigned long count){CHECK(reg==7&&count==presetCount&&data==CurrentVSConstantLayout->ReservedPresets.PeekAsCArray());if(count){const CVertexShaderConstant*v=static_cast<const CVertexShaderConstant*>(data);CHECK(v[0].X==1.0f&&v[0].Y==2.0f&&v[0].Z==3.0f&&v[0].W==4.0f&&v[1].X==5.0f&&v[1].Y==6.0f&&v[1].Z==7.0f&&v[1].W==8.0f);}record(5);}void UpdatePixelShader(){record(6);}void UpdatePixelShaderConstants(){record(7);}void UpdateAmbient(){record(8);}void UpdateFogColour(){record(9);}void UpdateLightGlobals(){record(10);}void UpdateCombinedProjectionTransform(){record(11);}
 void UpdateBoneTransform(){CHECK(false);}void UpdateWorldSpaceTextureTransform(){CHECK(false);}void UpdateObjectSpaceTextureTransform(){CHECK(false);}void UpdateWorldSpaceFogTransform(){CHECK(false);}void UpdateObjectSpaceFogTransform(){CHECK(false);}void UpdateObjectSpaceLighting(unsigned long){CHECK(false);}void UpdateWorldSpaceLighting(){CHECK(false);}void UpdateLocalisedSphereMapping(){CHECK(false);}void ResetWorldTransform(){CHECK(false);}void UpdateCameraPos(){CHECK(false);}
 FABLE_FORCEINLINE void SelectVertexBuffer(CVertexBufferWin32&vb){D3DDevice->SetStreamSource(0,vb.GetD3DVertexBuffer(),0,vb.GetStride());}
 FABLE_FORCEINLINE void DrawBuffersIndexed(CVertexBufferWin32&vb,unsigned long first,unsigned long count,CIndexBuffer&ib,unsigned long start,unsigned long primitives,EPrimitiveType type){SelectVertexBuffer(vb);D3DDevice->SetIndices(ib.GetD3DIndexBuffer());if(type==PT_NULL)type=ib.GetPrimitiveType();RenderManager->RealiseRenderState();D3DDevice->DrawIndexedPrimitive(static_cast<D3DPRIMITIVETYPE>(type),0,first,count,start,primitives);}
    FABLE_FORCEINLINE void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.PeekAsCArray(),
                           CurrentVSConstantLayout->ReservedPresets.size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

};
class CEngineOcclusionManager;
namespace NEngineInternal{CRenderManager*GPRenderManager;CShaderRenderManager*GPShaderRenderManager;CEngineOcclusionManager*GPOcclusionManager;}
#ifdef _MSC_VER
typedef unsigned long OcclusionWord;
#else
typedef unsigned int OcclusionWord;
#endif
class CEngineOcclusionManager{public:struct COccluderEntry{unsigned long PixelCount;OcclusionWord SlotID:30;OcclusionWord Testing:1;OcclusionWord Completed:1;COccluderEntry():PixelCount(0),SlotID(0),Testing(0),Completed(0){}};unsigned long EntriesSize;COccluderEntry*Entries;CVertexShader OcclusionBoxVShader;CPixelShader OcclusionBoxPixelShader;CVertexBufferWin32 UnitBoxVB;CIndexBuffer UnitBoxIB;
 unsigned long AllocateOccluderHandle(){record(16);if(EntriesSize<8192){unsigned long handle=EntriesSize;reinterpret_cast<COccluderEntry*>(Entries)[handle]=COccluderEntry();++EntriesSize;return handle;}return static_cast<unsigned long>(-1);}unsigned long RenderOcclusionBox(const CMatrix3x4&);
};
unsigned long CEngineOcclusionManager::RenderOcclusionBox(const CMatrix3x4&transform){
 CRenderStateCaptureBlock capture;
 NEngineInternal::GPShaderRenderManager->ApplyVertexShader(reinterpret_cast<const CVertexShader&>(OcclusionBoxVShader),false);
 NEngineInternal::GPShaderRenderManager->ApplyPixelShader(reinterpret_cast<const CPixelShader&>(OcclusionBoxPixelShader));
 NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
 NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
 NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
 NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
 NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(reinterpret_cast<CVertexBufferWin32&>(UnitBoxVB),0,8,reinterpret_cast<CIndexBuffer&>(UnitBoxIB),0,12,PT_NULL);
 return NEngineInternal::GPOcclusionManager->AllocateOccluderHandle();
}

int main(){const unsigned long sizes[]={0,1,4096,8191,8192,8193};const unsigned long bits[]={4,1,2,0x20000,0x400,0x80};
 for(int mask=0;mask<64;++mask)for(int size=0;size<6;++size)for(int preset=0;preset<2;++preset){
  CEngineOcclusionManager receiver,allocation;CEngineOcclusionManager::COccluderEntry entries[8192];for(int j=0;j<8192;++j){entries[j].PixelCount=123;entries[j].SlotID=77;entries[j].Testing=1;entries[j].Completed=1;}
  receiver.EntriesSize=999;receiver.Entries=0;allocation.EntriesSize=sizes[size];allocation.Entries=entries;
  CRenderManager render;CShaderRenderManager shaders;CTestDevice device;IDirect3DVertexBuffer9 vb;IDirect3DIndexBuffer9 ib;CMatrix3x4 transform;CVertexShaderConstant presets[2];CVertexShaderConstantLayout layout;
  for(int f=0;f<12;++f)transform.Data[f]=(float)f;presets[0].X=1.0f;presets[0].Y=2.0f;presets[0].Z=3.0f;presets[0].W=4.0f;presets[1].X=5.0f;presets[1].Y=6.0f;presets[1].Z=7.0f;presets[1].W=8.0f;
  layout.Reserved.Offset=7;layout.Reserved.Count=2;layout.ReservedPresets.First=presets;layout.ReservedPresets.Count=preset?2:0;presetCount=layout.ReservedPresets.Count;shaders.CurrentVSConstantLayout=&layout;shaders.RenderManager=&render;shaders.D3DDevice=&device;shaders.WorldMatrixIsIdentity=false;shaders.UpdateFlags=0xfb28;for(int b=0;b<6;++b)if(mask&(1<<b))shaders.UpdateFlags|=bits[b];
  receiver.UnitBoxVB.Buffer=&vb;receiver.UnitBoxIB.Buffer=&ib;expectedVS=&receiver.OcclusionBoxVShader;expectedPS=&receiver.OcclusionBoxPixelShader;expectedTransform=&transform;expectedVB=&vb;expectedIB=&ib;
  NEngineInternal::GPRenderManager=&render;NEngineInternal::GPShaderRenderManager=&shaders;NEngineInternal::GPOcclusionManager=&allocation;
  eventCount=openCaptures=0;unsigned long result=receiver.RenderOcclusionBox(transform);CHECK(openCaptures==0&&receiver.EntriesSize==999);
  int expected[32],n=0;expected[n++]=0;expected[n++]=1;expected[n++]=2;expected[n++]=3;expected[n++]=4;if(mask&1)expected[n++]=5;if(mask&2)expected[n++]=6;if(mask&4)expected[n++]=7;if(mask&1)expected[n++]=8;if(mask&8)expected[n++]=9;if(mask&16)expected[n++]=10;if(mask&32)expected[n++]=11;for(int e=12;e<=17;++e)expected[n++]=e;
  CHECK(n==eventCount);for(int q=0;q<n;++q)CHECK(events[q]==expected[q]);
  CHECK(result==(sizes[size]<8192?sizes[size]:static_cast<unsigned long>(-1))&&allocation.EntriesSize==sizes[size]+(sizes[size]<8192?1:0));
  for(int z=0;z<8192;++z){bool changed=sizes[size]<8192&&(unsigned long)z==sizes[size];CHECK(entries[z].PixelCount==(changed?0UL:123UL)&&entries[z].SlotID==(changed?0U:77U)&&entries[z].Testing==(changed?0U:1U)&&entries[z].Completed==(changed?0U:1U));}++cases;
 }
 CHECK(cases==768);puts("00b8d420_TEST PASS");return 0;}