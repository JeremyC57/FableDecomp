// NUISystem::CDynamicList::ScrollSelectionDown() -- move the highlighted child one row down (when a next child exists) and scroll the view so the selection stays inside the visible window
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00540EC0, length 136 bytes (+ int3 padding).
// Callees: CComponent virtual slot 48 (+0xc0, the child's state setter: 4 = normal, 3 = highlighted), this->ResetPositions (slot 142, +0x238,
// tail-jumped; CDynamicList's override @ 0x005416B0). Layout as in ScrollSelectionUp @ 0x00540E50 (PDB members 0x30 lower in retail).
// Shape: `test eax,eax ; jl` = `SelectionIndex >= 0`, then `lea edx,[eax+1] ; cmp edx,size ; jae` = `SelectionIndex + 1 < size()` with an unsigned
// size (`(last - first) >> 2`); the window test `SelectionIndex >= TopIndex + NumVisible` and the new top `SelectionIndex - NumVisible + 1`.
namespace NUISystem {

class CComponent { public:                              // a scrolling child: vtable slot 48 (+0xc0) sets its draw state
    virtual void c000();
    virtual void c001();
    virtual void c002();
    virtual void c003();
    virtual void c004();
    virtual void c005();
    virtual void c006();
    virtual void c007();
    virtual void c008();
    virtual void c009();
    virtual void c010();
    virtual void c011();
    virtual void c012();
    virtual void c013();
    virtual void c014();
    virtual void c015();
    virtual void c016();
    virtual void c017();
    virtual void c018();
    virtual void c019();
    virtual void c020();
    virtual void c021();
    virtual void c022();
    virtual void c023();
    virtual void c024();
    virtual void c025();
    virtual void c026();
    virtual void c027();
    virtual void c028();
    virtual void c029();
    virtual void c030();
    virtual void c031();
    virtual void c032();
    virtual void c033();
    virtual void c034();
    virtual void c035();
    virtual void c036();
    virtual void c037();
    virtual void c038();
    virtual void c039();
    virtual void c040();
    virtual void c041();
    virtual void c042();
    virtual void c043();
    virtual void c044();
    virtual void c045();
    virtual void c046();
    virtual void c047();
    virtual void SetState(long state);             // slot 48 (+0xc0): 4 = normal, 3 = highlighted
};

template <class T>
struct CRetailVector {                           // STLport vector: 12 bytes (first, last, end of storage)
    T* m_first;
    T* m_last;
    T* m_endOfStorage;
    T* begin() const { return m_first; }
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct IComponentA {                             // the interface base at +0x04 (vtable, 0x10 bytes of state)
    virtual void va00();
    unsigned char _state[0x10];
};

struct IComponentB {                             // the interface base at +0x18 (vtable only)
    virtual void vb00();
};

struct CComponentRoot {                          // the primary base: vptr at +0
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void ResetPositions();                 // slot 142 (+0x238; CDynamicList's override @ 0x005416B0)
};

class CChangingStateComponent : public CComponentRoot, public IComponentA, public IComponentB { public:   // retail 0x15c bytes
    unsigned char _body[0x15c - 0x1c];           // +0x1c .. +0x15c
};

class CDynamicList : public CChangingStateComponent { public:  // vtables 0x01246E44 / 0x01246E20 / 0x01246E18
    long BInitialised;                           // +0x15c (PDB +0x18c)
    float PositionOffset[2];                     // +0x160 (C2DVector)
    long TopIndex;                               // +0x168 (PDB +0x198)
    long SelectionIndex;                         // +0x16c
    long NumVisible;                             // +0x170
    CRetailVector<CComponent*> ScrollingChildren; // +0x174 (PDB +0x1a4)
    void ScrollSelectionUp();
    void ScrollSelectionDown();
};

void CDynamicList::ScrollSelectionDown()
{
    if (SelectionIndex >= 0 && (unsigned long)(SelectionIndex + 1) < ScrollingChildren.size()) {
        ScrollingChildren[SelectionIndex]->SetState(4);
        ++SelectionIndex;
        ScrollingChildren[SelectionIndex]->SetState(3);
        if (SelectionIndex >= TopIndex + NumVisible) {
            TopIndex = SelectionIndex - NumVisible + 1;
            ResetPositions();
        }
    }
}

}