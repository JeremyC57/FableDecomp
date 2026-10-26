// CCountedPointer<CBrainOutput>::DeleteData(void* data) -- the counted pointer's per-type deleter: run the CBrainOutput destructor and free it
// Leaf: DeleteData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6210, length 75 bytes (+ int3 padding), plain `ret`, `__fastcall`
// (object in ecx). No direct callers: its address is stored as the count block's destroy function
// (absolute reference at 0x006D62B0, inside CCountedPointer<CBrainOutput>::Reset 0x006D6260, landed).
// Manifest label "_global::DeleteData" is the right leaf; the class comes from the destroyed layout, which is
// CBrainOutput's (Ego_r.pdb, 0x48 bytes): PAction CCountedPointer<CCreatureActionBase> at +0x20 (object +0x20,
// count block +0x24) and LookAtThing CIntelligentPointer<const CThing> at +0x40 -- exactly the members the landed
// CBrainOutput scalar deleting destructor 0x006D61B0 tears down; the model is copied from that file.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (LookAtThing), the action's destroy
// function through its count block (`call [eax+4]`), operator delete @ 0x00BFE9BC (count block, then the object).
// Shape: `delete static_cast<CBrainOutput*>(data)` = null test, the inlined implicit destructor (members in reverse
// order: LookAtThing, then PAction's release with the count re-read after the decrement and both zero stores),
// then operator delete.
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
};

class CCreatureActionBase;

struct SCountBlock {                            // the shared count block of a CCountedPointer
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);   // +0x04
    CCreatureActionBase* m_pObject;             // +0x08
};

struct CCountedPointer_CCreatureActionBase {    // {object, count block}
    CCreatureActionBase* m_pObject;             // +0x00
    SCountBlock*         m_pCount;              // +0x04
    __forceinline ~CCountedPointer_CCreatureActionBase()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

class C3DVector { public: float X, Y, Z; };

class CNavigationInput { public:                       // 24 bytes, trivially destructible
    unsigned char _body[0x18];
};

class CBrainOutput { public:
    unsigned long                     Types;            // +0x00
    long                              LookFlags;        // +0x04
    CNavigationInput                  NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;        // +0x20
    C3DVector                         MoveToPos;        // +0x28
    C3DVector                         LookAtPos;        // +0x34
    CIntelligentPointer               LookAtThing;      // +0x40
};

template<class T> class CCountedPointer { public:
    T*    Data;                                 // +0x00
    void* Info;                                 // +0x04
    static void __fastcall DeleteData(void* data)
    {
        delete static_cast<T*>(data);
    }
};

template class CCountedPointer<CBrainOutput>;