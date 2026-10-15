// CCreatureAction_ShopkeeperGetOut::CCreatureAction_ShopkeeperGetOut(const CCreatureAction_ShopkeeperGetOut&) -- copy constructor: base CCreatureActionBase(other), own vtable, then the shop, goods id, position and orientation words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C170, length 159 bytes (+ int3 padding).
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0. Own vtable 0x01273FCC (rtti-vtables.tsv).
// Shape: the eleven words +0xa8..+0xd0 are copied one by one through eax/ecx/edx at fixed offsets with no base register, so the
// position and orientation are modelled as scalar members copied in the init list (a C3DVector / CRightHandedSet struct member, implicit
// or hand-copied, is copied through `lea` base pointers; a C3DVector user copy constructor goes through the x87). `ret 4`.
// Layout as PerformAction (0x00842D40): TCShop +0xa8, Goods +0xac, Pos +0xb0, RHSet {Up, Forward} +0xbc (PDB names; the PDB header sits
// 0x0c bytes high against retail's 0xa8-byte CCreatureActionBase).
struct CTCShop;

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};
struct CCreatureAction_ShopkeeperGetOut : CCreatureActionBase {   // vtable 0x01273FCC; retail sizeof 0xd4
    CTCShop* TCShop;                        // +0xa8
    long Goods;                             // +0xac
    float PosX, PosY, PosZ;                 // +0xb0 C3DVector Pos
    float UpX, UpY, UpZ;                    // +0xbc CRightHandedSet RHSet.Up
    float ForwardX, ForwardY, ForwardZ;     // +0xc8 CRightHandedSet RHSet.Forward
    virtual void v00();
    CCreatureAction_ShopkeeperGetOut(const CCreatureAction_ShopkeeperGetOut& other);
};

CCreatureAction_ShopkeeperGetOut::CCreatureAction_ShopkeeperGetOut(const CCreatureAction_ShopkeeperGetOut& other)
    : CCreatureActionBase(other)
    , TCShop(other.TCShop)
    , Goods(other.Goods)
    , PosX(other.PosX), PosY(other.PosY), PosZ(other.PosZ)
    , UpX(other.UpX), UpY(other.UpY), UpZ(other.UpZ)
    , ForwardX(other.ForwardX), ForwardY(other.ForwardY), ForwardZ(other.ForwardZ)
{
}