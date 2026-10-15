#pragma optimize("s", on)
// CTCTrapBase::CTCTrapBase(CThing*) -- CTCBase(owner), own vtable, three empty vectors and an empty obstruction pointer, the trap opener, a null trap def, Active = true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078918D, length 94 bytes. Size TU (`push dword ptr [esp+10h]`
// pass-through of the owner): `optimize("s",on)`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is a propagated label;
// every CTC constructor calls it), CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (then the
// CIntelligentPointer vtable 0x01238C6C), CFadeInFadeOutBase::CFadeInFadeOutBase() @ 0x00436560 (manifest
// "CTCHeroStats::SoberUp" is propagated). Vtable 0x0126B7FC = CTCTrapBase.
// Shape: all zero stores share ebx; the vectors (STLport _Vector_base: _M_start/_M_finish initialisers and the end
// pointer in an _STLP_alloc_proxy member built from an allocator temporary -- that member constructor is what
// schedules `mov ecx,edi` / `lea ecx,[esi+38h]` after the second zero store of the preceding vector) and the CDefPointer have inline
// constructors so their stores follow the vtable store; Active (+0x58) is stored after Triggered / LastTriggerOnProximity,
// so it is a body assignment. TrapType (+0x5C) is left uninitialised.
// Layout: CTCTrapBase.h is the PDB layout (16-byte vectors); retail vectors are 12 bytes, which moves every later
// member 4 bytes per vector (PObstruction +0x24, TriggerThings +0x2C, TrapOpener +0x38, PTrapDef +0x54, Active +0x58),
// so a local model is used.
struct CThing;
struct CBaseObject;

struct CTCBase {
    virtual void v00();
    CThing*       Thing;                    // +0x04
    unsigned char _pad_0x08[0x4];
    CTCBase(CThing* thing);                 // 0x00686800
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                   // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {          // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CRetailAllocator {                   // std::allocator<T> (empty)
    CRetailAllocator() { }
};

struct CRetailAllocProxy : CRetailAllocator {   // STLport _STLP_alloc_proxy<T*, T, allocator<T> >: the end pointer
    void* _M_data;
    CRetailAllocProxy(const CRetailAllocator& a, void* p) : CRetailAllocator(a), _M_data(p) { }
};

struct CRetailVectorBase {                  // STLport _Vector_base
    void*             _M_start;
    void*             _M_finish;
    CRetailAllocProxy _M_end_of_storage;
    CRetailVectorBase(const CRetailAllocator& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};

struct CRetailVector : CRetailVectorBase {  // std::vector<T> (retail 12 bytes)
    explicit CRetailVector(const CRetailAllocator& a = CRetailAllocator()) : CRetailVectorBase(a) { }
};

struct CFadeInFadeOutBase {
    unsigned char _body[0x1C];
    CFadeInFadeOutBase();                   // 0x00436560
};

struct CTrapDef;

struct CDefPointerTrapDef {
    const CTrapDef* Object;
    CDefPointerTrapDef() : Object(0) { }
};

struct CTCTrapBase : CTCBase {
    CRetailVector       HitThings;          // +0x0C vector<pair<CIntelligentPointer<CThing>, long> >
    CRetailVector       ImmuneThings;       // +0x18
    CIntelligentPointer PObstruction;       // +0x24
    CRetailVector       TriggerThings;      // +0x2C
    CFadeInFadeOutBase  TrapOpener;         // +0x38
    CDefPointerTrapDef  PTrapDef;           // +0x54
    bool                Active;             // +0x58
    bool                Triggered;          // +0x59
    bool                LastTriggerOnProximity;  // +0x5A
    long                TrapType;           // +0x5C ETrapType
    virtual void v00();
    CTCTrapBase(CThing* thing);
};

CTCTrapBase::CTCTrapBase(CThing* thing)
    : CTCBase(thing), Triggered(false), LastTriggerOnProximity(false)
{
    Active = true;
}