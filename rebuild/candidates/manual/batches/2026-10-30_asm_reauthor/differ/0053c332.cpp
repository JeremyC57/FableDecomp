// NUISystem::CList::DoRecomputeOffsets() -- re-lay out every scrolling child from the list def's offsets: positions into states 1/4/5/0/6, clamped alpha into states 1/4/5, step and bounce the next position / alpha, apply state 1 at once and keep a copy of each child's states
// Grade: DIFFER(1119v1111) -- (1) PDefPositionOffset copied through a materialised pointer in retail (`lea eax,[esi+1F0h]`), direct [esi+1F0h] here; (2) Y step `fld [+1A8h] ; fadd [+194h]` in retail, operands swapped here (`Y = Y + ToUse.Y` and += alike). Leaf fixed to the PDB MAEXXZ (protected virtual). User C2DVector::operator= made it worse (1113, frame 24h). 4 grades.
// Leaf: ?DoRecomputeOffsets@CList@NUISystem@@MAEXXZ
// Retail: Fable.exe @ 0x0053C332, 1111 bytes, `ret`; size TU with a frame pointer (`#pragma optimize("s", on)` +
// `("y", off)`, `leave`), as the landed AddScrollingChildAndSetOffsets 0x0053AD07 whose per-child body this loops.
// Body: InitialiseOffsets(count) (own slot 156, +0x270); PositionOffset = PDefPositionOffset and PositionOffsetToUse the
// same with X negated; AlphaOffset = AlphaOffsetToUse = PDefAlphaOffset / 255; ScrollingChildrenStates.clear()
// (erase 0x0053CD69); then per scrolling child: states 1, 4, 5, 0, 6 (map operator[] 0x0042D5B1 on GetStateMap, slot 82)
// take NextScrollingChildPosition; states 1, 4, 5 take the alpha byte (0 below 0, 255 above 1, else alpha * 255,
// _ftol 0x00BFEA70); the next position / alpha step and bounce as in AddScrollingChildAndSetOffsets; the child is set to
// state 1's colour (slot 38) and position (slot 31) immediately (time 0); its state map is appended to
// ScrollingChildrenStates (push_back 0x0053CFC2).
// Layout: the 0x0053AD07 view (retail = Ego_r.pdb CList - 0x3c from PositionOffset, - 0x44 from PDefPositionOffset):
// ScrollingChildrenStates +0x17C, PDefPositionOffset +0x1F0, PDefAlphaOffset +0x1F8.
#pragma optimize("s", on)
#pragma optimize("y", off)

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};

namespace NUISystem {

struct CRGBColour {
    unsigned char B, G, R, A;
};
struct SComponentState {                    // CUIState
    unsigned long StateChangeFlag;          // +0x00
    long          StateChangeType;          // +0x04
    C2DVector     Position;                 // +0x08
    C2DVector     Zoom;                     // +0x10
    CRGBColour    Colour;                   // +0x18 (alpha byte +0x1b)
    float         UpdateTime;               // +0x1c
    bool          LinearChange;             // +0x20
};
struct CStateMap {                          // map<unsigned long, SComponentState> (STLport, 12 bytes)
    unsigned char _body[12];
    SComponentState& operator[](const unsigned long& key);   // 0x0042D5B1
};
// Same-TU stand-in for map<unsigned long, CUIState>::operator[] 0x0042D5B1 (body is not the retail one): it only reads
// the key, so VC7.1 knows the named keys are loop-invariant.
SComponentState* FindOrInsertState_0042D5B1(CStateMap* map, unsigned long key);
#pragma auto_inline(off)
__declspec(noinline) SComponentState& CStateMap::operator[](const unsigned long& key)
{
    return *FindOrInsertState_0042D5B1(this, key);
}
#pragma auto_inline(on)
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual C2DVector GetScale() const;   // slot 12 (+0x30)
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018();
    virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void ChangePosition(const C2DVector& position, float time, bool b);   // slot 31 (+0x7c)
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void SetColour(const CRGBColour& colour, float time, bool b);   // slot 38 (+0x98)
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
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
    virtual void v108();
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
    virtual void InitialiseOffsets(unsigned long count);   // slot 156 (+0x270) virtual void v157();
    virtual void SetUpCount(unsigned long count);   // slot 158 (+0x278)
    virtual void SetDownCount(unsigned long count);   // slot 159 (+0x27c)
};

struct CStateMapVector {                    // vector<map<unsigned long, CUIState> > (12 bytes)
    CStateMap* _M_start;
    CStateMap* _M_finish;
    CStateMap* _M_end_of_storage;
    CStateMap* erase(CStateMap* first, CStateMap* last);   // 0x0053CD69
    void clear() { erase(_M_start, _M_finish); }
    void push_back(const CStateMap& x);                     // 0x0053CFC2
};
struct CComponentVector {                   // vector<CComponent*>
    CComponent** First; CComponent** Last; CComponent** End;
    unsigned long size() const { return (unsigned long)(Last - First); }
    CComponent* operator[](unsigned long n) const { return First[n]; }
};
struct CList : CComponent {
    unsigned char           _pad004[0x15c - 4];
    int                     SelectedChild;                  // +0x15c
    float                   LastInputTime;                  // +0x160
    CComponentVector        ScrollingChildren;              // +0x164
    unsigned char           _pad170[0x17c - 0x170];
    CStateMapVector         ScrollingChildrenStates;        // +0x17c
    C2DVector               PositionOffset;                 // +0x188
    C2DVector               PositionOffsetToUse;            // +0x190
    float                   AlphaOffset;                    // +0x198
    float                   AlphaOffsetToUse;               // +0x19c
    float                   NextScrollingChildAlpha;        // +0x1a0
    C2DVector               NextScrollingChildPosition;     // +0x1a4
    unsigned char           _pad1ac[0x1f0 - 0x1ac];
    C2DVector               PDefPositionOffset;             // +0x1f0
    unsigned char           PDefAlphaOffset;                // +0x1f8
protected:
    virtual void DoRecomputeOffsets();
public:
    __forceinline void SetPositionOffset(const C2DVector& offset)
    {
        PositionOffsetToUse = offset;
        PositionOffsetToUse.X = -PositionOffsetToUse.X;
        PositionOffset = offset;
    }
};

void CList::DoRecomputeOffsets()
{
    InitialiseOffsets(ScrollingChildren.size());
    SetPositionOffset(PDefPositionOffset);
    AlphaOffset = AlphaOffsetToUse = (float)PDefAlphaOffset * (1.0f / 255.0f);
    ScrollingChildrenStates.clear();
    for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
        CComponent* child = ScrollingChildren[i];
        unsigned long stateNormal = 1;            // the state keys (names descriptive): map keys are taken by reference,
        unsigned long stateHighlighted = 4;       // so each named key keeps its own frame word; VC7.1 hoists the
        unsigned long stateSelected = 5;          // stores into the loop preheader
        unsigned long stateInitial = 0;
        unsigned long stateExtra = 6;
        unsigned long colourState = 1;
        unsigned long positionState = 1;
        child->GetStateMap()[stateNormal].Position = NextScrollingChildPosition;
        child->GetStateMap()[stateHighlighted].Position = NextScrollingChildPosition;
        child->GetStateMap()[stateSelected].Position = NextScrollingChildPosition;
        child->GetStateMap()[stateInitial].Position = NextScrollingChildPosition;
        child->GetStateMap()[stateExtra].Position = NextScrollingChildPosition;
        if (NextScrollingChildAlpha < 0.0f) {
            child->GetStateMap()[1].Colour.A = 0;
            child->GetStateMap()[4].Colour.A = 0;
            child->GetStateMap()[5].Colour.A = 0;
        } else if (NextScrollingChildAlpha > 1.0f) {
            child->GetStateMap()[1].Colour.A = 0xff;
            child->GetStateMap()[4].Colour.A = 0xff;
            child->GetStateMap()[5].Colour.A = 0xff;
        } else {
            child->GetStateMap()[1].Colour.A = (unsigned char)(NextScrollingChildAlpha * 255.0f);
            child->GetStateMap()[4].Colour.A = (unsigned char)(NextScrollingChildAlpha * 255.0f);
            child->GetStateMap()[5].Colour.A = (unsigned char)(NextScrollingChildAlpha * 255.0f);
        }
        NextScrollingChildPosition.Y = NextScrollingChildPosition.Y + PositionOffsetToUse.Y;
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
        child->SetColour(child->GetStateMap()[colourState].Colour, 0.0f, false);
        child->ChangePosition(child->GetStateMap()[positionState].Position, 0.0f, false);
        ScrollingChildrenStates.push_back(ScrollingChildren[i]->GetStateMap());
    }
}

}
