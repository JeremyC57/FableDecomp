// Behaviour fixture: authentic names and the selected function body, with instrumented dependencies.
#include <cstdio>
#include <vector>
#include <cassert>
class CIEngine { public: int id; };
class CEnginePrimitivePrepareInfo { public: CIEngine* Engine; };
class CEnginePrimitiveHandle { public:};
static int events[64],eventCount,clockCalls,defCalls,backgroundCalls;
static double clocks[2];
static float expectedStep,backgroundAlpha;
static CIEngine* expectedEngine;
static void Event(int n){assert(eventCount<64);events[eventCount++]=n;}
static double GFGetTime(){assert(clockCalls<2);Event(1);return clocks[clockCalls++];}
namespace NUISystem {
class CComponent {
public:
    int id;
    virtual ~CComponent(){}
    virtual void Update(float dt){assert(dt==expectedStep);Event(100+id);}
    virtual void Draw(CIEngine& e,CEnginePrimitiveHandle* h,long n,long* indices,CComponent* parent)
    {assert(&e==expectedEngine&&h==0&&n==0&&indices==0&&parent==0);Event(200+id);}
};
class CUIMiscThingsDef {
public: int IVRefCount;float TimeInSecsForFade;int destroys;
    void DeleteThis(){++destroys;}
};
}
template<class T> class CDefPointer {
    T* Object;
public:
    explicit CDefPointer(T* p):Object(p){if(Object)++Object->IVRefCount;}
    CDefPointer(const CDefPointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}
    ~CDefPointer(){if(Object&&--Object->IVRefCount==0)Object->DeleteThis();}
    T* operator->()const{return Object;}
};
namespace NUISystem {
static CUIMiscThingsDef settings;
class CManager {
public:
    static CManager* GetInstance(){static CManager m;Event(2);return &m;}
    virtual CDefPointer<CUIMiscThingsDef> GetMiscThingsDef(){++defCalls;Event(3);return CDefPointer<CUIMiscThingsDef>(&settings);}
};
}
class CUserProfileManager {
public:
    static CUserProfileManager& Get(){static CUserProfileManager m;Event(4);return m;}
    void RenderUIBackground(float a){++backgroundCalls;backgroundAlpha=a;Event(5);}
};
class CTCQuestCompletionUI {
public:
    bool Active,Quitting;
    double LastRenderTime;
    float AccumFadeTime;
    NUISystem::CComponent* PBlack;
    std::vector<NUISystem::CComponent*> Components;
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};
void CTCQuestCompletionUI::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if(Active&&!Quitting){
        float time_step=float(GFGetTime()-LastRenderTime);
        LastRenderTime=GFGetTime();
        PBlack->Update(time_step);
        PBlack->Draw(*info.Engine,0,0,0,0);
        for(unsigned long count=0;count<Components.size();++count){
            Components[count]->Update(time_step);
            Components[count]->Draw(*info.Engine,0,0,0,0);
        }
        AccumFadeTime+=time_step;
        float lerp=AccumFadeTime/NUISystem::CManager::GetInstance()->GetMiscThingsDef()->TimeInSecsForFade;
        if(lerp>1.0f)lerp=1.0f;
        CUserProfileManager::Get().RenderUIBackground(lerp);
    }
}

static void Reset(){eventCount=clockCalls=defCalls=backgroundCalls=0;backgroundAlpha=-99.0f;}
int main()
{
    CIEngine engine;engine.id=7;expectedEngine=&engine;
    CEnginePrimitivePrepareInfo info;info.Engine=&engine;
    NUISystem::CComponent black,parts[4];black.id=0;
    for(int j=0;j<4;++j)parts[j].id=j+1;
    const float deltas[]={-1.0f,0.0f,0.5f,2.0f};
    const float fades[]={0.5f,1.0f,2.0f};
    const float accumulations[]={0.0f,0.5f,2.0f};
    for(int active=0;active<2;++active)for(int quitting=0;quitting<2;++quitting)
    for(int count=0;count<=4;++count)for(int prior=1;prior<=3;++prior)
    for(int delta=0;delta<4;++delta)for(int accum=0;accum<3;++accum)
    for(int fade=0;fade<3;++fade)for(int retained=0;retained<2;++retained){
        CTCQuestCompletionUI ui;ui.Active=active!=0;ui.Quitting=quitting!=0;
        ui.LastRenderTime=double(prior);ui.AccumFadeTime=accumulations[accum];ui.PBlack=&black;
        for(int j=0;j<count;++j)ui.Components.push_back(&parts[j]);
        NUISystem::settings.IVRefCount=retained;NUISystem::settings.destroys=0;
        NUISystem::settings.TimeInSecsForFade=fades[fade];
        expectedStep=deltas[delta];clocks[0]=double(prior)+expectedStep;clocks[1]=clocks[0]+0.25;
        Reset();ui.DrawPreparePrimitivesForRendering(info);
        if(!active||quitting){
            assert(eventCount==0&&ui.LastRenderTime==double(prior)&&ui.AccumFadeTime==accumulations[accum]);
            assert(NUISystem::settings.destroys==0);
        }else{
            assert(clockCalls==2&&defCalls==1&&backgroundCalls==1&&ui.LastRenderTime==clocks[1]);
            assert(ui.AccumFadeTime==accumulations[accum]+expectedStep);
            float expected=(accumulations[accum]+expectedStep)/fades[fade];if(expected>1.0f)expected=1.0f;
            assert(backgroundAlpha==expected);
            int pos=0;assert(events[pos++]==1);assert(events[pos++]==1);
            assert(events[pos++]==100);assert(events[pos++]==200);
            for(int j=0;j<count;++j){assert(events[pos++]==101+j);assert(events[pos++]==201+j);}
            assert(events[pos++]==2);assert(events[pos++]==3);assert(events[pos++]==4);assert(events[pos++]==5);
            assert(pos==eventCount);
            assert(NUISystem::settings.destroys==(retained?0:1));
        }
        assert(NUISystem::settings.IVRefCount==retained);
    }
    std::puts("005e3f00_TEST PASS");return 0;
}