#ifndef _MSC_VER
#define _STLP_NO_EXCEPTIONS 1
#define _STLP_NATIVE_HEADER(h) _STLP_MAKE_HEADER(/usr/include/c++/14,h)
#define _STLP_NATIVE_C_HEADER(h) _STLP_MAKE_HEADER(native_crt,h)
#define _STLP_NATIVE_CPP_C_HEADER(h) _STLP_MAKE_HEADER(/usr/include/c++/14,h)
#define _STLP_NATIVE_CPP_RUNTIME_HEADER(h) _STLP_MAKE_HEADER(/usr/include/c++/14,h)
#endif
#define _STLP_NO_OWN_IOSTREAMS 1
#define _STLP_NO_OWN_NAMESPACE 1
#define _STLP_USE_STATIC_LIB 1
#define _STLP_DEFAULT_CONSTRUCTOR_BUG 1
#define FABLE_RETAIL_STLPORT_DIRECT_MALLOC_ALLOCATOR 1
#define FABLE_STLPORT_ALLOCATE_BYTES(n) malloc(n)
#define FABLE_STLPORT_DEALLOCATE_BYTES(p,n) free(p)
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_GETEXTENTS
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_compiler.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsF.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/GFDebugPDynamicCast.h"
#include <map>
#include <vector>
static long gComponentsCreated,gComponentsDeleted,gDefsCreated,gDefsDeleted,gScaleCalls,gBaseUpdates;
static C2DBoxF gExpectedBounds;static unsigned gCallbackCount;static float gLastTick;static float gScaleX;
static bool sameVector(const C2DVector&a,const C2DVector&b){return a.X==b.X&&a.Y==b.Y;}
namespace NUISystem {
// Controlled original external interfaces. All template/container/value bodies above are real shared/vendor code.
class CUIDef:public CDefPointeeBase {public:C2DVector PositionOffset;CUIDef():PositionOffset(0,64){++gDefsCreated;}virtual~CUIDef(){++gDefsDeleted;}};
class CComponent {
public:
 C2DVector RenderPos;bool Allowed;float UpOffset,DownOffset;mutable unsigned RenderCalls;unsigned UpCalls,DownCalls;mutable bool PendingRender;
 CDefPointer<const CUIDef> Def;std::map<unsigned long,CUIState>States;
 CComponent():RenderPos(0,0),Allowed(false),UpOffset(0),DownOffset(0),RenderCalls(0),UpCalls(0),DownCalls(0),PendingRender(false){++gComponentsCreated;}
 virtual~CComponent(){++gComponentsDeleted;}
 virtual const C2DVector&GetRenderPosition()const{++RenderCalls;PendingRender=true;return RenderPos;}
 virtual bool IsAllowedToScroll()const{return Allowed;}
 virtual float CanScroll(bool up,C2DBoxF&bounds){assert(PendingRender);PendingRender=false;assert(bounds.TLX==gExpectedBounds.TLX+(float)gCallbackCount&&bounds.TLY==gExpectedBounds.TLY&&bounds.BRX==gExpectedBounds.BRX+(float)gCallbackCount&&bounds.BRY==gExpectedBounds.BRY);bounds.TLX+=1;bounds.BRX+=1;++gCallbackCount;if(up){++UpCalls;return UpOffset;}++DownCalls;return DownOffset;}
 virtual void ChangeZoom(const C2DVector&,float,bool){}
 virtual void ChangePosition(const C2DVector&,float,bool){}
 virtual std::map<unsigned long,CUIState>&GetStates(){return States;}
 virtual CDefPointer<const CUIDef>LoadAndReturnDef()const{return Def;}
};
class CDraggable:public CComponent {
public:bool Dragged;unsigned DragCalls,BeforeCalls;C2DVector Before;
 CDraggable():Dragged(false),DragCalls(0),BeforeCalls(0),Before(0,0){}
 virtual bool IsDragged(){++DragCalls;return Dragged;}
 virtual void SetPositionBeforeDrag(C2DVector&p){++BeforeCalls;Before=p;}
};
class CScrollBar:public CDraggable {
public:unsigned ZoomCalls,PositionCalls,DefCalls,UpDeltaCalls,DownDeltaCalls,UpMaxCalls,DownMaxCalls;C2DVector LastZoom,LastPosition,UpDelta,DownDelta;unsigned long UpMax,DownMax;bool ChangeDefOnZoom;
 CScrollBar():ZoomCalls(0),PositionCalls(0),DefCalls(0),UpDeltaCalls(0),DownDeltaCalls(0),UpMaxCalls(0),DownMaxCalls(0),LastZoom(0,0),LastPosition(0,0),UpDelta(0,0),DownDelta(0,0),UpMax(999),DownMax(999),ChangeDefOnZoom(false){}
 void ChangeZoom(const C2DVector&z,float time,bool linear){assert(time==0&&!linear);++ZoomCalls;LastZoom=z;if(ChangeDefOnZoom)const_cast<CUIDef*>(Def.Get())->PositionOffset.Y*=2;}
 void ChangePosition(const C2DVector&p,float time,bool linear){assert(time==0.2f&&!linear);++PositionCalls;LastPosition=p;}
 CDefPointer<const CUIDef>LoadAndReturnDef()const{++const_cast<CScrollBar*>(this)->DefCalls;return Def;}
 virtual void SetDeltaForActionUp(C2DVector&v){++UpDeltaCalls;UpDelta=v;}
 virtual void SetDeltaForActionDown(C2DVector&v){++DownDeltaCalls;DownDelta=v;}
 virtual void SetMaxAmountActionsUp(unsigned long n){++UpMaxCalls;UpMax=n;}
 virtual void SetMaxAmountActionsDown(unsigned long n){++DownMaxCalls;DownMax=n;}
};
class CViewport:public CComponent {
public:std::vector<CCountedPointer<CComponent> >Children;std::vector<CComponent*>FakeChildren;
 virtual void Update(float tick){++gBaseUpdates;gLastTick=tick;}
};
class CScrollingViewport:public CViewport {
public:C2DBoxF Window;CComponent*PScrollBar;CComponent*PScrollBarOutside;bool Changed;float OffsetToUseAbove,OffsetToUseUnder;
 CScrollingViewport():PScrollBar(0),PScrollBarOutside(0),Changed(false),OffsetToUseAbove(0),OffsetToUseUnder(0){}
 void Update(float);
};
class CManager {public:static CManager*__fastcall GetInstance();C2DVector GetUIScale()const;};
}

using namespace NUISystem;
void CScrollingViewport::Update(float tick)
{
    C2DBoxF window_to_use;
    window_to_use.TLX = Window.GetTLX() * CManager::GetInstance()->GetUIScale().X + GetRenderPosition().X;
    window_to_use.TLY = Window.GetTLY() * CManager::GetInstance()->GetUIScale().Y + GetRenderPosition().Y;
    window_to_use.BRX = Window.GetExtents().X * CManager::GetInstance()->GetUIScale().X + window_to_use.TLX;
    window_to_use.BRY = Window.GetExtents().Y * CManager::GetInstance()->GetUIScale().Y + window_to_use.TLY;
    float offset_to_use_above = 0.0f;
    float offset_to_use_under = 0.0f;
    bool viewport_inactive = true;
    if (false) {} else for (unsigned long count = 0; count < Children.size(); ++count)
    {
        if (Children[count]->IsAllowedToScroll() && Children[count].Get() != PScrollBar && Children[count].Get() != PScrollBarOutside)
        {
            C2DVector position = Children[count]->GetRenderPosition();
            float allowed_offset = Children[count]->CanScroll(true, window_to_use);
            if (fabs(offset_to_use_above) < fabs(allowed_offset))
                offset_to_use_above = allowed_offset;
            viewport_inactive = false;
        }
    }
    if (false) {} else for (unsigned long count = 0; count < FakeChildren.size(); ++count)
    {
        if (FakeChildren[count]->IsAllowedToScroll())
        {
            C2DVector position = FakeChildren[count]->GetRenderPosition();
            float allowed_offset = FakeChildren[count]->CanScroll(true, window_to_use);
            if (fabs(offset_to_use_above) < fabs(allowed_offset))
                offset_to_use_above = allowed_offset;
            viewport_inactive = false;
        }
    }
    if (false) {} else for (unsigned long count = 0; count < Children.size(); ++count)
    {
        if (Children[count]->IsAllowedToScroll() && Children[count].Get() != PScrollBar && Children[count].Get() != PScrollBarOutside)
        {
            C2DVector position = Children[count]->GetRenderPosition();
            float allowed_offset = Children[count]->CanScroll(false, window_to_use);
            if (fabs(offset_to_use_under) < fabs(allowed_offset))
                offset_to_use_under = allowed_offset;
        }
    }
    if (false) {} else for (unsigned long count = 0; count < FakeChildren.size(); ++count)
    {
        if (FakeChildren[count]->IsAllowedToScroll())
        {
            C2DVector position = FakeChildren[count]->GetRenderPosition();
            float allowed_offset = FakeChildren[count]->CanScroll(false, window_to_use);
            if (fabs(offset_to_use_under) < fabs(allowed_offset))
                offset_to_use_under = allowed_offset;
        }
    }
    if (OffsetToUseAbove != offset_to_use_above || OffsetToUseUnder != offset_to_use_under)
    {
        Changed = true;
        OffsetToUseUnder = offset_to_use_under;
        OffsetToUseAbove = offset_to_use_above;
    }
    if (PScrollBar)
    {
        C2DVector zoom(1.0f, 1.0f);
        float visible = window_to_use.GetBRY() - window_to_use.GetTLY();
        float invisible_above = offset_to_use_under;
        float invisible = -offset_to_use_above + offset_to_use_under;
        float invisible_under = offset_to_use_above;
        zoom.Y = visible / (visible + invisible);
        if (viewport_inactive)
            zoom.Y = 0.0f;
        PScrollBar->ChangeZoom(zoom, 0.0f, false);
        C2DVector position = PScrollBar->GetStates()[UI_STATE_INACTIVE].Position;
        float delta_size = PScrollBar->LoadAndReturnDef()->PositionOffset.Y - zoom.Y * PScrollBar->LoadAndReturnDef()->PositionOffset.Y;
        if (invisible > 0.0f)
            position.Y += (invisible_above / invisible) * delta_size;
        CScrollBar* pscroll_bar = GFDebugPDynamicCast(PScrollBar, (CScrollBar*)0);
        if (pscroll_bar && Changed)
        {
            if (offset_to_use_under == 0.0f || offset_to_use_above == 0.0f || !pscroll_bar->IsDragged())
            {
                PScrollBar->ChangePosition(position, 0.2f, false);
                pscroll_bar->SetPositionBeforeDrag(position);
            }
        }
        if (invisible > 0.0f)
        {
            C2DVector delta_action(0.0f, 1.0f * PScrollBar->LoadAndReturnDef()->PositionOffset.Y / (visible + invisible));
            C2DVector delta_action_null(-1.0f, -1.0f);
            CScrollBar* pscroll_bar2 = GFDebugPDynamicCast(PScrollBar, (CScrollBar*)0);
            if (pscroll_bar2)
            {
                if (offset_to_use_under > 0.0f)
                {
                    pscroll_bar2->SetDeltaForActionUp(delta_action);
                    pscroll_bar2->SetMaxAmountActionsUp((unsigned long)offset_to_use_under);
                }
                else
                {
                    pscroll_bar2->SetDeltaForActionUp(delta_action_null);
                    pscroll_bar2->SetMaxAmountActionsUp(0);
                }
                if (offset_to_use_above < 0.0f)
                {
                    pscroll_bar2->SetDeltaForActionDown(delta_action);
                    pscroll_bar2->SetMaxAmountActionsDown((unsigned long)-offset_to_use_above);
                }
                else
                {
                    pscroll_bar2->SetDeltaForActionDown(delta_action_null);
                    pscroll_bar2->SetMaxAmountActionsDown(0);
                }
            }
        }
    }
    Changed = false;
    CViewport::Update(tick);
}

CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CResource::~CResource(){assert(IVRefCount==0);}
void CIVCountedPointeeBase::Delete()const{delete this;}
bool CResource::UnloadResource(){return false;}
unsigned long CResource::GetUnloadableMemory()const{return 0;}
bool CResource::IsResourceLoaded()const{return true;}
void CResource::OnResourceLoaded(){}
bool CResource::CanAbortResourceLoad(long)const{return false;}
void CResource::OnResourceLoadAborted(){}
unsigned long CResource::GetDebugMemoryInUse()const{return 0;}
void CResource::ResetDebugMemoryInUse(){}
static CManager gManager;
CManager*__fastcall CManager::GetInstance(){return &gManager;}
C2DVector CManager::GetUIScale()const{++gScaleCalls;return C2DVector(gScaleX,1);}
CUIState::CUIState(){Zoom.X=Zoom.Y=1;Position.X=Position.Y=0;Colour.R=Colour.G=Colour.B=Colour.A=255;UpdateTime=-1;StateChangeType=STATE_CHANGE_SIMULTANEOUS;StateChangeFlag=7;LinearChange=false;}
CUIState::CUIState(const CUIState&o):StateChangeFlag(o.StateChangeFlag),StateChangeType(o.StateChangeType),Position(o.Position),Zoom(o.Zoom),Colour(o.Colour),UpdateTime(o.UpdateTime),LinearChange(o.LinearChange){}
struct Record{CComponent*p;bool fake;};
static void put(CScrollingViewport&v,const std::vector<CCountedPointer<CComponent> >&owners,Record*records,unsigned&n,CComponent*p,bool fake){records[n].p=p;records[n++].fake=fake;if(fake)v.FakeChildren.push_back(p);else {unsigned k=0;while(k<owners.size()&&owners[k].Get()!=p)++k;assert(k<owners.size());v.Children.push_back(owners[k]);}}
static float absolute(float x){return x<0?-x:x;}
int main(){
 unsigned cases=0;
 for(unsigned plan=0;plan<7;++plan)for(unsigned bp=0;bp<2;++bp)for(unsigned op=0;op<2;++op)for(unsigned drag=0;drag<2;++drag)for(unsigned cp=0;cp<4;++cp)for(unsigned pattern=0;pattern<8;++pattern)for(unsigned active=0;active<2;++active)for(unsigned key=0;key<2;++key)for(unsigned effect=0;effect<2;++effect)for(unsigned sx=0;sx<2;++sx){
  long made=gComponentsCreated,deleted=gComponentsDeleted,defs=gDefsCreated,deaddefs=gDefsDeleted;
  {
   CDefPointer<const CUIDef>def;def.Object=new CUIDef;std::vector<CCountedPointer<CComponent> >owners;CScrollingViewport v;
   CScrollBar*bar=new CScrollBar;CComponent*outside=new CComponent;CComponent*a=new CComponent;CComponent*b=new CComponent;
   CComponent*all[4]={bar,outside,a,b};
   for(unsigned k=0;k<4;++k){owners.push_back(CCountedPointer<CComponent>(all[k]));all[k]->Def=def;all[k]->Allowed=active!=0;}
   static const float ups[7]={0,0,-64,-32,-96,32,0};static const float downs[7]={0,64,0,32,96,0,-32};
   for(unsigned k=0;k<4;++k){all[k]->UpOffset=ups[plan];all[k]->DownOffset=downs[plan];}
   v.PScrollBar=bp?bar:0;v.PScrollBarOutside=op?outside:0;v.Changed=(cp&1)!=0;v.RenderPos=C2DVector(8,16);v.Window=C2DBoxF(0,0,64,64);gScaleX=sx?2.0f:1.0f;bar->Dragged=drag!=0;bar->ChangeDefOnZoom=effect!=0;
   if(key)bar->States[UI_STATE_INACTIVE].Position=C2DVector(4,8);
   Record records[16];unsigned n=0;
   if(pattern==1)put(v,owners,records,n,a,false);
   if(pattern==2)put(v,owners,records,n,a,true);
   if(pattern==3){put(v,owners,records,n,bar,false);put(v,owners,records,n,outside,false);}
   if(pattern==4){put(v,owners,records,n,b,false);put(v,owners,records,n,a,false);}
   if(pattern==5){put(v,owners,records,n,b,true);put(v,owners,records,n,a,false);}
   if(pattern==6){put(v,owners,records,n,a,false);put(v,owners,records,n,a,true);put(v,owners,records,n,b,false);}
   if(pattern==7){put(v,owners,records,n,bar,true);put(v,owners,records,n,outside,true);put(v,owners,records,n,a,false);}
   float above=0,under=0;unsigned eligible=0,per[4]={0,0,0,0};
   for(unsigned k=0;k<n;++k){CComponent*p=records[k].p;bool qualifies=p->Allowed&&(records[k].fake||(p!=v.PScrollBar&&p!=v.PScrollBarOutside));if(!qualifies)continue;++eligible;for(unsigned q=0;q<4;++q)if(all[q]==p)++per[q];if(absolute(p->UpOffset)>absolute(above))above=p->UpOffset;if(absolute(p->DownOffset)>absolute(under))under=p->DownOffset;}
   if(cp&2){v.OffsetToUseAbove=above;v.OffsetToUseUnder=under;}else{v.OffsetToUseAbove=17;v.OffsetToUseUnder=19;}
   bool changed=v.Changed||v.OffsetToUseAbove!=above||v.OffsetToUseUnder!=under;float range=under-above;float zoom=eligible?64.0f/(64.0f+range):0.0f;float defY=effect&&bp?128.0f:64.0f;float length=defY-zoom*defY;C2DVector expectedPosition(key?4.0f:0.0f,key?8.0f:0.0f);if(range>0)expectedPosition.Y+=(under/range)*length;
   unsigned refs=def.Object->IVRefCount;gExpectedBounds=C2DBoxF(8,16,8+64*gScaleX,80);gCallbackCount=0;gScaleCalls=gBaseUpdates=0;
   v.Update(0.25f);
   assert(gScaleCalls==4&&gBaseUpdates==1&&gLastTick==0.25f&&!v.Changed&&v.OffsetToUseAbove==above&&v.OffsetToUseUnder==under);assert(gCallbackCount==eligible*2&&v.RenderCalls==2);assert(def.Object->IVRefCount==refs);
   for(unsigned k=0;k<4;++k)assert(all[k]->RenderCalls==per[k]*2&&all[k]->UpCalls==per[k]&&all[k]->DownCalls==per[k]&&!all[k]->PendingRender);
   if(!bp){assert(!bar->ZoomCalls&&!bar->PositionCalls&&!bar->BeforeCalls&&!bar->DefCalls&&!bar->DragCalls&&!bar->UpDeltaCalls&&!bar->DownDeltaCalls);}
   else{
    assert(bar->ZoomCalls==1&&sameVector(bar->LastZoom,C2DVector(1,zoom)));assert(bar->DefCalls==(range>0?3u:2u));bool move=changed&&(under==0||above==0||!drag);assert(bar->PositionCalls==(move?1u:0u)&&bar->BeforeCalls==(move?1u:0u));assert(bar->DragCalls==(changed&&under!=0&&above!=0?1u:0u));if(move){assert(sameVector(bar->LastPosition,expectedPosition)&&sameVector(bar->Before,expectedPosition));}
    if(range>0){C2DVector delta(0,defY/(64+range)),none(-1,-1);assert(bar->UpDeltaCalls==1&&bar->DownDeltaCalls==1&&bar->UpMaxCalls==1&&bar->DownMaxCalls==1);assert(sameVector(bar->UpDelta,under>0?delta:none)&&sameVector(bar->DownDelta,above<0?delta:none));assert(bar->UpMax==(under>0?(unsigned long)under:0)&&bar->DownMax==(above<0?(unsigned long)-above:0));}
    else assert(!bar->UpDeltaCalls&&!bar->DownDeltaCalls&&!bar->UpMaxCalls&&!bar->DownMaxCalls);
   }
  }
  if(gComponentsCreated-made!=gComponentsDeleted-deleted||gDefsCreated-defs!=gDefsDeleted-deaddefs)fprintf(stderr,"lifetime case %u: component %ld/%ld def %ld/%ld\n",cases,gComponentsCreated-made,gComponentsDeleted-deleted,gDefsCreated-defs,gDefsDeleted-deaddefs);
  assert(gComponentsCreated-made==gComponentsDeleted-deleted&&gDefsCreated-defs==gDefsDeleted-deaddefs);++cases;
 }
 assert(cases==28672);
 unsigned treeCases=0;
 for(unsigned permutation=0;permutation<120;++permutation){
  unsigned pool[5]={0,1,2,3,4},order[5],used=5,x=permutation;
  for(unsigned i=0;i<5;++i){unsigned index=x%used;x/=used;order[i]=pool[index];for(unsigned j=index+1;j<used;++j)pool[j-1]=pool[j];--used;}
  std::map<unsigned long,CUIState>m;bool present[7]={false,false,false,false,false,false,false};float values[7]={0,0,0,0,0,0,0};
  for(unsigned i=0;i<5;++i){unsigned k=order[i];m[k].Position=C2DVector((float)k,(float)(k*4));present[k]=true;values[k]=(float)(k*4);for(unsigned q=0;q<7;++q){std::map<unsigned long,CUIState>::iterator f=m.find(q);assert((f!=m.end())==present[q]);if(present[q])assert(f->second.Position.Y==values[q]);}++treeCases;}
  CUIState extra;extra.Position=C2DVector(6,24);m.insert(m.begin(),std::pair<const unsigned long,CUIState>(6,extra));m.insert(m.end(),std::pair<const unsigned long,CUIState>(5,extra));m.insert(m.begin(),std::pair<const unsigned long,CUIState>(3,extra));assert(m.size()==7);unsigned prev=0,count=0;for(std::map<unsigned long,CUIState>::iterator it=m.begin();it!=m.end();++it){if(count)assert(it->first>prev);prev=it->first;++count;}assert(count==7&&m[3].Position.Y==12);++treeCases;
 }
 assert(treeCases==720);printf("0053f1c0_TEST PASS\n");return 0;
}