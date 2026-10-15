// CTCTavernBar::CTCTavernBar(CThing* owner) -- tavern-bar component constructor: CTCBase(owner), an empty jug vector, a null barman pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B9FC0, length 52 bytes (ret 4). Vtable 0x0126DC4C (CTCTavernBar).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10.
// Vtable 0x0123E7E0 = CIntelligentPointer (the barman pointer's own vtable). Created by the factory @ 0x004D5184.
// Ego_r.pdb CTCTavernBar (0x24): Jugs (vector<CIntelligentPointer<CThingObject>>) +0x0c, Barman
// (CIntelligentPointer<CThingAICreature>) +0x1c; the retail vector is 12 bytes, so Barman sits at +0x18 (sizeof 0x20).
// Shape: the vector's three zero stores follow the derived vtable store (a member object with an inline constructor,
// GOTCHAS wave 24i) and share the eax zero; the barman's address is formed in edi for its base constructor and its own
// vtable store. The vector is modelled as the real STLport structure (the landed 0x00547600 model): _M_start/_M_finish
// are _Vector_base initialisers and the end pointer an _STLP_alloc_proxy member built from (allocator(), 0) -- that is
// what schedules `mov ecx,edi` between the second and third zero store (a plain three-pointer init list puts it first).
struct CThing;
struct CBaseObject;
struct CThingObject;
struct CThingAICreature;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                     // +0x04
    bool    ToBeRemoved;                     // +0x08
    bool    IsInGlobalUpdateTCs;             // +0x09
    CTCBase(CThing* owner);                  // 0x00686800
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                    // +0x04
    CBaseIntelligentPointer();               // 0x00A01B10
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7E0 for T = CThingAICreature
    virtual void v00();
    CIntelligentPointer() { }
};

template<class T> struct CAllocator {        // std::allocator<T> (empty)
    CAllocator() { }
};

template<class T> struct CAllocProxy : CAllocator<T> {   // STLport _STLP_alloc_proxy<T*, T, allocator<T> >: the end pointer
    T* _M_data;
    CAllocProxy(const CAllocator<T>& a, T* p) : CAllocator<T>(a), _M_data(p) { }
};

template<class T> struct CVectorBase {       // STLport _Vector_base
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};

template<class T> struct CRetailVector : CVectorBase<T> {   // retail std::vector (12 bytes)
    explicit CRetailVector(const CAllocator<T>& a = CAllocator<T>()) : CVectorBase<T>(a) { }
};

struct CTCTavernBar : CTCBase {              // vtable 0x0126DC4C
    CRetailVector<CIntelligentPointer<CThingObject> > Jugs;   // +0x0c
    CIntelligentPointer<CThingAICreature> Barman;             // +0x18
    virtual void v00();
    CTCTavernBar(CThing* owner);
};

CTCTavernBar::CTCTavernBar(CThing* owner)
    : CTCBase(owner)
{
}
