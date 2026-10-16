// CTCInventoryWeapons::OpenInventory() -- opens the base inventory, sets the inventory animation render box from the inventory def's box position and extents, then tells the front-end manager the screen opened
// Leaf: OpenInventory@CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2F27, length 131 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::OpenInventory() @ 0x005BD4EA (explicit base call), CTCInventoryBase::SetInventoryAnimRenderBox(
// const C2DBoxF&) @ 0x005BC58F, the def's slot 1 on release, NUISystem::CFrontEndManager::GetInstance() @ 0x0041E5F2 (__fastcall)
// and its virtual slot 15 (+0x3C) taking the screen (name descriptive).
// Shape: the box is built from two by-value GetPDef() temporaries in one full expression (arguments right to left: the
// extents' copy in edi first, the position's in esi second), each an add-ref with a null test; the 2-float vectors copy as
// dword pairs into the box at [ebp-10h] through a user (forceinline) C2DVector copy ctor (`lea ecx,[esi+0A4h]` address forms); the temporaries release in reverse order (null test, `dec [reg+4] ; jne`, slot 1).
// Layout: retail CTCInventoryBase PDef +0x68 (CDefPointer<const CInventoryDef>); the weapons box position/extents at
// CInventoryDef +0xA4/+0xAC (retail offsets; names descriptive).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector {
    float X, Y;
    __forceinline C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
};
struct C2DBoxF {
    C2DVector TL;
    C2DVector Extents;
    __forceinline C2DBoxF(const C2DVector& tl, const C2DVector& extents) : TL(tl), Extents(extents) {}
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
struct CInventoryDef : CDefPointeeBase {
    unsigned char _pad08[0xa4 - 0x08];
    C2DVector WeaponsAnimBoxTLPos;                // +0xa4
    C2DVector WeaponsAnimBoxExtents;              // +0xac
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};

struct CTCInventoryBase_Weapons {
    unsigned char _pad00[0x68];
    CDefPointer<CInventoryDef> PDef;            // +0x68
    __forceinline CDefPointer<CInventoryDef> GetPDef() const { return PDef; }
    void OpenInventory();                       // 0x005BD4EA
    void SetInventoryAnimRenderBox(const C2DBoxF& box);   // 0x005BC58F
};
struct CTCInventoryWeapons;
struct CFrontEndManager {
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
    virtual void OnScreenOpened(CTCInventoryWeapons* screen);   // slot 15 (+0x3C)
    static CFrontEndManager* __fastcall GetInstance();          // 0x0041E5F2
};
struct CTCInventoryWeapons : CTCInventoryBase_Weapons {
    void OpenInventory();
};

void CTCInventoryWeapons::OpenInventory()
{
    CTCInventoryBase_Weapons::OpenInventory();
    SetInventoryAnimRenderBox(C2DBoxF(GetPDef()->WeaponsAnimBoxTLPos, GetPDef()->WeaponsAnimBoxExtents));
    CFrontEndManager::GetInstance()->OnScreenOpened(this);
}
