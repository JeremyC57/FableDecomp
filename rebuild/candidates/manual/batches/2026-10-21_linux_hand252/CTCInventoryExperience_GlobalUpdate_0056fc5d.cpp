// CTCInventoryExperience::GlobalUpdate(bool paused) -- base update; while the screen is open: update every slider animator, advance the animator iterator past the ones with nothing to animate (starting the first whose start and end differ), and set each visible slider's alpha (full when playing or when the iterator is at the end, else 120); then tick the two experience-spend event timers and apply a pending rebuild of the screen
// Leaf: GlobalUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056FC5D, 358 bytes, `ret 4`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (no frame words: the colour and the list temporary use the argument slot).
// Callees: NInventory::CTCInventoryBase::GlobalUpdate(bool) @ 0x005BDD39, CSliderAnimator update @ 0x0056EB48, the
//   slider component's virtual slots 14 (+0x38, GetColour by hidden pointer) and 13 (+0x34, SetColour), this's slots 90
//   (+0x168) and 87 (+0x15C), the screen object's (this+0x0C -> +0x04) slots 159 (+0x27C, a list by hidden pointer) and
//   160 (+0x280), that list's destructor @ 0x0053C965.
// Shape: AnimIt is an STLport list iterator (`++AnimIt`, `AnimIt->`); the advance-loop and alpha-pick end tests compare
//   the raw node pointers (`cmp eax,[esi+1A4h]`; a by-value end() iterator loads the head into a register first), the
//   opening test uses end(); the alpha is `!Play ? 0x78 : 0xFF` (setne/dec/and 79h/dec).
// Layout: retail CTCInventoryExperience (12-byte STLport maps/vectors, 4-byte list; cross-checked against the ctor
//   0x0057832D): CTCInventoryBase InventoryOpen +0x58; Animators +0x1A4, AnimIt +0x1A8; ExperienceSpendAdd +0x1C8,
//   ExperienceSpendRemove +0x1D0 (Ego_r.pdb ExperienceSpendEvent {enable, initiate, time}); DestroyEverything +0x21C;
//   CSliderAnimator as Ego_r.pdb (Play +0x18). PDB offsets differ from retail (debug containers are 16 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
};
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual void SetColour(const CRGBColour& colour);   // slot 13 (+0x34)
    virtual CRGBColour GetColour() const;            // slot 14 (+0x38)
};
struct CSliderAnimator {
    CComponent* pSlider;                           // +0x00
    float StartValue;                                      // +0x04
    float EndValue;                                // +0x08
    unsigned char _pad0c[0x18 - 0x0c];
    bool Play;                                      // +0x18
    void Update();                                   // 0x0056EB48
};
struct SAnimatorNode {
    SAnimatorNode* _M_next;
    SAnimatorNode* _M_prev;
    CSliderAnimator _M_data;                         // +0x08
};
struct CAnimatorIterator {                               // STLport _List_iterator
    SAnimatorNode* _M_node;
    CSliderAnimator* operator->() const { return &_M_node->_M_data; }
    CAnimatorIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const CAnimatorIterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const CAnimatorIterator& o) const { return _M_node != o._M_node; }
};
struct ExperienceSpendEvent {                        // Ego_r.pdb, 8 bytes
    bool enable;                                     // +0x00
    bool initiate;                                   // +0x01
    long time;                                       // +0x04
};
struct CAnimatorList {
    SAnimatorNode* _M_node;
    CAnimatorIterator end() const { CAnimatorIterator it; it._M_node = _M_node; return it; }
};

struct CPickupList {
    void* _M_node;
    ~CPickupList();                                  // 0x0053C965
};
struct CScreenObject {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94();
    virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158();
    virtual CPickupList SaveState_slot159();          // slot 159 (+0x27C)
    virtual void RestoreState_slot160(const CPickupList& state);   // slot 160 (+0x280)
};
struct CScreenHolder {
    void* __vftable;
    CScreenObject* Screen;                           // +0x04
};

namespace NInventory {
struct CTCInventoryBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86();
    virtual void Slot87(long value);                 // slot 87 (+0x15C)
    virtual void v88(); virtual void v89();
    virtual void Slot90(long value);                 // slot 90 (+0x168)
    void*         ParentThing;                       // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CScreenHolder* PScreen;                          // +0x0C
    unsigned char _pad10[0x58 - 0x10];
    bool          InventoryOpen;                     // +0x58
    void GlobalUpdate(bool paused);                  // 0x005BDD39
};
}

struct CTCInventoryExperience : NInventory::CTCInventoryBase {
    unsigned char _pad5c[0x1a4 - 0x5c];
    CAnimatorList     Animators;                             // +0x1A4 list<CSliderAnimator> node pointer
    CAnimatorIterator AnimIt;                       // +0x1A8
    unsigned char _pad1ac[0x1c8 - 0x1ac];
    ExperienceSpendEvent ExperienceSpendAdd;         // +0x1C8
    ExperienceSpendEvent ExperienceSpendRemove;      // +0x1D0
    unsigned char _pad1d8[0x21c - 0x1d8];
    bool          DestroyEverything;                    // +0x21C

    void GlobalUpdate(bool paused);
};

void CTCInventoryExperience::GlobalUpdate(bool paused)
{
    NInventory::CTCInventoryBase::GlobalUpdate(paused);
    if (InventoryOpen && AnimIt != Animators.end()) {
        for (SAnimatorNode* n = Animators._M_node->_M_next; n != Animators._M_node; n = n->_M_next)
            n->_M_data.Update();
        if (!AnimIt->Play) {
            for (;;) {
                ++AnimIt;
                if (AnimIt._M_node == Animators._M_node)
                    break;
                if (AnimIt->StartValue == AnimIt->EndValue)
                    continue;
                AnimIt->Play = true;
                break;
            }
        }
        for (SAnimatorNode* n = Animators._M_node->_M_next; n != Animators._M_node; n = n->_M_next) {
            CRGBColour colour = n->_M_data.pSlider->GetColour();
            if (colour.A >= 0x46) {
                if (AnimIt._M_node != Animators._M_node)
                    colour.A = !n->_M_data.Play ? 0x78 : 0xff;
                else
                    colour.A = 0xff;
                n->_M_data.pSlider->SetColour(colour);
            }
        }
    }
    if (ExperienceSpendAdd.initiate) {
        long& time = ExperienceSpendAdd.time;
        if (--time < 0) {
            time = 0;
            ExperienceSpendAdd.enable = true;
        }
    }
    if (ExperienceSpendRemove.initiate) {
        long& time = ExperienceSpendRemove.time;
        if (--time < 0) {
            time = 0;
            ExperienceSpendRemove.enable = true;
        }
    }
    if (InventoryOpen) {
        bool& pending = DestroyEverything;
        if (pending) {
            pending = false;
            CPickupList state = PScreen->Screen->SaveState_slot159();
            Slot90(0);
            Slot87(0);
            PScreen->Screen->RestoreState_slot160(state);
        }
    }
}
