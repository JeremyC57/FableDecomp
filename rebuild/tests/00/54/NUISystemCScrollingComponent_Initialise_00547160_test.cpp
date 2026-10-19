#include <cstdio>
#ifndef _MSC_VER
#define FABLE_ENGINE_CSCROLLINGCOMPONENT_H
#endif
// NUISystem::CScrollingComponent::Initialise() -- initialise the changing-state base and set state3 vertical offset beyond the lowest child
// Leaf: Initialise
// Retail00547160,217 bytes ending ret00547238;7 trailing int3 bytes excluded.
// Shared header is donor-sized flattened and lacks inherited children/states; method view uses
// established component States+24 and8-byte child records+b0. CUIState Position.Y+c agrees PDB.
// Base Initialise0052C730, state-map operator[]0042D5B1, virtual GetCurrentState slot50/GetSize slot23.
#include "engine/CScrollingComponent.h"
namespace NUISystem {
struct C2DVector { float X,Y; C2DVector() {} C2DVector(const C2DVector& v):X(v.X),Y(v.Y) {} };
struct CUIState { unsigned long StateChangeFlag,StateChangeType; C2DVector Position,Zoom; unsigned long Colour; float UpdateTime; bool LinearChange; };
struct CStateMap { void* Head; unsigned long Count; unsigned char Comparator; CUIState& operator[](const unsigned long&); }; //0042D5B1
struct CComponentCalls {
    virtual ~CComponentCalls() {}
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
    virtual void v21();
    virtual void v22();
    virtual C2DVector GetSize() const;
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual CUIState* GetCurrentState();
};
struct CCountedComponent { CComponentCalls* Data; void* Info; };
struct CChangingStateComponentCalls {
    void* VTable;
    unsigned char LeadingState[0x24-4];
    CStateMap States;
    unsigned char BeforeChildren[0xb0-0x30];
    CCountedComponent* ChildrenStart; CCountedComponent* ChildrenFinish; CCountedComponent* ChildrenEnd;
    unsigned char RemainingState[0x15c-0xbc];
    unsigned long size() const {return ChildrenFinish-ChildrenStart;}
    void Initialise(); //0052C730
};
struct CScrollingComponentInitialiseView : CChangingStateComponentCalls { void Initialise(); };
void CScrollingComponentInitialiseView::Initialise() {
    CChangingStateComponentCalls::Initialise();
    float maximum=0.0f;
    unsigned long lowest=0;
    for(unsigned long i=0;i<size();++i) {
        if(ChildrenStart[i].Data->GetCurrentState()->Position.Y>maximum) {
            maximum=ChildrenStart[i].Data->GetCurrentState()->Position.Y;
            lowest=i;
        }
    }
    States[3].Position.Y=lowest<size() ? -maximum-ChildrenStart[lowest].Data->GetSize().Y : -maximum;
}
}

namespace NUISystem {
static CScrollingComponentInitialiseView* active;
static int bad,baseCalls,mapCalls,gets[3],sizes[3],mode;
static CUIState output;
void CChangingStateComponentCalls::Initialise() {
    if(this!=active || baseCalls || mapCalls || gets[0] || gets[1] || gets[2]) bad=1;
    ++baseCalls;
}
CUIState& CStateMap::operator[](const unsigned long& key) {
    if(this!=&active->States || key!=3 || baseCalls!=1 || mapCalls) bad=1;
    ++mapCalls;return output;
}
void CComponentCalls::v01() {}
void CComponentCalls::v02() {}
void CComponentCalls::v03() {}
void CComponentCalls::v04() {}
void CComponentCalls::v05() {}
void CComponentCalls::v06() {}
void CComponentCalls::v07() {}
void CComponentCalls::v08() {}
void CComponentCalls::v09() {}
void CComponentCalls::v10() {}
void CComponentCalls::v11() {}
void CComponentCalls::v12() {}
void CComponentCalls::v13() {}
void CComponentCalls::v14() {}
void CComponentCalls::v15() {}
void CComponentCalls::v16() {}
void CComponentCalls::v17() {}
void CComponentCalls::v18() {}
void CComponentCalls::v19() {}
void CComponentCalls::v20() {}
void CComponentCalls::v21() {}
void CComponentCalls::v22() {}
void CComponentCalls::v24() {}
void CComponentCalls::v25() {}
void CComponentCalls::v26() {}
void CComponentCalls::v27() {}
void CComponentCalls::v28() {}
void CComponentCalls::v29() {}
void CComponentCalls::v30() {}
void CComponentCalls::v31() {}
void CComponentCalls::v32() {}
void CComponentCalls::v33() {}
void CComponentCalls::v34() {}
void CComponentCalls::v35() {}
void CComponentCalls::v36() {}
void CComponentCalls::v37() {}
void CComponentCalls::v38() {}
void CComponentCalls::v39() {}
void CComponentCalls::v40() {}
void CComponentCalls::v41() {}
void CComponentCalls::v42() {}
void CComponentCalls::v43() {}
void CComponentCalls::v44() {}
void CComponentCalls::v45() {}
void CComponentCalls::v46() {}
void CComponentCalls::v47() {}
void CComponentCalls::v48() {}
void CComponentCalls::v49() {}

C2DVector CComponentCalls::GetSize() const {bad=1;return C2DVector();}
CUIState* CComponentCalls::GetCurrentState() {bad=1;return 0;}
struct Child : CComponentCalls {
    int ID;CUIState Current;float Height,SecondY;
    Child(int id):ID(id) {}
    virtual CUIState* GetCurrentState() {
        if(baseCalls!=1 || mapCalls || sizes[0] || sizes[1] || sizes[2]) bad=1;
        ++gets[ID];
        if(mode==1 && ID==1 && gets[ID]==2) Current.Position.Y=SecondY;
        return &Current;
    }
    virtual C2DVector GetSize() const {
        if(baseCalls!=1 || mapCalls) bad=1;
        ++sizes[ID];C2DVector v;v.X=91.0f;v.Y=Height;return v;
    }
};
static int run(int n,float a,float b,float c,int special,float expected,int selected,int ga,int gb,int gc) {
    CScrollingComponentInitialiseView view;active=&view;
    Child x(0),y(1),z(2);Child* children[3]={&x,&y,&z};
    CCountedComponent entries[3]={{&x,0},{&y,0},{&z,0}};
    view.ChildrenStart=entries;view.ChildrenFinish=entries+n;view.ChildrenEnd=entries+3;
    view.States.Head=(void*)children;view.States.Count=7;view.States.Comparator=41;
    float values[3]={a,b,c};
    for(int i=0;i<3;++i) {children[i]->Current.Position.Y=values[i];children[i]->Height=(float)(i+1)*2.0f;children[i]->SecondY=11.0f;gets[i]=sizes[i]=0;}
    bad=baseCalls=mapCalls=0;mode=special;
    output.StateChangeFlag=23;output.Position.X=17;output.Position.Y=81;output.UpdateTime=37;output.LinearChange=true;
    view.Initialise();
    if(bad || baseCalls!=1 || mapCalls!=1 || output.Position.Y!=expected || gets[0]!=ga || gets[1]!=gb || gets[2]!=gc) return 0;
    for(int j=0;j<3;++j) if(sizes[j]!=(j==selected?1:0)) return 0;
    return output.StateChangeFlag==23 && output.Position.X==17 && output.UpdateTime==37 && output.LinearChange &&
      view.ChildrenStart==entries && view.ChildrenFinish==entries+n && view.ChildrenEnd==entries+3 &&
      view.States.Count==7 && view.States.Comparator==41;
}
}
int main() {
    using namespace NUISystem;
    if(!run(0,0,0,0,0,0,-1,0,0,0) || !run(3,2,5,3,0,-9,1,2,2,1) ||
       !run(3,-1,-4,-2,0,-2,0,1,1,1) || !run(3,4,4,1,0,-6,0,2,1,1) ||
       !run(2,1,3,0,1,-15,1,2,2,0)) {printf("00547160_TEST FAIL\n");return 1;}
    printf("00547160_TEST PASS\n");return 0;
}