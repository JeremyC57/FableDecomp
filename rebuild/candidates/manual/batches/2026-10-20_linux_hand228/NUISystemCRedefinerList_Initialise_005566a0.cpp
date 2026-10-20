// NUISystem::CRedefinerList::Initialise() -- the scrollable-list initialisation, register as the active redefiner list, then create the undefined-controls warning component from the def, attach it to the parent, initialise it and hide it at its own position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005566A0, length 257 bytes (vtable slot 43 of 0x0124A994).
// Callees: CScrollableList::Initialise @ 0x00555EB0 (manifest label "CChangingStateComponent::Initialize"), CFrontEndManager::
// GetInstance @ 0x0041E5F2 (static `__fastcall`), CManager::CreateComponent(long) @ 0x0041D21B, CCountedPointer<CComponent>::
// CCountedPointer(CComponent*) @ 0x00429C15, CScrollableList::HideComponent(CComponent*, float, float, C2DVector, unsigned char)
// @ 0x00555440, operator delete (import thunk 0x00BFE9BC); component slots 16 GetPosition, 43 Initialise, 51/52 (descriptive
// SetParent/GetParent), 59 AddChild, 108 GetDef (CDefPointer by hidden pointer), the def's slot 1 when its count reaches 0.
// Globals: the active redefiner list 0x013B8AC4 (CRedefinerList::s_pActiveList, as the landed `??_G` 0x00557360).
// Layout: retail = Ego_r.pdb - 0x30: PUndefinedWarning +0x188; the def's warning component definition at +0x1a0.
// Shape: the def pointer is a CDefPointer local whose null-tested dtor calls the def's inline ReleaseTemporary() over a ref-count
// base at +4 (the landed 0x00545720 model: `mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi`); the warning goes into a
// counted-pointer local (inline {count, destroy, object} release, block re-read from its frame word) and is used through it
// for the parent/initialise calls; HideComponent re-reads the member and builds the C2DVector in the argument area.
namespace NUISystem {

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
};

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};
template<class T> struct CCountedPointer {
    T*           m_pObject;                // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(T* object);            // 0x00429C15 (T = NUISystem::CComponent)
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
    }
    T* operator->() const { return m_pObject; }
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();                    // slot 0 (the deleting dtor)
    virtual void Destroy();                // slot 1 (+4)
};
struct CRedefinerListDef : CDefVirtual, CRefCounted {
    unsigned char _def[0x198];             // +0x08
    long          UndefinedWarningDef;     // +0x1a0 the warning component's definition (descriptive)
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CRedefinerListDef* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CRedefinerListDef* operator->() const { return m_p; }
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
    virtual C2DVector GetPosition() const;                      // slot 16 (+0x40)
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
    virtual void Initialise();                                  // slot 43 (+0xAC)
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void SetParent(CComponent* parent);                 // slot 51 (+0xCC) descriptive
    virtual CComponent* GetParent();                            // slot 52 (+0xD0) descriptive
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void AddChild(const CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual void v87();
    virtual void v88();
    virtual void v89();
    virtual void v90();
    virtual void v91();
    virtual void v92();
    virtual void v93();
    virtual void v94();
    virtual void v95();
    virtual void v96();
    virtual void v97();
    virtual void v98();
    virtual void v99();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual CDefPointer GetDef();                               // slot 108 (+0x1B0)
};

struct CManager {
    CComponent* CreateComponent(long definition);       // 0x0041D21B
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};

struct CScrollableList : CComponent {
    unsigned char _pad004[0x178];
    void Initialise();                                  // 0x00555EB0
    void HideComponent(CComponent* component, float startTime, float endTime, C2DVector position, unsigned char alpha);   // 0x00555440
};

struct CRedefinerList : CScrollableList {
    bool          IsActivated;                          // +0x17c
    bool          RefreshAfterFade;                     // +0x17d
    float         FadeOutTime;                          // +0x180
    bool          HaveUndefinedControls;                // +0x184
    CComponent*   PUndefinedWarning;                    // +0x188
    CComponent*   PAcceptButton;                        // +0x18c
    static CRedefinerList* s_pActiveList;               // 0x013B8AC4
    virtual void Initialise();
};

void CRedefinerList::Initialise()
{
    CScrollableList::Initialise();
    s_pActiveList = this;
    CDefPointer def = GetDef();
    if (def->UndefinedWarningDef) {
        PUndefinedWarning = CFrontEndManager::GetInstance()->CreateComponent(def->UndefinedWarningDef);
        CCountedPointer<CComponent> warning(PUndefinedWarning);
        GetParent()->AddChild(warning);
        warning->SetParent(GetParent());
        warning->Initialise();
        HideComponent(PUndefinedWarning, 0.0f, 0.0f, PUndefinedWarning->GetPosition(), 0);
    }
}

}
