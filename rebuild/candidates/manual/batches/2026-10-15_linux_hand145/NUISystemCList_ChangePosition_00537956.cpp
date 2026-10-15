// NUISystem::CList::ChangePosition(const C2DVector& position, float time, bool immediate) -- move the list: when the owning component is in scrolling mode (+0x220) the target is first mapped through the list's own position transform (slot 110); then the base component move and the three layout refreshes (slots 185, 187, 186)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00537956, length 159 bytes.
// Callees: virtual slot 108 (+0x1B0, the owner as a counted pointer returned by hidden pointer), slot 110 (+0x1B8,
//          a C2DVector by hidden pointer), NUISystem::CComponent::ChangePosition @ 0x0052E9C0 (direct base call),
//          slots 185 (+0x2E4), 187 (+0x2EC), 186 (+0x2E8).
// Shape: the frame-pointer size TU (`optimize("y", off)` + `("s", on)`: `leave`, `fld ; push ecx ; fstp [esp]`);
// the owner pointer of the landed CList::OnActivate (0x005366FF) with a null-tested release (`mov ecx,eax ;
// add eax,4 ; dec [eax] ; cmp [eax],0`) lands in the dead argument slot; the flag is read into bl before the
// release; the target vector is a 2-word local copied memberwise.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector {
    float x, y;
    C2DVector() { }
    C2DVector(const C2DVector& other) : x(other.x), y(other.y) { }
};
struct CListOwner {                                // reference-counted: count at +4, Delete in slot 1
    virtual void v000();
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +0x04
    unsigned char _pad08[0x218];
    bool          m_scrolling;                     // +0x220
};
struct CListOwnerPointer {                         // 4 bytes: the counted object
    CListOwner* m_pObject;
    CListOwnerPointer() { }
    CListOwnerPointer(const CListOwnerPointer& other);
    CListOwner* operator->() const { return m_pObject; }
    ~CListOwnerPointer()
    {
        if (m_pObject) {
            CListOwner* object = m_pObject;
            unsigned long* count = &m_pObject->IVRefCount;
            --*count;
            if (*(volatile unsigned long*)count == 0)
                object->Delete();
        }
    }
};
namespace NUISystem {
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CListOwnerPointer GetOwner();          // slot 108 (+0x1B0)
    virtual void v109();
    virtual C2DVector TransformPosition(const C2DVector& position);   // slot 110 (+0x1B8)
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158();
    virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174();
    virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182();
    virtual void v183(); virtual void v184();
    virtual void RefreshLayout();                  // slot 185 (+0x2E4)
    virtual void RefreshScrollBars();              // slot 186 (+0x2E8)
    virtual void RefreshChildren();                // slot 187 (+0x2EC)
    void ChangePosition(const C2DVector& position, float time, bool immediate);   // 0x0052E9C0
};
struct CList : CComponent {
    void ChangePosition(const C2DVector& position, float time, bool immediate);
};
}

void NUISystem::CList::ChangePosition(const C2DVector& position, float time, bool immediate)
{
    C2DVector target(position);
    bool scrolling = GetOwner()->m_scrolling;
    if (scrolling)
        target = TransformPosition(position);
    CComponent::ChangePosition(target, time, immediate);
    RefreshLayout();
    RefreshChildren();
    RefreshScrollBars();
}
