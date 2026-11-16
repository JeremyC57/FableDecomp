// 01212472 positional-source update full-body real-object behavior test
// Flags: none
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23
// 01212472 CUnprovenSoundWorld_00c1c460::UpdateSourcePositions parent and generated guard cleanup
// Leaf: ?UpdateSourcePositions@CUnprovenSoundWorld_00c1c460@@QAEXXZ
// Flags: /EHsc /Gr /d2inlT1
// Inline-budget: native00c1c5b2 calls Copy; ordinary full-body expansion is435B.
// The local /d2inlT1 budget retains that call and its genuine escape information.
#define FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_BODY_PASS23
#define FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23
#define FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_PASS23
#define FABLE_RETAIL_C3DVECTOR_LIBPK_OUTOFLINE
#include "libpk/CUnprovenSoundWorld_00c1c460.h"

// 0x00C1BF00: the listener frame: axes right = top x front, top, front as the columns, the translation -axis.position, and its
// inverse.
static void BuildListenerMatrices(CMatrix3x4& world_to_listener, CMatrix3x4* listener_to_world, const C3DVector& position,
                                  const C3DVector& front, const C3DVector& top)
{
    C3DVector right = top.Cross(front);
    world_to_listener.E41=world_to_listener.E42=world_to_listener.E43=world_to_listener.E12=world_to_listener.E13=world_to_listener.E21=world_to_listener.E23=world_to_listener.E31=world_to_listener.E32=0.0f;
    world_to_listener.E11=world_to_listener.E22=world_to_listener.E33=1.0f;
    world_to_listener.E11 = right.X; world_to_listener.E12 = top.X; world_to_listener.E13 = front.X;
    world_to_listener.E21 = right.Y; world_to_listener.E22 = top.Y; world_to_listener.E23 = front.Y;
    world_to_listener.E31 = right.Z; world_to_listener.E32 = top.Z; world_to_listener.E33 = front.Z;
    world_to_listener.E41 = -right.DotProduct(position);
    world_to_listener.E42 = -top.DotProduct(position);
    world_to_listener.E43 = -front.DotProduct(position);
    InvertMatrix3x4(listener_to_world, world_to_listener);
}

// 0x00C1C3F0 (landed): point through a 4x3 matrix.
static C3DVector TransformHelperPoint_00c1c3f0(const CMatrix3x4& matrix, const C3DVector& point)
{
    C3DVector result;
    result.X = matrix.E31 * point.Z + matrix.E21 * point.Y + matrix.E11 * point.X + matrix.E41;
    result.Y = matrix.E32 * point.Z + matrix.E12 * point.X + matrix.E22 * point.Y + matrix.E42;
    result.Z = matrix.E33 * point.Z + matrix.E13 * point.X + matrix.E23 * point.Y + matrix.E43;
    return result;
}

// 0x00C1C330: point through a, depth mirrored to the front (z < 0 -> -z), then through b.
static C3DVector TransformPointThroughMirroredFrame_00c1c330(const CMatrix3x4& a, const CMatrix3x4& b, const C3DVector& point)
{
    float x = a.E31 * point.Z + a.E21 * point.Y + a.E11 * point.X + a.E41;
    float y = a.E32 * point.Z + a.E12 * point.X + a.E22 * point.Y + a.E42;
    float z = a.E33 * point.Z + a.E13 * point.X + a.E23 * point.Y + a.E43;
    if (z < 0.0f)
        z *= -1.0f;
    C3DVector result;
    result.X = b.E21 * y + b.E31 * z + b.E11 * x + b.E41;
    result.Y = b.E22 * y + b.E12 * x + b.E32 * z + b.E42;
    result.Z = b.E23 * y + b.E13 * x + b.E33 * z + b.E43;
    return result;
}

void CUnprovenSoundWorld_00c1c460::UpdateSourcePositions()
{
    if (SourcePositionsEnabled) {
        CSoundEventManager* manager = PManager;
        if (manager->ListenerValid) {
            C3DVector position = SwapYZ(manager->ListenerPosition);
            C3DVector front = SwapYZ(manager->ListenerFront);
            C3DVector top = SwapYZ(manager->ListenerTop);
            CMatrix3x4 listener_to_world;
            CMatrix3x4 world_to_listener;
            BuildListenerMatrices(world_to_listener, &listener_to_world, position, front, top);
            PKCProtectionInstance lock(&g_soundLock);
            for(unsigned long i=0;;++i){
                unsigned long count=PManager->m_events.Playbacks.m_count;
                if(i>=count)break;
                CPlaybackSlot* slot = PManager->m_events.Playbacks.Get(i);
                if (slot->PEvent && slot->PEvent->PPlayingData && slot->Positional) {
                    CSoundPosition event_position = slot->PEvent->PositionHolder.GetValue();
                    C3DVector listener_space;
                    C3DVector source = SwapYZ(event_position.Position);
                    if (event_position.Relative)
                        listener_space = TransformHelperPoint_00c1c3f0(listener_to_world, source);
                    else
                        listener_space = TransformPointThroughMirroredFrame_00c1c330(world_to_listener, listener_to_world, source);
                    CVECTOR coda_position;
                    coda_position = listener_space.Copy();
                    CODA_3DSourceSetPosition(slot->SourceHandle, &coda_position);
                }
            }
        }
    }
    CODA_Commit();
}

void CSoundEventManager::AddToWorld(void*){abort();}
void CSoundEventManager::SetMember888(long){abort();}
void CSoundEventManager::SetWorldMode(bool){abort();}
void CSoundEventManager::SetMember7cc(long){abort();}
void* CSoundEventManager::GetWorldSelection(){abort();}
void* CSoundEventManager::FindInWorld(void*){abort();}
void CSoundEventManager::SetMember884(unsigned char){abort();}
void CSoundEventManager::SetWorldGain(float){abort();}
float CSoundEventManager::GetWorldGain(){abort();}
long CSoundEventManager::SetWorldEnabled(void*){abort();}
int CSoundEventManager::IsWorldRunning(){abort();}
long CSoundEventManager::SetWorldState(void*){abort();}
void CSoundEventManager::SetListenerFrame(const CSoundPosition*,const CSoundPosition*,const CSoundPosition*){abort();}
void CSoundEventManager::SetListenerPosition(const CSoundPosition*){abort();}
long CSoundEventManager::PlayStreamed(CStreamSoundBank*,long,unsigned long,const Input*){abort();}
int CStreamBankLookup::IsEqual(const void*,const void*)const{abort();}
int CStreamBankLookup::IsLess(const void*,const void*)const{abort();}
int CStreamBankLookup::IsKeyEqual(const void*,const void*)const{abort();}
int CStreamBankLookup::IsKeyLess(const void*,const void*)const{abort();}
void CStreamBankLookup::TransferAllFrom(CStreamBankLookup*){abort();}
void* CStreamBankLookup::FindByKey(const void*){abort();}
int CSoundEventList::IsEqual(const void*,const void*)const{abort();}
int CSoundEventList::IsLess(const void*,const void*)const{abort();}
int CSoundEventList::IsKeyEqual(const void*,const void*)const{abort();}
int CSoundEventList::IsKeyLess(const void*,const void*)const{abort();}
void CSoundEventList::TransferAllFrom(CSoundEventList*){abort();}
CSoundEvent* CSoundEventList::FindByKey(const CSampleRef*){abort();}
int CSoundEventSet::IsEqual(const void*,const void*)const{abort();}
int CSoundEventSet::IsLess(const void*,const void*)const{abort();}
int CSoundEventSet::IsKeyEqual(const void*,const void*)const{abort();}
int CSoundEventSet::IsKeyLess(const void*,const void*)const{abort();}
void CHandleBasedMemoryPool_012b4098::MoveResource(void* resource, long handle, void* from, void* to, unsigned long size){abort();}
int CHandleBasedMemoryPool_012b4098::CanMoveResource(void* resource, long handle){abort();}
void CHandleBasedMemoryPool_012b4098::ValidateResource(void* resource){abort();}
long* CSoundEvent::GetIndexSlot(){abort();}
void CSoundEvent::ResetLevel(CSoundEventManager* manager){abort();}
void CSoundEvent::SetUserGain(CSoundEventManager* manager,float gain){abort();}
void CSoundEvent::SetPlaybackParameter(CSoundEventManager* manager,float value){abort();}
void CSoundEvent::ResetPlaybackState(CSoundEventManager* manager){abort();}
void CSoundEvent::Set3DPosition(CSoundEventManager* manager,const CSoundPosition& position){abort();}
void CSoundEvent::SetPositionParameters(CSoundEventManager* manager,float first,float second){abort();}
void CSoundEvent::ApplyPan(CSoundEventManager* manager,float pan){abort();}
bool CSoundEvent::GetPositionFlag(CSoundEventManager* manager){abort();}

static CUnprovenSoundWorld_00c1c460*World;
static CSoundEventManager*Manager;
static CPlaybackSlot*Slots;
static unsigned long SlotCount;
static int Depth,Enters,Leaves,PoolCalls,HolderCalls,Copies,APICalls,InverseCalls,CrossCalls,DotCalls,SwapCalls,Tails,ThrowAt;
static unsigned long LastIndex,ExpectedIndices[8],ExpectedCount;
static C3DVector ExpectedPositions[8];
static void ThrowIf(int n){
#ifndef _MSC_VER
 if(ThrowAt==n)throw n;
#else
 assert(!ThrowAt);(void)n;
#endif
}
static C3DVector V(float x,float y,float z){C3DVector v;v.X=x;v.Y=y;v.Z=z;return v;}
static C3DVector Swizzle(const C3DVector&v){return V(v.X,v.Z,v.Y);}
static C3DVector CrossValue(const C3DVector&a,const C3DVector&b){return V(a.Y*b.Z-a.Z*b.Y,a.Z*b.X-a.X*b.Z,a.X*b.Y-a.Y*b.X);}
static float DotValue(const C3DVector&a,const C3DVector&b){return a.X*b.X+a.Y*b.Y+a.Z*b.Z;}
static void Equal(const C3DVector&a,const C3DVector&b){assert(a.X==b.X&&a.Y==b.Y&&a.Z==b.Z);}
PKCCriticalSection::PKCCriticalSection(){}PKCCriticalSection::~PKCCriticalSection(){}
void PKCCriticalSection::Enter(){assert(Depth==0);++Depth;++Enters;}
void PKCCriticalSection::Leave(){assert(Depth==1);--Depth;++Leaves;}
PKCCriticalSection g_soundLock;
PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection*p):m_pSection(p),m_tick(0){assert(p==&g_soundLock);p->Enter();}
PKCProtectionInstance::~PKCProtectionInstance(){assert(m_pSection==&g_soundLock);m_pSection->Leave();}
CSoundPosition::CSoundPosition():Relative(false){Position.X=Position.Y=Position.Z=0;}
CSoundPosition CQuadHolder::GetValue()const{assert(Depth==1&&LastIndex<SlotCount);assert(this==&Slots[LastIndex].PEvent->PositionHolder);++HolderCalls;return Value;}
C3DVector __fastcall SwapYZ(const C3DVector&p){++SwapCalls;if(Depth==0)assert(&p==&Manager->ListenerPosition||&p==&Manager->ListenerFront||&p==&Manager->ListenerTop);return Swizzle(p);}
const C3DVector C3DVector::Cross(const C3DVector&v)const{assert(Depth==0);++CrossCalls;return CrossValue(*this,v);}
float C3DVector::DotProduct(const C3DVector&v)const{assert(Depth==0);++DotCalls;return DotValue(*this,v);}
#ifndef FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_BODY_PASS23
_CVECTOR C3DVector::Copy()const{assert(Depth==1);++Copies;CVECTOR v;v.x=X;v.y=Y;v.z=Z;return v;}
#endif
void FABLE_FASTCALL InvertMatrix3x4(CMatrix3x4*out,const CMatrix3x4&m){assert(Depth==0);++InverseCalls;ThrowIf(1);
 C3DVector p=Swizzle(Manager->ListenerPosition),f=Swizzle(Manager->ListenerFront),t=Swizzle(Manager->ListenerTop),r=CrossValue(t,f);
 float got[]={m.E11,m.E12,m.E13,m.E21,m.E22,m.E23,m.E31,m.E32,m.E33,m.E41,m.E42,m.E43};
 float expected[]={r.X,t.X,f.X,r.Y,t.Y,f.Y,r.Z,t.Z,f.Z,-DotValue(r,p),-DotValue(t,p),-DotValue(f,p)};
 for(int i=0;i<12;++i)assert(got[i]==expected[i]);
 out->E11=r.X;out->E12=r.Y;out->E13=r.Z;out->E21=t.X;out->E22=t.Y;out->E23=t.Z;out->E31=f.X;out->E32=f.Y;out->E33=f.Z;out->E41=p.X;out->E42=p.Y;out->E43=p.Z;
}
extern "C" void __stdcall CODA_3DSourceSetPosition(CHANDLE handle,const CVECTOR*position){assert(Depth==1);assert(APICalls<(int)ExpectedCount);unsigned long i=ExpectedIndices[APICalls];assert(handle==Slots[i].SourceHandle);assert(position->x==ExpectedPositions[APICalls].X&&position->y==ExpectedPositions[APICalls].Y&&position->z==ExpectedPositions[APICalls].Z);++APICalls;ThrowIf(2);}
extern "C" void __stdcall CODA_Commit(){assert(Depth==0);++Tails;}
template<class T>void PKCTRList<T>::MorphToArray(){abort();}
template<class T>void PKCTRList<T>::MorphToTree(){abort();}
template<class T>T*PKCTRList<T>::Find(const T*){abort();return 0;}
template<class T>void PKCTRList<T>::Remove(T*){abort();}
template<class T>void PKCTRList<T>::Add(T*){abort();}
template<class T>void PKCTRList<T>::ForEach(void*){abort();}
template<class T>T*PKCTRList<T>::GetFirst(){abort();return 0;}
template<class T>T*PKCTRList<T>::GetLast(){abort();return 0;}
template<class T>T*PKCTRList<T>::GetItemByIndex(long){abort();return 0;}
template<class T>void PKCTRList<T>::Reserve(long){abort();}
template<class T>long PKCTRList<T>::GetCount(){abort();return 0;}
CSoundEventList::CSoundEventList():PKCTRList<CSoundEvent>(0){}
CSoundEventSet::CSoundEventSet():PKCTRList<CPooledResource>(0){}
template<class T>T*PKCFrallocStatic<T>::Get(unsigned long i){assert(Depth==1&&i<SlotCount);assert((void*)this==(void*)&Manager->m_events.Playbacks);assert(i==(unsigned long)PoolCalls);++PoolCalls;LastIndex=i;return m_pData+i;}
static C3DVector Oracle(const CSoundPosition&event){
 C3DVector source=Swizzle(event.Position),p=Swizzle(Manager->ListenerPosition),front=Swizzle(Manager->ListenerFront),top=Swizzle(Manager->ListenerTop),right=CrossValue(top,front);
 if(event.Relative)return V(p.X+right.X*source.X+top.X*source.Y+front.X*source.Z,p.Y+right.Y*source.X+top.Y*source.Y+front.Y*source.Z,p.Z+right.Z*source.X+top.Z*source.Y+front.Z*source.Z);
 // Reflect only the half-space behind the listener, avoiding the production's two matrix transforms.
 C3DVector delta=V(source.X-p.X,source.Y-p.Y,source.Z-p.Z);float distance=DotValue(delta,front);if(distance<0)return V(source.X-2*distance*front.X,source.Y-2*distance*front.Y,source.Z-2*distance*front.Z);return source;
}
static void Run(bool enabled,bool valid,unsigned long count,int frame,int seed,int pattern,int throwat){
 CUnprovenSoundWorld_00c1c460 world;CSoundEventManager manager;CPlaybackSlot slots[8];CSoundEvent events[8];long opaqueHandles[8];
 World=&world;Manager=&manager;Slots=slots;SlotCount=count;ThrowAt=throwat;Depth=Enters=Leaves=PoolCalls=HolderCalls=Copies=APICalls=InverseCalls=CrossCalls=DotCalls=SwapCalls=Tails=0;ExpectedCount=0;LastIndex=0;
 world.PManager=&manager;world.Paused=true;world.SourcePositionsEnabled=enabled;manager.ListenerValid=valid;manager.ListenerPosition=V((float)(seed-2),(float)(seed+1),(float)(1-seed));
 C3DVector axes[]={V(1,0,0),V(-1,0,0),V(0,1,0),V(0,-1,0),V(0,0,1),V(0,0,-1)};
 manager.ListenerFront=axes[frame];manager.ListenerTop=axes[frame<2?4:frame<4?0:2];manager.m_events.Playbacks.m_count=count;manager.m_events.Playbacks.m_pData=slots;
 for(unsigned long i=0;i<8;++i){int flags=(pattern+(int)i)%8;slots[i].PEvent=(flags&1)?&events[i]:0;events[i].PPlayingData=(flags&2)?(void*)&opaqueHandles[i]:0;slots[i].Positional=(flags&4)!=0;slots[i].SourceHandle=&opaqueHandles[i];events[i].PositionHolder.Value.Position=V((float)((int)i*3-seed*4),(float)(seed*5-(int)i),(float)((int)i*2-seed*3));events[i].PositionHolder.Value.Relative=((pattern+(int)i)&1)!=0;
  if(i<count&&slots[i].PEvent&&events[i].PPlayingData&&slots[i].Positional){ExpectedIndices[ExpectedCount]=i;ExpectedPositions[ExpectedCount]=Oracle(events[i].PositionHolder.Value);++ExpectedCount;}
 }
#ifdef _MSC_VER
 assert(sizeof(CPlaybackSlot)==0x2c&&sizeof(CSoundPosition)==16);assert((char*)&manager.ListenerPosition-(char*)&manager==8);assert((char*)&manager.ListenerFront-(char*)&manager==0x14);assert((char*)&manager.ListenerTop-(char*)&manager==0x20);assert((char*)&manager.m_events.Playbacks-(char*)&manager==0x224);assert((char*)&manager.ListenerValid-(char*)&manager==0x885);assert((char*)&events[0].PositionHolder-(char*)&events[0]==0x38);assert((char*)&events[0].PPlayingData-(char*)&events[0]==0x48);
#endif
#ifndef _MSC_VER
 try{world.UpdateSourcePositions();}catch(int n){assert(n==throwat&&Depth==0&&Tails==0);if(n==1)assert(Enters==0&&Leaves==0);else assert(Enters==1&&Leaves==1&&APICalls==1);return;}
#else
 world.UpdateSourcePositions();
#endif
 assert(!throwat&&Depth==0&&Tails==1&&world.PManager==&manager&&world.Paused&&world.SourcePositionsEnabled==enabled&&manager.ListenerValid==valid&&manager.m_events.Playbacks.m_count==count);
 if(enabled&&valid){assert(Enters==1&&Leaves==1&&PoolCalls==(int)count&&HolderCalls==(int)ExpectedCount&&APICalls==(int)ExpectedCount);assert(InverseCalls==1&&CrossCalls==1&&DotCalls==3&&SwapCalls==3+(int)ExpectedCount);}
 else assert(!Enters&&!Leaves&&!PoolCalls&&!HolderCalls&&!Copies&&!APICalls&&!InverseCalls&&!CrossCalls&&!DotCalls&&!SwapCalls);
}
int main(){unsigned long counts[]={0,1,3,4,7,8};long cases=0;for(int e=0;e<2;++e)for(int v=0;v<2;++v)for(int c=0;c<6;++c)for(int f=0;f<6;++f)for(int seed=0;seed<7;++seed)for(int p=0;p<8;++p){Run(e!=0,v!=0,counts[c],f,seed,p,0);++cases;}assert(cases==8064);
#ifndef _MSC_VER
 Run(true,true,8,0,3,7,1);Run(true,true,8,0,3,7,2);
#endif
 puts("01212472_TEST PASS");return 0;}
