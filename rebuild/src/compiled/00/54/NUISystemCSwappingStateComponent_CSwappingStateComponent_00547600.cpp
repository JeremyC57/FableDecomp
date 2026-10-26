// NUISystem::CSwappingStateComponent::CSwappingStateComponent(long owner) -- CChangingStateComponent(owner), the three vtables of this class, the empty SwappingStates vector (three null pointers), then GetDataFromDef()
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547600, 66 bytes. Manifest label "CScrollBarOutside::CScrollBarOutside"
// is propagated; the vtables 0x012485AC / 0x01248584 / 0x0124857C are NUISystem::CSwappingStateComponent
// (rtti-vtables.tsv).
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50, GetDataFromDef
// @ 0x00547500 (on `this`, in the body; manifest "CList::GetDataFromDef" is propagated).
//
// Shape: base constructor, `xor eax,eax`, the three vtable stores, the vector's inline constructor, then
// the body call. The vector is STLport: _M_start/_M_finish are plain initialisers of _Vector_base and the
// end pointer is the _STLP_alloc_proxy member object built from (allocator, 0) -- that member constructor
// is what puts `mov ecx,esi` between the second and third zero store (a three-pointer vector with one
// init list, with or without the defaulted allocator argument, schedules `mov ecx,esi` before all three).
// Retail layout = Ego_r.pdb minus 0x30: SwappingStates (vector<pair<unsigned long, float>>; the retail
// vector is three pointers, the PDB's 16 bytes) +0x15C, LastSwapTime +0x168, Seed +0x16C.
// Leaf: ??0CSwappingStateComponent

namespace NUISystem {

struct CComponentInterface {                     // primary vtable at +0
    virtual void v000();
};

struct CObserver_Retail {                        // vtable at +4, then its 0x10 bytes of state
    virtual void ProcessEvent();
    unsigned char _body[0x10];
};

struct CEventSinkInterface {                     // vtable at +0x18
    virtual void v0();
};

struct SSwapState {                              // std::pair<unsigned long, float>
    unsigned long State;
    float Time;
};

struct CSwapStateAllocator {                     // std::allocator<std::pair<unsigned long, float>> (empty)
    CSwapStateAllocator() {}
};

struct CSwapStateAllocProxy : CSwapStateAllocator {   // STLport _STLP_alloc_proxy<T*, T, allocator<T>>: the end pointer
    SSwapState* _M_data;
    CSwapStateAllocProxy(const CSwapStateAllocator& a, SSwapState* p) : CSwapStateAllocator(a), _M_data(p) {}
};

struct CSwapStateVectorBase {                    // STLport _Vector_base
    SSwapState* _M_start;
    SSwapState* _M_finish;
    CSwapStateAllocProxy _M_end_of_storage;
    CSwapStateVectorBase(const CSwapStateAllocator& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) {}
};

struct CSwapStateVector : CSwapStateVectorBase { // std::vector<std::pair<unsigned long, float>> (retail 12 bytes)
    explicit CSwapStateVector(const CSwapStateAllocator& a = CSwapStateAllocator()) : CSwapStateVectorBase(a) {}
};

class CChangingStateComponent : public CComponentInterface, public CObserver_Retail, public CEventSinkInterface { public:   // retail 0x15C bytes
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long owner);         // 0x0052CC50
};

class CSwappingStateComponent : public CChangingStateComponent { public:   // vtables 0x012485AC / 0x01248584 / 0x0124857C
    virtual void v000();
    virtual void ProcessEvent();
    virtual void v0();
    CSwapStateVector SwappingStates;             // +0x15C
    float            LastSwapTime;               // +0x168
    unsigned long    Seed;                       // +0x16C
    CSwappingStateComponent(long owner);
    void GetDataFromDef();                       // 0x00547500
};

CSwappingStateComponent::CSwappingStateComponent(long owner)
    : CChangingStateComponent(owner)
{
    GetDataFromDef();
}

}