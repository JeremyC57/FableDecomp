// NUISystem::CRedefinerList::UpdateUndefinedWarning(float time) -- scan the child redefiners for an undefined control; show the undefined-controls warning and hide the accept button if one is found, else the reverse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00556810, length 279 bytes (`ret 4`).
// Callees: CKeyRedefiner::IsUndefined @ 0x00557940 (descriptive: SubType == 2 && Definition == 0; manifest label
// "CTCKraken::HaveAllTentaclesBeenHit" is propagated), CScrollableList::UnhideComponent(CComponent*, float, float, C2DVector)
// @ 0x005556F0, CScrollableList::HideComponent(CComponent*, float, float, C2DVector, unsigned char) @ 0x00555440, the child's
// virtual slot 65 (+0x104, component type; 0x27 = key redefiner) and slot 16 GetPosition (+0x40, C2DVector by hidden pointer).
// Layout: retail = Ego_r.pdb - 0x30 (as the landed OnActivate 0x005570F0): CComponent::Children +0xb0 (12-byte STLport vector of
// 8-byte counted pointers), HaveUndefinedControls +0x184, PUndefinedWarning +0x188, PAcceptButton +0x18c.
// Shape: the flag is cleared before the warning test; the loop recomputes size() (`sar 3`) each pass and re-reads the
// element through the vector for the redefiner's null test; the flag is re-read from memory for the branch, the warning's
// vtable fetch is shared by both arms; the by-value C2DVector results are built straight into the argument area (user ctor
// + user copy ctor), the float time passes through edi.
namespace NUISystem {

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
};

struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    T* operator->() const { return Data; }
};
template <class T> struct CRetailVector {               // STLport vector (12 bytes; PDB 16)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
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
    virtual C2DVector GetPosition() const;              // slot 16 (+0x40)
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
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
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual long GetComponentType();                    // slot 65 (+0x104) descriptive
};

struct CKeyRedefiner : CComponent {
    bool IsUndefined();                                 // 0x00557940
};

struct CScrollableList : CComponent {
    unsigned char _pad004[0xac];
    CRetailVector<CCountedPointer<CComponent> > Children;   // +0xb0 (CComponent::Children, PDB +0xb8)
    unsigned char _pad0bc[0xc0];
    void HideComponent(CComponent* component, float startTime, float endTime, C2DVector position, unsigned char alpha);   // 0x00555440
    void UnhideComponent(CComponent* component, float startTime, float endTime, C2DVector position);                      // 0x005556F0
};

struct CRedefinerList : CScrollableList {
    bool          IsActivated;                          // +0x17c
    bool          RefreshAfterFade;                     // +0x17d
    float         FadeOutTime;                          // +0x180
    bool          HaveUndefinedControls;                // +0x184
    CComponent*   PUndefinedWarning;                    // +0x188
    CComponent*   PAcceptButton;                        // +0x18c
    void UpdateUndefinedWarning(float time);
};

void CRedefinerList::UpdateUndefinedWarning(float time)
{
    HaveUndefinedControls = false;
    if (!PUndefinedWarning)
        return;
    for (unsigned int i = 0; i < Children.size(); ++i) {
        if (Children[i]->GetComponentType() == 0x27) {
            CKeyRedefiner* redefiner = static_cast<CKeyRedefiner*>(Children[i].Data);
            if (redefiner && redefiner->IsUndefined()) {
                HaveUndefinedControls = true;
                break;
            }
        }
    }
    if (HaveUndefinedControls) {
        UnhideComponent(PUndefinedWarning, 0.0f, time, PUndefinedWarning->GetPosition());
        if (PAcceptButton)
            HideComponent(PAcceptButton, 0.0f, time, PAcceptButton->GetPosition(), 0x60);
    }
    else {
        HideComponent(PUndefinedWarning, 0.0f, time, PUndefinedWarning->GetPosition(), 0);
        if (PAcceptButton)
            UnhideComponent(PAcceptButton, 0.0f, time, PAcceptButton->GetPosition());
    }
}

}