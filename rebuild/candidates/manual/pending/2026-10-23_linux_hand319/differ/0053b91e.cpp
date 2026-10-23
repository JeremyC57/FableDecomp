// Grade: DIFFER(1416v1422) -- register allocation: retail promotes the constant 0 into ebx for the whole body (so the GetDef()->SelectFirstChild bool spills to [ebp-1], frame 0x28) and recomputes lea [edi+0x17c] in the first loop; ours hoists the scroll layout address into ebx (frame 0x24); middle-index operand order
// NUISystem::CList::Initialise() -- the changing-state initialisation, then start the input timer, add every scrolling child's
//   state map to the scroll layout and select the middle child (or child 0 when the def asks for it); then, for each
//   def-named scroll bar (up +0x134, down +0x130) create the component, add it to the children and the non-scrolling list,
//   give it its range and parent, and for the two scroll-bar sliders (+0x4c, +0x12c) also create the
//   PC_LIST_SCROLL_BAR_POSITION_DUMMY marker and link them; finally hook the second slider to the first
// Leaf: ?Initialise@CList@NUISystem
// Retail: Fable.exe @ 0x0053B91E, 1422 bytes, plain ret; /Os + optimize("y", off) TU (frame pointer), as the matched sibling
//   CList::AddScrollingChildDynamicallyAndSetOffsets 0x0053BF20.
// Callees: CChangingStateComponent::Initialise 0x0052C730, the scroll layout's AddChild 0x0053CFC2,
//   CFrontEndManager::GetInstance 0x0041E5F2, CManager::CreateComponent(long) 0x0041D21B / (const CCharString&, bool)
//   0x0041DB1D, CCountedPointer<CComponent>(CComponent*) 0x00429C15 / ~ 0x004291DE, vector<CCountedPointer<CComponent>>
//   push_back 0x00535AD0, vector<CComponent*> push_back 0x00486024, CCountedPointer<CScrollBar>::ShareData 0x00535FBE /
//   ~ 0x00535EBB, CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Layout: the matched sibling's CList members (+0xb0 children, +0x160, +0x164 scrolling children, +0x17c scroll layout) and
//   the landed CScrollableList::Initialise 0x00555EB0 counted-pointer / def models; +0x170 and +0x1d8..+0x1e4, the def's
//   +0x4c/+0x12c..+0x13c and the component slots 149/150/152/161/124 are descriptive.
#pragma optimize("s", on)
#pragma optimize("y", off)

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

namespace NUISystem {

struct SCountBlock;
struct CStateMap;
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
struct CListDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x4c - 8];
    long          SliderDef;                // +0x4c (descriptive)
    unsigned char _pad50[0x12c - 0x50];
    long          SliderBackDef;            // +0x12c
    long          DownScrollBarDef;         // +0x130
    long          UpScrollBarDef;           // +0x134
    long          UpScrollBarParam;         // +0x138
    long          DownScrollBarOffset;      // +0x13c
    unsigned char _pad140[0x221 - 0x140];
    bool          SelectFirstChild;         // +0x221
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
#pragma pack(pop)
struct CDefPointer {
    CListDef* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CListDef* operator->() const { return m_p; }
};

struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042();
    virtual void Initialise();                              // slot 43 (+0xac)
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050();
    virtual void SetParent(CComponent* parent);             // slot 51 (+0xcc)
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual CStateMap& GetStateMap();                       // slot 82 (+0x148)
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107();
    virtual CDefPointer GetDef();                           // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123();
    virtual void SetPositionMarker(CComponent* marker);     // slot 124 (+0x1f0) descriptive
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void SetScrollParam(long value);                // slot 149 (+0x254) descriptive
    virtual void SetScrollRange(long range);                // slot 150 (+0x258)
    virtual void v151();
    virtual void SetLinkedSlider(CComponent* slider);       // slot 152 (+0x260)
    virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158();
    virtual void v159(); virtual void v160();
    virtual void SetSelectedChild(long index);              // slot 161 (+0x284)
};
struct CScrollBar : CComponent {
};

template <class T> struct CCountedPointer {
    T*           Data;
    SCountBlock* Count;
    CCountedPointer() : Data(0), Count(0) { }
    CCountedPointer(T* object);                             // 0x00429C15 (T = CComponent)
    ~CCountedPointer();                                     // 0x004291DE (CComponent) / 0x00535EBB (CScrollBar)
    void ShareData(void* data, SCountBlock* count);         // 0x00535FBE (CScrollBar)
    template <class U> __forceinline CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Count);
        return *this;
    }
    T* operator->() const { return Data; }
    T* GetPtr() const { return Data; }
};
struct CComponentVector {                   // vector<CComponent*>
    CComponent** First; CComponent** Last; CComponent** End;
    void push_back(CComponent* const& value);               // 0x00486024
    unsigned int size() const { return (unsigned int)(Last - First); }
    bool empty() const { return First == Last; }
    CComponent* operator[](unsigned int i) const { return First[i]; }
};
struct CCountedPointerVector {              // vector<CCountedPointer<CComponent> >
    CCountedPointer<CComponent>* First; CCountedPointer<CComponent>* Last; CCountedPointer<CComponent>* End;
    void push_back(const CCountedPointer<CComponent>& value);   // 0x00535AD0
    unsigned int size() const { return (unsigned int)(Last - First); }
    CCountedPointer<CComponent>& operator[](unsigned int i) { return First[i]; }
};
struct CManager {
    CComponent* CreateComponent(long definition);                       // 0x0041D21B
    CComponent* CreateComponent(const CCharString& definition, bool b); // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();      // 0x0041E5F2
};
struct CScrollLayout {
    void* m_pRows;
    void AddChild(CStateMap& states);       // 0x0053CFC2
};
struct CChangingStateComponent : CComponent {
    unsigned char _pad004[0x30 - 4];
    float         m_Time;                   // +0x30 (descriptive)
    void Initialise();                      // 0x0052C730
};
struct CList : CChangingStateComponent {
    unsigned char           _pad034[0xb0 - 0x34];
    CCountedPointerVector   Children;                       // +0xb0
    unsigned char           _pad0bc[0x160 - 0xbc];
    float                   LastInputTime;                  // +0x160
    CComponentVector        ScrollingChildren;              // +0x164
    CComponentVector        NonScrollingChildren;           // +0x170 (descriptive)
    CScrollLayout           ScrollLayout;                   // +0x17c
    unsigned char           _pad180[0x1d8 - 0x180];
    CComponent*             Slider;                         // +0x1d8
    CComponent*             SliderBack;                     // +0x1dc
    CComponent*             UpScrollBar;                    // +0x1e0
    CComponent*             DownScrollBar;                  // +0x1e4
    void Initialise();
};

void CList::Initialise()
{
    CChangingStateComponent::Initialise();
    LastInputTime = m_Time;
    for (unsigned int i = 0; i < ScrollingChildren.size(); i++)
        ScrollLayout.AddChild(ScrollingChildren[i]->GetStateMap());
    if (!ScrollingChildren.empty()) {
        unsigned int middle = ScrollingChildren.size() / 2;
        if (!(ScrollingChildren.size() & 1))
            middle--;
        SetSelectedChild(middle);
        if (GetDef()->SelectFirstChild)
            SetSelectedChild(0);
    }
    CDefPointer def = GetDef();
    CCountedPointer<CScrollBar> upBar;
    if (def->UpScrollBarDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->UpScrollBarDef)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].GetPtr());
        upBar = Children[Children.size() - 1];
        CScrollBar* up = upBar.Data;
        up->SetScrollParam(def->UpScrollBarParam);
        up->SetScrollRange(ScrollingChildren.size());
        up->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        UpScrollBar = Children[Children.size() - 1].Data;
    }
    CCountedPointer<CScrollBar> downBar;
    if (def->DownScrollBarDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->DownScrollBarDef)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].GetPtr());
        downBar = Children[Children.size() - 1];
        CScrollBar* down = downBar.Data;
        down->SetScrollParam(0);
        down->SetScrollRange(ScrollingChildren.size() - def->DownScrollBarOffset - 1);
        down->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        DownScrollBar = Children[Children.size() - 1].Data;
    }
    if (def->SliderDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->SliderDef)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].GetPtr());
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        Slider = Children[Children.size() - 1].Data;
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(CCharString("PC_LIST_SCROLL_BAR_POSITION_DUMMY"), false)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].GetPtr());
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        Children[Children.size() - 1]->SetPositionMarker(Slider);
    }
    if (def->SliderBackDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->SliderBackDef)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].GetPtr());
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        SliderBack = Children[Children.size() - 1].Data;
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(CCharString("PC_LIST_SCROLL_BAR_POSITION_DUMMY"), false)));
        NonScrollingChildren.push_back(Children[Children.size() - 1].GetPtr());
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        Children[Children.size() - 1]->SetPositionMarker(SliderBack);
        if (Slider && SliderBack)
            SliderBack->SetLinkedSlider(Slider);
    }
}

}
