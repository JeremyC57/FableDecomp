// Exact target and original colour-copy/refcount operations; controlled external services.
#include <stdio.h>
#include <list>
#define FABLE_RETAIL_CRGBCOLOUR
#ifndef FABLE_ENGINE_CRGBCOLOUR_H
#define FABLE_ENGINE_CRGBCOLOUR_H

// Layout: original PDB class V, sizeof4; native B,G,R,A at0,1,2,3.
class CRGBColour {
public:
    union {
        struct { unsigned char B,G,R,A; };
        unsigned long IntValue;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
#ifdef FABLE_RETAIL_CRGBCOLOUR
    // Retail opt-in (chunk 724): the PDB's Initialise(r, g, b, a) (?Initialise@CRGBColour@@QAEXEEEE@Z) and the copy constructor
    // built on it: all four channel bytes are read (as Initialise's arguments) before any is stored, the byte-by-byte copy
    // retail emits where a CRGBColour is passed by value (CPSCDecalRenderer::InterpolateTo 0x00B1BDA0 into
    // GFGetInterpolatedColour; GOTCHAS: the landed rows spell the setter `Set`, which neither PDB has).
    void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
    CRGBColour(const CRGBColour& colour) { Initialise(colour.R,colour.G,colour.B,colour.A); }
    // Codex pass 13 (rows 0x00EEDC20 / 0x00E8BEF0): FableWin ??0CRGBColour@@QAE@EEE@Z, inline in retail (R, G, B then A = 0xFF stored
    // in order through Initialise).
    CRGBColour(unsigned char r,unsigned char g,unsigned char b) { Initialise(r,g,b,255); }
#else
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
#endif
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
};

#endif

static int load_calls,delete_calls,click_calls,state_calls,action_calls;
static float expected_tick;
static unsigned char next_alpha;
class CIVCountedPointeeBase {
public:
    mutable unsigned long IVRefCount;
    virtual ~CIVCountedPointeeBase(){}
    virtual void Delete()const{++delete_calls;}
    void AddRef()const{++IVRefCount;}
    long Release()const{if(--IVRefCount==0){Delete();return 0;}return IVRefCount;}
};
class CDefPointeeBase:public CIVCountedPointeeBase{};
template<class T>class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer():Object(0){}
    CDefPointer(const CDefPointer& other):Object(other.Object){if(Object)Object->AddRef();}
    ~CDefPointer(){Release();}
    T* Get()const{return static_cast<T*>(Object);}
    T* operator->()const{return Get();}
    void Release(){if(Object){Object->Release();Object=0;}}
};
template<class T>class CCountedPointer { public:T* Data; };
namespace NUISystem {
class CAction{};
class CUIDef:public CDefPointeeBase {public:bool PCStyle;};
class CComponent {
public:
    CRGBColour RenderColour;
    CUIDef* TestDef;
    std::list<CCountedPointer<CAction> >* ExpectedActions;
    virtual ~CComponent(){}
    virtual CDefPointer<const CUIDef> LoadAndReturnDef()const {
        ++load_calls;CDefPointer<const CUIDef> result;result.Object=TestDef;return result;
    }
    virtual void ExecuteActions(std::list<CCountedPointer<CAction> >* actions){
        if(actions!=ExpectedActions||click_calls+state_calls!=1)action_calls+=1000;
        ++action_calls;
    }
};
class CChangingStateComponent:public CComponent {
public:
    virtual void Update(float tick){++state_calls;if(tick!=expected_tick)state_calls+=1000;RenderColour.A=next_alpha;RenderColour.R=17;RenderColour.G=18;RenderColour.B=19;}
};
class CClickable:public CChangingStateComponent {
public:
    virtual void Update(float tick){++click_calls;if(tick!=expected_tick)click_calls+=1000;RenderColour.A=next_alpha;RenderColour.R=31;RenderColour.G=32;RenderColour.B=33;}
};
class CMenuEntry:public CClickable {
public:
    std::list<CCountedPointer<CAction> >* PActionsOnVisible;
    virtual void Update(float tick);
};
}
void NUISystem::CMenuEntry::Update(float tick)
{
    CRGBColour old_render_colour=RenderColour;
    if(LoadAndReturnDef()->PCStyle)
        CClickable::Update(tick);
    else
        CChangingStateComponent::Update(tick);
    if(RenderColour.A && !old_render_colour.A)
        ExecuteActions(PActionsOnVisible);
}

int main(){
    unsigned cases=0;const unsigned char alpha[]={0,1,2,127,128,255};const float ticks[]={0.0f,0.25f,1.0f,2.0f};
    for(unsigned old=0;old<6;++old)for(unsigned now=0;now<6;++now)for(unsigned pc=0;pc<2;++pc)
    for(unsigned refs=1;refs<=3;++refs)for(unsigned ti=0;ti<4;++ti)for(unsigned has_actions=0;has_actions<2;++has_actions){
        NUISystem::CUIDef def;def.PCStyle=pc!=0;def.IVRefCount=refs;
        NUISystem::CMenuEntry entry;std::list<CCountedPointer<NUISystem::CAction> >actions;
        entry.TestDef=&def;entry.PActionsOnVisible=has_actions?&actions:0;entry.ExpectedActions=entry.PActionsOnVisible;
        entry.RenderColour.R=211;entry.RenderColour.G=77;entry.RenderColour.B=113;entry.RenderColour.A=alpha[old];
        next_alpha=alpha[now];expected_tick=ticks[ti];load_calls=delete_calls=click_calls=state_calls=action_calls=0;
        entry.Update(expected_tick);
        if(load_calls!=1||delete_calls!=(refs==1?1:0)||def.IVRefCount!=refs-1||click_calls!=(pc?1:0)||state_calls!=(pc?0:1)
            ||action_calls!=((alpha[old]==0&&alpha[now]!=0)?1:0)||entry.RenderColour.A!=alpha[now]
            ||entry.RenderColour.R!=(pc?31:17)||entry.RenderColour.G!=(pc?32:18)||entry.RenderColour.B!=(pc?33:19))return 1;
        ++cases;
    }
    if(cases!=1728)return 2;
    puts("0053d4b0_TEST PASS");return 0;
}