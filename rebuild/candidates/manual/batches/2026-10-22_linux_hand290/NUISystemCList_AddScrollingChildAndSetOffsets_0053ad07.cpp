// NUISystem::CList::AddScrollingChildAndSetOffsets(const CCountedPointer<CComponent>&) -- add a scrolling child at the
//   list's next scrolling-child position / alpha: write the position into the child's states 1, 4, 5 (and 0, 3 when the
//   list def asks for it) and the alpha byte (clamped: 0, 255 or alpha * 255) into states 1, 4, 5; advance the next position by the
//   position offset and the alpha by the alpha offset, bouncing both offsets when the alpha leaves [.., 0.999); then add the
//   child, append it to the scrolling children (visible) and record its child index as a shape child
// Leaf: ?AddScrollingChildAndSetOffsets@CList@NUISystem
// Retail: Fable.exe @ 0x0053AD07, 952 bytes, `ret 4`; /Os + optimize("y", off) TU (as ComputeScrollBarPosition 0x0053B0BF).
// Callees: map operator[] 0x0042D5B1, CComponent::AddChild 0x00533B50, vector push_backs 0x00486024 (CComponent*),
//   0x0053CB4F (bool), 0x005178A0 (unsigned long), _ftol 0x00BFEA70. 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 0.999f 0x012462AC,
//   255.0f 0x01230014.
// Layout: the ComputeScrollBarPosition view, plus retail = Ego_r.pdb CList - 0x3c from PositionOffset on (12-byte
//   vectors): PositionOffsetToUse +0x190, AlphaOffsetToUse +0x19c, NextScrollingChildAlpha +0x1a0,
//   NextScrollingChildPosition +0x1a4; CComponent Children +0xb0 (8-byte CCountedPointer elements), ShapeChildren +0xe4.
#pragma optimize("s", on)
#pragma optimize("y", off)

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};

namespace NUISystem {

struct SComponentState {                    // CUIState
    unsigned char _pad00[8];
    C2DVector     Position;                 // +0x08
    unsigned char _pad10[0x1b - 0x10];
    unsigned char Alpha;                    // +0x1b
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
    unsigned char _pad08[0x221 - 8];
    bool          SetOffsetsForAllStates;   // +0x221 (descriptive)
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

struct CComponent {
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

template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
    T* GetPtr() const { return Data; }
};
struct CComponentVector {                   // vector<CComponent*>
    CComponent** First; CComponent** Last; CComponent** End;
    void push_back(CComponent* const& value);           // 0x00486024
};
struct CBoolVector {                        // vector<bool> as retail stores it
    unsigned char* First; unsigned char* Last; unsigned char* End;
    void push_back(const bool& value);                   // 0x0053CB4F
};
struct CULongVector {                       // vector<unsigned long>
    unsigned long* First; unsigned long* Last; unsigned long* End;
    void push_back(const unsigned long& value);          // 0x005178A0
};
struct CCountedPointerVector {              // vector<CCountedPointer<CComponent> >
    CCountedPointer<CComponent>* First; CCountedPointer<CComponent>* Last; CCountedPointer<CComponent>* End;
    unsigned int size() const { return (unsigned int)(Last - First); }
};

struct CList : CComponent {
    unsigned char           _pad004[0xb0 - 4];
    CCountedPointerVector   Children;                       // +0xb0
    unsigned char           _pad0bc[0xe4 - 0xbc];
    CULongVector            ShapeChildren;                  // +0xe4
    unsigned char           _pad0f0[0x15c - 0xf0];
    int                     SelectedChild;                  // +0x15c
    float                   LastInputTime;                  // +0x160
    CComponentVector        ScrollingChildren;              // +0x164
    unsigned char           _pad170[0x188 - 0x170];
    C2DVector               PositionOffset;                 // +0x188
    C2DVector               PositionOffsetToUse;            // +0x190
    float                   AlphaOffset;                    // +0x198
    float                   AlphaOffsetToUse;               // +0x19c
    float                   NextScrollingChildAlpha;        // +0x1a0
    C2DVector               NextScrollingChildPosition;     // +0x1a4
    unsigned char           _pad1ac[0x1c8 - 0x1ac];
    CBoolVector             ScrollingChildrenVisibility;    // +0x1c8
    void AddChild(const CCountedPointer<CComponent>& child);   // 0x00533B50
    void AddScrollingChildAndSetOffsets(const CCountedPointer<CComponent>& child);
};

void CList::AddScrollingChildAndSetOffsets(const CCountedPointer<CComponent>& child)
{
    CComponent* c;
    c = child.Data;
    c->GetStateMap()[1].Position = NextScrollingChildPosition;
    c = child.Data;
    c->GetStateMap()[4].Position = NextScrollingChildPosition;
    c = child.Data;
    c->GetStateMap()[5].Position = NextScrollingChildPosition;
    if (GetDef()->SetOffsetsForAllStates) {
        c = child.Data;
        c->GetStateMap()[0].Position = NextScrollingChildPosition;
        c = child.Data;
        c->GetStateMap()[3].Position = NextScrollingChildPosition;
    }
    if (NextScrollingChildAlpha < 0.0f) {
        child->GetStateMap()[1].Alpha = 0;
        c = child.Data;
        c->GetStateMap()[4].Alpha = 0;
        c = child.Data;
        c->GetStateMap()[5].Alpha = 0;
    } else if (NextScrollingChildAlpha > 1.0f) {
        child->GetStateMap()[1].Alpha = 0xff;
        c = child.Data;
        c->GetStateMap()[4].Alpha = 0xff;
        c = child.Data;
        c->GetStateMap()[5].Alpha = 0xff;
    } else {
        child->GetStateMap()[1].Alpha = (unsigned char)(NextScrollingChildAlpha * 255.0f);
        child->GetStateMap()[4].Alpha = (unsigned char)(NextScrollingChildAlpha * 255.0f);
        child->GetStateMap()[5].Alpha = (unsigned char)(NextScrollingChildAlpha * 255.0f);
    }
    NextScrollingChildPosition.Y += PositionOffsetToUse.Y;
    float alpha = AlphaOffsetToUse + NextScrollingChildAlpha;
    if (!(alpha >= 1.0f) && !(alpha >= 0.999f)) {
        NextScrollingChildAlpha = alpha;
        NextScrollingChildPosition.X += PositionOffsetToUse.X;
    } else {
        AlphaOffsetToUse = -AlphaOffsetToUse;
        PositionOffsetToUse.X = -PositionOffsetToUse.X;
        if (NextScrollingChildAlpha < 1.0f) {
            NextScrollingChildPosition.X = 0.0f;
            NextScrollingChildAlpha = 1.0f;
        } else {
            NextScrollingChildPosition.X += PositionOffsetToUse.X;
            NextScrollingChildAlpha += AlphaOffsetToUse;
        }
    }
    AddChild(child);
    ScrollingChildren.push_back(child.GetPtr());
    ScrollingChildrenVisibility.push_back(true);
    ShapeChildren.push_back(Children.size() - 1);
}

}
