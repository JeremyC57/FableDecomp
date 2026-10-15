// CShopItemDefVector::resize(unsigned long newSize) -- the one-argument vector resize: grow or shrink to newSize, filling with a default-constructed CShopItemDef
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ED04D, length 48 bytes.
// Callees: CShopItemDef::CShopItemDef() @ 0x004D405A (landed), the two-argument resize(newSize, const CShopItemDef&)
// @ 0x004ED07D (element size 0x48: `(last-first)/0x48`, erase below / insert above; labelled
// "CArray<pair<ulong,CSoundPair>>::push_back", propagated), the def root destructor @ 0x009FC550 (labelled
// "~CMemoryBlock", propagated). The catalog label "CPersistContext::Transfer<CAnimationSet>" is propagated: `this` is the
// vector itself (ecx passed straight on) and the 0x48-byte temporary is a CShopItemDef (vtable 0x0123B644).
// Shape: a frame-pointer size TU (`sub esp,48h`, `push dword ptr [ebp+8]`, `leave`): `#pragma optimize("y", off)` +
// `("s", on)`. `resize(n, T())` builds the temporary in the frame, passes the constructor's returned `this` (eax), then the
// temporary's inline destructor chain: the CShopItemDef and intermediate vtable stores are dead and dropped, the
// CDefPointeeBase store (0x01230BA0) and the out-of-line root destructor call remain; `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CBase_009fc550 {
    virtual void v00();
    ~CBase_009fc550();                             // 0x009FC550 (labelled "~CMemoryBlock")
};
struct CDefPointeeBase : CBase_009fc550 {          // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }
};
struct CShopItemDef : CDefPointeeBase {            // vtable 0x0123B644, 0x48 bytes (engine/CShopItemDef.h, landed ctor)
    unsigned char _body[0x44];                     // +0x04 the def base fields, Type +0x28, the CShopItem member +0x2c..+0x48
    virtual void v00();
    CShopItemDef();                                // 0x004D405A
};

struct CShopItemDefVector {                        // retail std::vector<CShopItemDef>: first / last / end
    CShopItemDef* m_pFirst;
    CShopItemDef* m_pLast;
    CShopItemDef* m_pEnd;
    void resize(unsigned long newSize, const CShopItemDef& value);   // 0x004ED07D
    void resize(unsigned long newSize);
};

void CShopItemDefVector::resize(unsigned long newSize)
{
    resize(newSize, CShopItemDef());
}
