#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define FABLE_ENGINE_CSPRITE_H
#define FABLE_ENGINE_CFRONTENDMANAGER_H
#define FABLE_ENGINE_CGRAPHICDATABANK_H
#endif
// NUISystem::CSprite::Initialise() -- initialise component and fetch default graphic-frame dimensions into inherited size
// Leaf: Initialise@CSpriteInitialiseView
// Retail0041A8D0,176 bytes through ret0041A97F. Actual RTTI0122F5D4 slot43 isCSprite.
// Shared CSprite donor inherited Size+dc->retailcc and GraphicIndex+1a8->178;
// native three8-byte handle/angle/dimension groups follow changing-state base15c.
// Shared FrontEndManager/quarantinedGraphicDataBank retained; views expose missing
// inherited nativefields and graphics-bank width21/height22 calls. Base0052C730,
// singleton0041E5F2, each owning graphics-bank temporary hasfastcall Data deletion callback.
#include <new>
#include "engine/CSprite.h"
#include "engine/CFrontEndManager.h"
#include "engine/_quarantine/CGraphicDataBank.h"
struct CGraphicDataBankCalls {
    virtual ~CGraphicDataBankCalls() {}
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual unsigned long GetGraphicFrameWidth(unsigned long,unsigned long) const;
    virtual unsigned long GetGraphicFrameHeight(unsigned long,unsigned long) const;
};
struct CSpriteGraphicsInfo {
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CSpriteGraphicsPointer {
    CGraphicDataBankCalls* Data;CSpriteGraphicsInfo* Info;
    CSpriteGraphicsPointer():Data(0),Info(0) {}
    CSpriteGraphicsPointer(const CSpriteGraphicsPointer& p):Data(p.Data),Info(p.Info) {if(Info) ++Info->RefCount;}
    ~CSpriteGraphicsPointer() {if(Info && --Info->RefCount==0) {Info->DeleteFunc(Info->Data);::operator delete(Info);}}
    CGraphicDataBankCalls* operator->() const {return Data;}
};
struct CFrontEndManagerGraphicsView {
    unsigned char Base[0x10];
    CSpriteGraphicsPointer PGraphicsBank;
    static CFrontEndManagerGraphicsView* __fastcall GetInstance(); //0041E5F2
    CSpriteGraphicsPointer GetGraphicsBank() const {return PGraphicsBank;}
};
namespace NUISystem {
struct CChangingStateComponentInitialiseCalls {
    void* VTable;
    unsigned char BeforeSize[0xcc-4];
    float SizeX,SizeY;
    unsigned char BeforeGraphicIndex[0x178-0xd4];
    unsigned long GraphicIndex;
    void Initialise(); //0052C730
};
struct CSpriteInitialiseView : CChangingStateComponentInitialiseCalls {void Initialise();};
void CSpriteInitialiseView::Initialise() {
    CChangingStateComponentInitialiseCalls::Initialise();
    SizeX=(float)CFrontEndManagerGraphicsView::GetInstance()->GetGraphicsBank()->GetGraphicFrameWidth(GraphicIndex,0);
    SizeY=(float)CFrontEndManagerGraphicsView::GetInstance()->GetGraphicsBank()->GetGraphicFrameHeight(GraphicIndex,0);
}
}

using NUISystem::CSpriteInitialiseView;
static CSpriteInitialiseView* active;
static CFrontEndManagerGraphicsView* managers[2];
static CSpriteGraphicsInfo* infos[2];
static unsigned long dimensions[2],owners;
static float expectedSizes[2];
static int bad,baseCalls,getters,widths,heights,deletes,releaseOwner;
void CGraphicDataBankCalls::v01() {}
void CGraphicDataBankCalls::v02() {}
void CGraphicDataBankCalls::v03() {}
void CGraphicDataBankCalls::v04() {}
void CGraphicDataBankCalls::v05() {}
void CGraphicDataBankCalls::v06() {}
void CGraphicDataBankCalls::v07() {}
void CGraphicDataBankCalls::v08() {}
void CGraphicDataBankCalls::v09() {}
void CGraphicDataBankCalls::v10() {}
void CGraphicDataBankCalls::v11() {}
void CGraphicDataBankCalls::v12() {}
void CGraphicDataBankCalls::v13() {}
void CGraphicDataBankCalls::v14() {}
void CGraphicDataBankCalls::v15() {}
void CGraphicDataBankCalls::v16() {}
void CGraphicDataBankCalls::v17() {}
void CGraphicDataBankCalls::v18() {}
void CGraphicDataBankCalls::v19() {}
void CGraphicDataBankCalls::v20() {}

CFrontEndManagerGraphicsView* __fastcall CFrontEndManagerGraphicsView::GetInstance() {
    if(baseCalls!=1 || getters>=2) {bad=1;return managers[0];}
    if(getters==1 && (widths!=1 || active->SizeX!=expectedSizes[0] || active->GraphicIndex!=124)) bad=1;
    return managers[getters++];
}
void NUISystem::CChangingStateComponentInitialiseCalls::Initialise() {
    if(this!=active || baseCalls || getters || widths || heights) bad=1;
    ++baseCalls;GraphicIndex=123;
}
static void __fastcall deleted(void* data);
struct Bank : CGraphicDataBankCalls {
    int ID;
    Bank(int id):ID(id) {}
    virtual unsigned long GetGraphicFrameWidth(unsigned long graphic,unsigned long frame) const {
        ++widths;
        if(ID!=0 || graphic!=123 || frame || getters!=1 || heights || (infos[0] && infos[0]->RefCount!=owners+1)) bad=1;
        active->GraphicIndex=124;
        if(releaseOwner && infos[0]) {--infos[0]->RefCount;managers[0]->PGraphicsBank.Info=0;}
        return dimensions[0];
    }
    virtual unsigned long GetGraphicFrameHeight(unsigned long graphic,unsigned long frame) const {
        ++heights;
        if(ID!=1 || graphic!=124 || frame || getters!=2 || widths!=1 || (infos[1] && infos[1]->RefCount!=owners+1)) bad=1;
        if(releaseOwner && infos[1]) {--infos[1]->RefCount;managers[1]->PGraphicsBank.Info=0;}
        return dimensions[1];
    }
};
unsigned long CGraphicDataBankCalls::GetGraphicFrameWidth(unsigned long,unsigned long) const {bad=1;return 0;}
unsigned long CGraphicDataBankCalls::GetGraphicFrameHeight(unsigned long,unsigned long) const {bad=1;return 0;}
static void __fastcall deleted(void* data) {
    Bank* bank=static_cast<Bank*>(data);++deletes;
    if(bank->ID==0) {if(widths!=1 || heights || active->SizeX!=expectedSizes[0]) bad=1;}
    else if(heights!=1 || active->SizeY!=expectedSizes[1]) bad=1;
}
static int run(int nullInfo,int finalRelease,unsigned long refs,unsigned long width,unsigned long height) {
    CSpriteInitialiseView sprite;active=&sprite;
    CFrontEndManagerGraphicsView m0,m1;managers[0]=&m0;managers[1]=&m1;
    Bank b0(0),b1(1);CGraphicDataBankCalls* banks[2]={&b0,&b1};
    owners=refs;releaseOwner=finalRelease;dimensions[0]=width;dimensions[1]=height;expectedSizes[0]=(float)width;expectedSizes[1]=(float)height;
    for(int i=0;i<2;++i) {
        infos[i]=nullInfo?0:new CSpriteGraphicsInfo;
        if(infos[i]) {infos[i]->RefCount=refs;infos[i]->DeleteFunc=deleted;infos[i]->Data=banks[i];}
        managers[i]->PGraphicsBank.Data=banks[i];managers[i]->PGraphicsBank.Info=infos[i];
    }
    sprite.SizeX=-17;sprite.SizeY=-19;sprite.GraphicIndex=91;
    sprite.BeforeSize[0]=47;sprite.BeforeGraphicIndex[0]=53;
    bad=baseCalls=getters=widths=heights=deletes=0;
    sprite.Initialise();
    int ok=!bad && baseCalls==1 && getters==2 && widths==1 && heights==1 &&
       deletes==(!nullInfo && finalRelease?2:0) && sprite.SizeX==expectedSizes[0] && sprite.SizeY==expectedSizes[1] &&
       sprite.GraphicIndex==124 && sprite.BeforeSize[0]==47 && sprite.BeforeGraphicIndex[0]==53;
    for(int j=0;j<2;++j) {
        if(infos[j] && !finalRelease) {if(infos[j]->RefCount!=refs) ok=0;managers[j]->PGraphicsBank.Info=0;::operator delete(infos[j]);}
    }
    return ok;
}
int main() {
    if(!run(1,0,0,0,1) || !run(0,0,3,128,257) || !run(0,1,1,0x80000000UL,0xffffffffUL)) {
        printf("0041a8d0_TEST FAIL\n");return 1;
    }
    printf("0041a8d0_TEST PASS\n");return 0;
}