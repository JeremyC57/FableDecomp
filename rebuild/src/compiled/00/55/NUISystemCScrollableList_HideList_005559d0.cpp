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