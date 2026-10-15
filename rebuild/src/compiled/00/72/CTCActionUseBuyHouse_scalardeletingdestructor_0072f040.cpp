// CTCActionUseBuyHouse::`scalar_deleting_destructor'(unsigned int) -- leave the global update list, release both def pointers, ~CTCBase, free if asked
// Leaf: ??_GCTCActionUseBuyHouse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072F040, length 91 bytes.
// Callees: CTCBase::RemoveFromGlobalTCs @ 0x00686DF0 (landed; `this` in ecx), the def objects' virtual
// slot 1 (`call [eax+4]` / `call [edx+4]`) when a release drops the count to zero, CTCBase::~CTCBase @
// 0x00686830, operator delete @ 0x00BFE9BC. Own vtable 0x01266A7C = CTCActionUseBuyHouse (rtti-vtables.tsv);
// the constructor is 0x0072E690.
//
// Layout = Ego_r.pdb CTCActionUseBuyHouse: CTCActionUseBase 0x14 {CTCBase 0xc, CDefPointer<CActionUseDef>
// PActionUseDef +0x0c, Usable +0x10}, CDefPointer<CBuyHouseDef> PDef +0x14, WaitingForConfirmation +0x18,
// HouseQuestion +0x1c.
// Shape: the deleting destructor with the whole destructor chain inlined: own vtable, the body's
// RemoveFromGlobalTCs, PDef's release, then the intermediate base's destructor (no vtable store, only
// PActionUseDef's release) and the out-of-line root destructor; `add esp,4` after operator delete.
// The `??_G` is pulled into the TU by a `new` factory (the vtable is emitted where the inline
// constructor stores it); the release and the destructors are `__forceinline` so the deleting
// destructor does not collapse to `call ??1`. Left implicit, the intermediate base's destructor is
// emitted out of line in this TU (it inlines in 0x007EBDD0's): it is written as an empty
// `__forceinline` destructor of a `__declspec(novtable)` model so it inlines without a vtable store.
struct CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
    virtual ~CTCBase_Calls();                               // 0x00686830
    void RemoveFromGlobalTCs();                             // 0x00686DF0
};

struct CDefPointeeBase {                                    // counted definition object
    virtual void Destroy();                                 // slot 0
    virtual void Delete();                                  // slot 1
    long m_refCount;                                        // +4
};

template<class T>
struct CDefPointer {
    T* m_pDef;
    CDefPointer() : m_pDef(0) { }
    __forceinline ~CDefPointer()
    {
        T* def = m_pDef;
        if (def != 0) {
            if (--def->m_refCount == 0)
                def->Delete();
            m_pDef = 0;
        }
    }
};

struct __declspec(novtable) CTCActionUseBase : CTCBase_Calls {   // 0x14 bytes
    CDefPointer<CDefPointeeBase> PActionUseDef;             // +0x0c
    bool                         Usable;                    // +0x10
    CTCActionUseBase(CThing* owner);                        // 0x004DA939
    __forceinline ~CTCActionUseBase() { }
};

struct CTCActionUseBuyHouse : CTCActionUseBase {            // vtable 0x01266A7C
    CDefPointer<CDefPointeeBase> PDef;                      // +0x14
    bool                         WaitingForConfirmation;    // +0x18
    long                         HouseQuestion;             // +0x1c
    CTCActionUseBuyHouse(CThing* owner) : CTCActionUseBase(owner), WaitingForConfirmation(false) { }
    __forceinline virtual ~CTCActionUseBuyHouse() { RemoveFromGlobalTCs(); }
};

CTCActionUseBuyHouse* CreateActionUseBuyHouse(CThing* owner)
{
    return new CTCActionUseBuyHouse(owner);
}