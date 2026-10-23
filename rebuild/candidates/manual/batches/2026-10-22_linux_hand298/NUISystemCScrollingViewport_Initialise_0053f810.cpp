// NUISystem::CScrollingViewport::Initialise() -- the viewport initialisation, then (per def) create the scroll bar and the outside scroll bar as children: each component is pushed as a counted pointer, initialised and parented, followed by a "PC_DESCRIPTION_SCROLL_BAR_POSITION_DUMMY" component bound to it; with both present the outside bar is linked to the inner one
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053F810, 976 bytes, `ret` (RTTI vtable 0x012468D4 slot 43 = NUISystem::CScrollingViewport;
//   the manifest's "CSprite::Initialize" label is propagated).
// Callees: NUISystem::CViewport::Initialise 0x0054E640 (landed), CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall),
//   CManager::CreateComponent(long) 0x0041D21B / CreateComponent(const CCharString&, bool) 0x0041DB1D,
//   CCountedPointer<CComponent>::CCountedPointer(CComponent*) 0x00429C15, STLport vector<CCountedPointer<CComponent> >
//   _M_insert_overflow 0x005359D0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, operator delete
//   0x00BFE9BC; component slots 43 Initialise, 51 SetParent, 108 GetDef, 124 (+0x1F0) and 152 (+0x260).
// Layout: retail = Ego_r.pdb - 0x34 for CScrollingViewport (PScrollBar +0x188, PScrollBarOutside +0x18C); CComponent::Children
//   (STLport 12-byte vector) +0xB0; the def's scroll-bar definitions +0x4C / +0x12C (descriptive names).
// Shape: the inline STLport push_back (forceinline; `__false_type` with a user ctor so the dummy tag is not zero-stored) of a
//   temporary CCountedPointer whose inline dtor clears both words (the clears survive only before the name's ~CCharString);
//   the last child is read as `Children[Children.size() - 1]` (start in edx, the element straight into ecx); the dummy's
//   name is an implicit const char* -> CCharString conversion, so the temporary is built before `false` is pushed (an explicit
//   CCharString(...) temporary pushes `false` first; a named local in a block drops the clears and the ebx zero register).
#include <stddef.h>
namespace NUISystem {
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};
template<class T> struct CCountedPointer {
    T*           m_pObject;                // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(T* object);            // 0x00429C15 (T = NUISystem::CComponent)
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
    T* operator->() const { return m_pObject; }
};
struct __true_type { };
struct __false_type { __false_type() { } };
template<class T> struct CRetailVector {   // STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);   // 0x005359D0
    __forceinline void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return _M_start[i]; }
    T& back() { return (*this)[size() - 1]; }
};
}
inline void* __cdecl operator new(size_t, void* where) { return where; }
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace NUISystem {
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();                    // slot 0 (the deleting dtor)
    virtual void Destroy();                // slot 1 (+4)
};
struct CScrollingViewportDef : CDefVirtual, CRefCounted {
    unsigned char _def008[0x4c - 0x08];
    long          ScrollBarDef;            // +0x4c (descriptive)
    unsigned char _def050[0x12c - 0x50];
    long          ScrollBarOutsideDef;     // +0x12c (descriptive)
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CScrollingViewportDef* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CScrollingViewportDef* operator->() const { return m_p; }
};
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Initialise();                                  // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void SetParent(CComponent* parent);                 // slot 51 (+0xCC) descriptive
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89(); virtual void v90(); virtual void v91();
    virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer GetDef();                               // slot 108 (+0x1B0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void SetScrolledComponent(CComponent* component);   // slot 124 (+0x1F0) descriptive
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151();
    virtual void SetScrollBar(CComponent* scrollBar);           // slot 152 (+0x260) descriptive
    unsigned char _pad004[0xb0 - 0x04];
    CRetailVector<CCountedPointer<CComponent> > Children;   // +0xb0
};
struct CManager {
    CComponent* CreateComponent(long definition);                     // 0x0041D21B
    CComponent* CreateComponent(const CCharString& name, bool flag);  // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
struct CViewport : CComponent {
    unsigned char _pad0bc[0x188 - 0xbc];
    void Initialise();                                  // 0x0054E640
};
struct CScrollingViewport : CViewport {
    CComponent* PScrollBar;                             // +0x188
    CComponent* PScrollBarOutside;                      // +0x18c
    virtual void Initialise();
};

void CScrollingViewport::Initialise()
{
    CViewport::Initialise();
    CDefPointer def = GetDef();
    if (def->ScrollBarDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->ScrollBarDef)));
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        PScrollBar = Children[Children.size() - 1].m_pObject;
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(
            "PC_DESCRIPTION_SCROLL_BAR_POSITION_DUMMY", false)));
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        Children[Children.size() - 1]->SetScrolledComponent(PScrollBar);
    }
    if (def->ScrollBarOutsideDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->ScrollBarOutsideDef)));
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        PScrollBarOutside = Children[Children.size() - 1].m_pObject;
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(
            "PC_DESCRIPTION_SCROLL_BAR_POSITION_DUMMY", false)));
        Children[Children.size() - 1]->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        Children[Children.size() - 1]->SetScrolledComponent(PScrollBarOutside);
        if (PScrollBar && PScrollBarOutside)
            PScrollBarOutside->SetScrollBar(PScrollBar);
    }
}
}
