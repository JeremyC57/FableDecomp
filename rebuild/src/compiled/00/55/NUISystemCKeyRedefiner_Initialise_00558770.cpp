// NUISystem::CKeyRedefiner::Initialise() -- clickable initialisation, then (once each) create the two list components named by the def, append them to the child vector, keep them as CList pointers (+0x194/+0x198), initialise them and parent them to this redefiner
// Grade: RELOCATION_MATCH (sec1b 2026-10-30) -- the local CListPointer model moved onto the shared engine/CCountedPointer.h (CCountedPointer<CList>): its dtor gives retail's `dec [edi]; jne` release and the register web; SetStaticCast stays out of line (0x0042A2CA) by VC7.1's own choice
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00558770, length 472 bytes (vtable slot 43, plain `ret`).
// Callees: CClickable::Initialise @ 0x0055AC90, CFrontEndManager::GetInstance @ 0x0041E5F2 (static `__fastcall`), CManager::
//   CreateComponent(long) @ 0x0041D21B, CCountedPointer<CComponent>::CCountedPointer(CComponent*) @ 0x00429C15, the child vector's
//   push_back @ 0x00535AD0, CCountedPointer<CList>::SetStaticCast<CComponent> @ 0x0042A2CA, operator delete (import thunk 0x00BFE9BC);
//   component slots 43 Initialise, 51 (descriptive SetParent), 108 GetDef (CDefPointer by hidden pointer), the def's slot 1.
// Layout (retail = Ego_r.pdb - 0x30 like the landed CRedefinerList rows): the component's child vector of CCountedPointer<CComponent>
//   +0xb0, the two list pointers +0x194/+0x198; the def's list-component definitions at +0x198/+0x19c.
// Shape: the landed CRedefinerList::Initialise 0x005566A0 model (CDefPointer local with the inline ReleaseTemporary, counted-pointer
//   temporaries with the inline release); a CCountedPointer<CList> local zero-initialised up front and released at the end; the new
//   child is addressed as `Children[Children.size() - 1]`.
#include "engine/CCountedPointer.h"
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
    unsigned char _def[0x190];             // +0x08
    long          HeaderListDef;           // +0x198 the first list component's definition (descriptive)
    long          ActionListDef;           // +0x19c the second list component's definition (descriptive)
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

class CComponent { public:
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

class CManager { public:
    CComponent* CreateComponent(long definition);       // 0x0041D21B
};
class CFrontEndManager : public CManager { public:
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
class CList : public CComponent { public:
};
typedef CCountedPointer<CList> CListPointer;
struct CChildVector {                                   // STLport vector<CCountedPointer<CComponent> >
    CCountedPointer<CComponent>* _M_start;
    CCountedPointer<CComponent>* _M_finish;
    CCountedPointer<CComponent>* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CCountedPointer<CComponent>& operator[](unsigned long n) { return *(_M_start + n); }
    void push_back(const CCountedPointer<CComponent>& value);   // 0x00535AD0
};
class CClickable : public CComponent { public:
    unsigned char _pad004[0xac];
    CChildVector  Children;                             // +0xb0
    unsigned char _pad0bc[0x194 - 0xbc];
    void Initialise();                                  // 0x0055AC90
};
class CKeyRedefiner : public CClickable { public:
    CList*        PHeaderList;                          // +0x194
    CList*        PActionList;                          // +0x198
    virtual void Initialise();
};

void CKeyRedefiner::Initialise()
{
    CClickable::Initialise();
    CDefPointer def = GetDef();
    CListPointer list;
    if (!PHeaderList && def->HeaderListDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->HeaderListDef)));
        list.SetStaticCast(Children[Children.size() - 1]);
        CList* header = list.Data;
        header->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        PHeaderList = header;
    }
    if (!PActionList && def->ActionListDef) {
        Children.push_back(CCountedPointer<CComponent>(CFrontEndManager::GetInstance()->CreateComponent(def->ActionListDef)));
        list.SetStaticCast(Children[Children.size() - 1]);
        CList* actions = list.Data;
        actions->Initialise();
        Children[Children.size() - 1]->SetParent(this);
        PActionList = actions;
    }
}

}