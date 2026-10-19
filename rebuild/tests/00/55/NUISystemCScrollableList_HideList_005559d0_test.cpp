#include <cstdio>
// NUISystem::CScrollableList::HideList(float) -- fade list children and both arrows at their current positions and reset scrolling state
// Leaf: HideList
// Retail005559D0,184 bytes through ret00555A85; eight trailing int3 bytes excluded.
// No generated CScrollableList header. Retail list entries +b0 are8-byte records;
// PUpArrow+170/PDownArrow+174 and timer+16c agree canonical OnActivate00556200.
// HideComponent00555440; component GetPosition virtual slot16.
namespace NUISystem {
struct C2DVector {
    float X,Y;
    C2DVector() {}
    C2DVector(float x,float y):X(x),Y(y) {}
    C2DVector(const C2DVector& other):X(other.X),Y(other.Y) {}
};
struct CComponent {
    virtual void v00();
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
    virtual C2DVector GetPosition() const;
};
struct CListEntry { CComponent* Component; unsigned long Data; };
struct CScrollableList {
    void* VTable;
    unsigned char BaseLayout[0xb0-4];
    CListEntry* EntriesStart; CListEntry* EntriesFinish; CListEntry* EntriesEnd;
    unsigned char ListLayout[0x16c-0xbc];
    float ScrollTimer;
    CComponent* PUpArrow; CComponent* PDownArrow;
    bool ScrollUp,ScrollDown;
    unsigned long size() const { return EntriesFinish-EntriesStart; }
    void HideComponent(CComponent*,float,float,C2DVector,unsigned char); //00555440
    void HideList(float duration);
};
void CScrollableList::HideList(float duration) {
    for(unsigned long i=0;i<size();++i) {
        CComponent* component=EntriesStart[i].Component;
        HideComponent(component,0.0f,duration,component->GetPosition(),0);
    }
    HideComponent(PUpArrow,0.0f,duration,PUpArrow->GetPosition(),0);
    HideComponent(PDownArrow,0.0f,duration,PDownArrow->GetPosition(),0);
    ScrollTimer=0.0f;ScrollUp=false;ScrollDown=false;
}
}

namespace NUISystem {
static CScrollableList* active;
static CComponent* objects[6];
static int bad,gets,hides,scenario,initialCount;
static float expectedDuration;
static int sequence[8];
void CComponent::v00() {}
void CComponent::v01() {}
void CComponent::v02() {}
void CComponent::v03() {}
void CComponent::v04() {}
void CComponent::v05() {}
void CComponent::v06() {}
void CComponent::v07() {}
void CComponent::v08() {}
void CComponent::v09() {}
void CComponent::v10() {}
void CComponent::v11() {}
void CComponent::v12() {}
void CComponent::v13() {}
void CComponent::v14() {}
void CComponent::v15() {}

C2DVector CComponent::GetPosition() const {
    int id=-1;for(int i=0;i<6;++i) if(this==objects[i]) id=i;
    if(id<0 || gets!=hides || active->ScrollTimer!=8.5f || !active->ScrollUp || !active->ScrollDown) bad=1;
    sequence[gets++]=id;
    if(scenario==3 && id==3) active->PUpArrow=objects[5];
    return C2DVector((float)id+0.25f,-(float)id-0.75f);
}
void CScrollableList::HideComponent(CComponent* component,float start,float duration,C2DVector pos,unsigned char alpha) {
    int id=sequence[hides];
    if(gets!=hides+1 || component!=objects[(scenario==3 && id==3)?5:id] || start!=0.0f ||
       duration!=expectedDuration || alpha || pos.X!=(float)id+0.25f || pos.Y!=-(float)id-0.75f ||
       ScrollTimer!=8.5f || !ScrollUp || !ScrollDown) bad=1;
    ++hides;
    if(hides==1 && scenario==1) EntriesFinish=EntriesStart+3;
    if(hides==1 && scenario==2) EntriesFinish=EntriesStart+1;
}
static int run(int mode,int count,float duration) {
    CComponent a,b,c,u,d,replacement;
    objects[0]=&a;objects[1]=&b;objects[2]=&c;objects[3]=&u;objects[4]=&d;objects[5]=&replacement;
    CListEntry entries[3]={{&a,17},{&b,23},{&c,31}};
    CScrollableList list={};active=&list;scenario=mode;initialCount=count;
    list.EntriesStart=entries;list.EntriesFinish=entries+count;list.EntriesEnd=entries+3;
    list.PUpArrow=&u;list.PDownArrow=&d;list.ScrollTimer=8.5f;list.ScrollUp=list.ScrollDown=true;
    list.ListLayout[0]=71;list.ListLayout[sizeof(list.ListLayout)-1]=93;
    bad=gets=hides=0;expectedDuration=duration;
    list.HideList(duration);
    int n=mode==1?3:mode==2?1:count;
    if(bad || gets!=n+2 || hides!=n+2 || sequence[n]!=3 || sequence[n+1]!=4) return 0;
    for(int i=0;i<n;++i) if(sequence[i]!=i) return 0;
    return list.ScrollTimer==0.0f && !list.ScrollUp && !list.ScrollDown && list.PDownArrow==&d &&
      list.PUpArrow==(mode==3?&replacement:&u) && list.EntriesStart==entries && list.EntriesEnd==entries+3 &&
      list.EntriesFinish==entries+n && entries[0].Data==17 && entries[1].Data==23 && entries[2].Data==31 &&
      list.ListLayout[0]==71 && list.ListLayout[sizeof(list.ListLayout)-1]==93;
}
}
int main() {
    if(!NUISystem::run(0,0,0.0f) || !NUISystem::run(0,3,1.25f) || !NUISystem::run(1,1,-2.5f) ||
      !NUISystem::run(2,3,0.125f) || !NUISystem::run(3,2,7.0f)) {printf("005559d0_TEST FAIL\n");return 1;}
    printf("005559d0_TEST PASS\n");return 0;
}