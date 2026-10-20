// CShopDef::CShopDef() -- default constructor of the shop definition: the definition base (0x0044C0C0), the CShopDef vtable, an empty shopkeeper mesh-type string, the two empty stock vectors and the two attitude price-multiplier maps (the scalar tuning fields are left for the loader)
// Retry 10: RELOCATION_MATCH -- the vector modelled as STLport _Vector_base (m_pEnd an _STLP_alloc_proxy with its own ctor); three plain pointers schedule the map's `lea` too early.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E06C3, length 62 bytes.
// Leaf: ??0CShopDef
// Callees: the definition base constructor @ 0x0044C0C0 (vtable 0x01231D54; manifest "CShoveHaPennyRegionDef"
// propagated), CCharString::CCharString() @ 0x0099E4B0 (counted default string), the attitude map constructor
// @ 0x004DF92A (a VC6 std::map with default comparator/allocator temporaries) twice; vtable 0x01241F9C (CShopDef).
// Shape: base call, `lea ecx,[esi+3Ch]` hoisted above the vtable store, the string constructor, `xor eax,eax`
// and six zero stores (two inline VC6 vector constructors, 12 bytes each), the two map constructors on +0x64 /
// +0x74, `mov eax,esi`, `ret`. Retail CShopDef is 0xd0 bytes (CShopDef_GetSizeofClass 0x004E0701), the PDB's
// 0xe0 shrinks by 4 per vector and per map.
// Tried: the vectors as inline-constructed members (this form), the same under optimize("s"), the six words as
// plain init-list members (the `lea` moves above all six stores).
// The manifest label "CSystemManager::CSystemManager" is propagated; the vtable says CShopDef.
// The definition base: polymorphic, 0x28 bytes, its constructor out of line.
class CDefBase_0044c0c0 {
public:
    CDefBase_0044c0c0();                           // 0x0044C0C0
    virtual ~CDefBase_0044c0c0();
    unsigned char _body[0x28 - 4];
};
class CCharString {
public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};
template <class P> struct CAllocProxy {      // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    P p;
    CAllocProxy(P v) : p(v) { }
    operator P() const { return p; }
    CAllocProxy& operator=(P v) { p = v; return *this; }
};
struct CShopItemVector {
    void* m_pFirst;
    void* m_pLast;
    CAllocProxy<void*> m_pEnd;
    CShopItemVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
};
class CAttitudeMultiplierMap {
public:
    unsigned char _tree[12];
    CAttitudeMultiplierMap();                      // 0x004DF92A
};
class CShopDef : public CDefBase_0044c0c0 {
public:
    unsigned long          Name;
    unsigned long          InitialHawkingPhrase;
    unsigned long          ReplyToRequestPhrase;
    unsigned long          CustomerFollowPhrase;
    unsigned long          ThankingPhrase;
    CCharString            ShopkeeperMeshType;
    CShopItemVector        DefaultStock;
    CShopItemVector        StockCategoryDefaults;
    float                  BuyPriceMultiplier;
    float                  SellPriceMultiplier;
    float                  AttitudeBuyPriceMultiplierDefault;
    CAttitudeMultiplierMap AttitudeBuyPriceMultiplier;
    float                  AttitudeSellPriceMultiplierDefault;
    CAttitudeMultiplierMap AttitudeSellPriceMultiplier;
    unsigned char          _rest[0xd0 - 0x80];
    CShopDef();
    virtual ~CShopDef();
};
CShopDef::CShopDef()
{
}