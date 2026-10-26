// NUISystem::CDraggableInto::Initialize() -- if the component's def names a drop action, create that action and install it as the component's drop action
// Leaf: Initialize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00558E10, length 174 bytes (+ int3 padding), `ret`.
// Callees: virtual slot 108 (+0x1b0) GetDef (CDefPointer by hidden pointer), operator new @ 0x00BFEA1A (16 bytes),
// NUISystem::CAction::CAction() @ 0x0042BE50 (catalog "CRumbleDef::CRumbleDef", propagated),
// CCountedPointer<NUISystem::CAction>::CCountedPointer(CAction*) @ 0x0042AA29, virtual slot 66 (+0x108) taking the counted
// action by const reference, operator delete @ 0x00BFE9BC (the count block), the def's virtual slot 1 when its
// count reaches zero.
// Vtables: NUISystem::CDraggableInto 0x0124B2D4 (primary) / 0x0124B2B0.
// Layout: the def's drop action id at +0xc4; the 16-byte CAction keeps its type in the first word. The def's reference
// count is a base at +4 (the landed CSpellContainer 0x00545720 model).
// Shape: the def pointer is a named local whose destructor null-tests it and runs the inline ReleaseTemporary()
// (`DecRef(); if (GetRefCount() == 0) Destroy();` through the +4 base); the counted action is a named local in the `if`
// with its inline destructor (block re-read after the decrement, deleter through the block, block freed).
namespace NUISystem {

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CDraggableIntoDef : CDefVirtual, CRefCounted {
    unsigned char _def[0xbc];                      // +0x08
    long DropAction;                               // +0xc4
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CDraggableIntoDef* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CDraggableIntoDef* operator->() const { return m_p; }
};

class CAction { public:                                   // 16 bytes
    long Type;                                     // +0x00
    unsigned char _body[0xc];
    CAction();                                     // 0x0042BE50
};

void __fastcall DestroyAction(void* action);       // the deleter stored in the count block
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> struct CCountedPointer {
    T* Object;                                     // +0
    SCountBlock* Info;                             // +4
    CCountedPointer(T* object);                    // 0x0042AA29
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
    T* operator->() const { return Object; }
};

class CDraggableInto {                             // vtable 0x0124B2D4
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065();
    virtual void SetDropAction(const CCountedPointer<CAction>& action);   // slot 66 (+0x108)
    virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107();
    virtual CDefPointer GetDef();                  // slot 108 (+0x1b0)
    void Initialize();
};

void CDraggableInto::Initialize()
{
    CDefPointer def = GetDef();
    if (def->DropAction) {
        CCountedPointer<CAction> action(new CAction);
        action->Type = def->DropAction;
        SetDropAction(action);
    }
}

}