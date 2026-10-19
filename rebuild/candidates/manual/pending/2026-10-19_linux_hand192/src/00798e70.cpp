// CTCShopKeeper::CTCShopKeeper(CThing*) -- constructor: thing-component base, empty customer and shop pointers, three empty previous-customer slots, cleared customer index / shop-occupancy state / graphic / opinion flag, then the customer list emptied
// Leaf: ??0CTCShopKeeper
// Retail: Fable.exe (TLC, VC7.1) @ 0x00798E70, 213 bytes (+ int3 padding), `ret 4`.
// Label correction: the catalog's "CEngineSubPrimitive2DClockSprite::Constructor" is propagated; the vtable stored is
//   0x0126C154 = CTCShopKeeper (rtti-vtables.tsv), and 0x00798E10 is CTCShopKeeper::EmptyCustomerList.
// Callees: CTCBase::CTCBase(CThing*) 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 / GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90 / ~CBaseIntelligentPointer 0x00A01C10, malloc 0x00BFEA0E, EmptyCustomerList 0x00798E10.
//   Vtables: CIntelligentPointer<CThingCreatureBase> 0x0123E7D8, CIntelligentPointer<CThing> 0x01238C6C.
// Layout (retail; Ego_r.pdb with the 12-byte retail vector): Customer +0xc, Shop +0x14, PreviousCustomers +0x1c,
//   CustomerIndex +0x28, OtherPeopleInShop +0x2c, FrameLastCalcedOtherPeopleInShop +0x30, Graphic +0x34 (CEngineGraphic,
//   14 bytes, 2-byte packed), RepressedOpinions +0x42 (0x44 bytes).
// Shape: PreviousCustomers is the STLport `explicit vector(3)`: _Vector_base zeroes with an _STLP_alloc_proxy end pointer
//   (the `push 18h` between them), the allocation, then uninitialized_fill_n of a T() temporary (built after the allocation)
//   through the intelligent pointer's copy ctor (base ctor, vtable, `SetPItem(other.GetPItem())`), the temporary destroyed
//   by the base dtor (no user dtor on CIntelligentPointer); the scalars are init-list members.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CThing;
struct CThingCreatureBase;

struct CTCBase {                                       // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                               // +0x04
    bool    ToBeRemoved;                               // +0x08
    bool    IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* owner);                            // 0x00686800
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                     // +0x04
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    CThing* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct SCustomerAllocProxy {                           // STLport _STLP_alloc_proxy: the end-of-storage pointer
    CIntelligentPointer<CThingCreatureBase>* _M_data;
    SCustomerAllocProxy(CIntelligentPointer<CThingCreatureBase>* data) : _M_data(data) { }
};
struct CCustomerVector {                               // vector<CIntelligentPointer<CThingCreatureBase>> (12 bytes)
    CIntelligentPointer<CThingCreatureBase>* _M_start;
    CIntelligentPointer<CThingCreatureBase>* _M_finish;
    SCustomerAllocProxy                      _M_end_of_storage;
    static __forceinline CIntelligentPointer<CThingCreatureBase>* uninitialized_fill_n(
        CIntelligentPointer<CThingCreatureBase>* first, unsigned long n, const CIntelligentPointer<CThingCreatureBase>& value)
    {
        CIntelligentPointer<CThingCreatureBase>* cur = first;
        for (; n > 0; --n, ++cur)
            new (cur) CIntelligentPointer<CThingCreatureBase>(value);
        return cur;
    }
    __forceinline explicit CCustomerVector(unsigned long n)
        : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = (CIntelligentPointer<CThingCreatureBase>*)malloc(n * sizeof(CIntelligentPointer<CThingCreatureBase>));
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
        _M_finish = uninitialized_fill_n(_M_start, n, CIntelligentPointer<CThingCreatureBase>());
    }
};

#pragma pack(push, 2)
struct CEngineGraphic {                                // Ego_r.pdb: 14 bytes (2-byte packed)
    unsigned long BankIndex;                           // +0x00
    float         AnimStep;                            // +0x04
    float         RenderSizeX;                         // +0x08
    bool          AdditiveAlpha;                       // +0x0c
    unsigned char Type;                                // +0x0d
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { }
};
#pragma pack(pop)

struct CTCShopKeeper : CTCBase {                       // vtable 0x0126C154
    CIntelligentPointer<CThingCreatureBase> Customer;  // +0x0c
    CIntelligentPointer<CThing>             Shop;      // +0x14
    CCustomerVector                         PreviousCustomers;   // +0x1c
    long                                    CustomerIndex;       // +0x28
    bool                                    OtherPeopleInShop;   // +0x2c
    long                                    FrameLastCalcedOtherPeopleInShop;   // +0x30
    CEngineGraphic                          Graphic;   // +0x34
    bool                                    RepressedOpinions;   // +0x42
    virtual void v00();
    void EmptyCustomerList();                          // 0x00798E10
    CTCShopKeeper(CThing* owner);
};

CTCShopKeeper::CTCShopKeeper(CThing* owner)
    : CTCBase(owner), PreviousCustomers(3), CustomerIndex(0), OtherPeopleInShop(false), FrameLastCalcedOtherPeopleInShop(0),
      RepressedOpinions(false)
{
    EmptyCustomerList();
}
