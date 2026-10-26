// NUISystem::CList::ComputeScrollBarPosition() -- place the scroll bar thumb: count the visible scrolling children and the
//   hidden ones before the selection, move the thumb down the bar's free length (bar height - thumb height * scale.Y) by
//   hiddenBefore / hidden, and give the bar its up/down step offsets and counts ((-1,-1) / 0 when nothing is hidden there).
// Leaf: ?ComputeScrollBarPosition@CList@NUISystem
// Retail: Fable.exe @ 0x0053B0BF, 610 bytes, plain `ret`; /Os + optimize("y", off) TU (as the landed ComputeScrollBarZoom
//   0x0053664C).
// Shape: the def pointers release through a CRefCounted base (DecRef / GetRefCount, slot 1 Destroy) and are zeroed;
//   ~CDefPointer is __forceinline (else /Os calls it out of line).
// Layout: the landed ComputeScrollBarZoom 0x0053664C CList view: SelectedChild +0x15c, ScrollingChildren +0x164 (12-byte
//   vector), ScrollingChildrenVisibility +0x1c8 (byte vector), PScrollBar +0x1d8. Component slots: 12 (+0x30) the scale by
//   value, 31 (+0x7c) ChangePosition, 82 (+0x148) GetStateMap with map operator[] 0x0042D5B1 (landed CreateLevelSlider
//   model), 108 (+0x1b0) GetDef by value (def height at +0x146, released through slot 1), 150..159 descriptive scroll-bar
//   slots. 4294967296.0f @ 0x0122DCB4 (unsigned-to-float fix-up).
#pragma optimize("s", on)
#pragma optimize("y", off)

class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};

namespace NUISystem {

struct SComponentState {
    unsigned char _pad00[8];
    C2DVector     Position;                 // +0x08
};
struct CStateMap {                          // map<unsigned long, SComponentState> (STLport, 12 bytes)
    unsigned char _body[12];
    SComponentState& operator[](const unsigned long& key);   // 0x0042D5B1
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                 // slot 1
};
#pragma pack(push, 2)
struct CComponentDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x146 - 8];
    float         Height;                   // +0x146
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
#pragma pack(pop)
template <class T> struct CDefPointer {
    T* Data;
    __forceinline ~CDefPointer()
    {
        if (Data) {
            Data->ReleaseTemporary();
            Data = 0;
        }
    }
    T* operator->() const { return Data; }
};

class CComponent { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual C2DVector GetScale() const;   // slot 12 (+0x30)
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018();
    virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void ChangePosition(const C2DVector& position, float time, bool b);   // slot 31 (+0x7c)
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081();
    virtual CStateMap& GetStateMap();   // slot 82 (+0x148)
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107();
    virtual CDefPointer<CComponentDef> GetDef();   // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual bool IsMoving();   // slot 150 (+0x258)
    virtual void SetBasePosition(const C2DVector& position);   // slot 151 (+0x25c)
    virtual void v152(); virtual void v153();
    virtual void SetUpOffset(const C2DVector& offset);   // slot 154 (+0x268)
    virtual void SetDownOffset(const C2DVector& offset);   // slot 155 (+0x26c)
    virtual void v156(); virtual void v157();
    virtual void SetUpCount(unsigned long count);   // slot 158 (+0x278)
    virtual void SetDownCount(unsigned long count);   // slot 159 (+0x27c)
};

template <class T> struct Vector {
    T* First; T* Last; T* End;
    unsigned int size() const { return (unsigned int)(Last - First); }
    bool empty() const { return First == Last; }
};

class CList { public:
    unsigned char           _pad000[0x15c];
    int                     SelectedChild;                  // +0x15c
    float                   LastInputTime;                  // +0x160
    Vector<CComponent*>     ScrollingChildren;              // +0x164
    unsigned char           _pad170[0x1c8 - 0x170];
    Vector<unsigned char>   ScrollingChildrenVisibility;    // +0x1c8
    CComponent*             PInitialPositionDummy;          // +0x1d4
    CComponent*             PScrollBar;                     // +0x1d8
    void ComputeScrollBarPosition();
};

void CList::ComputeScrollBarPosition()
{
    CComponent* scrollBar = PScrollBar;
    if (scrollBar == 0 || ScrollingChildren.empty() || scrollBar->IsMoving())
        return;
    unsigned int visible = 0;
    unsigned int hiddenBefore = 0;
    for (unsigned int i = 0; i < ScrollingChildrenVisibility.size(); ++i) {
        if (ScrollingChildrenVisibility.First[i])
            ++visible;
        else if (i < (unsigned int)SelectedChild)
            ++hiddenBefore;
    }
    unsigned int hidden = ScrollingChildrenVisibility.size() - visible;
    unsigned int hiddenAfter = hidden - hiddenBefore;
    C2DVector position = PScrollBar->GetStateMap()[0].Position;
    float length = PScrollBar->GetDef()->Height - PScrollBar->GetDef()->Height * PScrollBar->GetScale().Y;
    if (hidden > 0)
        position.Y += (float)hiddenBefore / (float)hidden * length;
    if (hiddenBefore == 0 || hiddenAfter == 0 || !scrollBar->IsMoving()) {
        PScrollBar->ChangePosition(position, 0.2f, false);
        scrollBar->SetBasePosition(position);
    }
    if (visible < ScrollingChildren.size()) {
        C2DVector step(0.0f, length / (float)hidden);
        C2DVector none(-1.0f, -1.0f);
        if (hiddenBefore) {
            scrollBar->SetUpOffset(step);
            scrollBar->SetUpCount(hiddenBefore);
        } else {
            scrollBar->SetUpOffset(none);
            scrollBar->SetUpCount(0);
        }
        if (hiddenAfter) {
            scrollBar->SetDownOffset(step);
            scrollBar->SetDownCount(hiddenAfter);
        } else {
            scrollBar->SetDownOffset(none);
            scrollBar->SetDownCount(0);
        }
    }
}

}