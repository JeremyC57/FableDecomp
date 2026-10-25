// NThingShot::CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun(CThing& thing) -- NThingShot::CTCShotDefinitionProjectile(thing), then this class's vtable and TrapCreated = false
// Leaf: ??0CTCShotDefinitionNymphFun@NThingShot@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCA62, 31 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x01240E3C.
// Identity: NThingShot::CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun(CThing&) -- proven; evidence: it stores vtable 0x01240E3C whose RTTI is .?AVCTCShotDefinitionNymphFun@NThingShot@@ : NThingShot::CTCShotDefinitionProjectile : NThingShot::CTCShotDefinitionBase : CTCBase;
//   FableWin.pdb name set holds ??0CTCShotDefinitionNymphFun@NThingShot@@QAE@AAVCThing@@@Z; its only caller is the landed type factory 0x004DEACF (NThingShot::CTCShotDefinitionNymphFun alloc). Family G056 (seven such constructors; the propagated label
//   "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" is right only for 0x004DCA62).
// Callees: NThingShot::CTCShotDefinitionProjectile::CTCShotDefinitionProjectile(CThing&) 0x004DC991 (landed callers bind ??0CTCShotDefinitionProjectile@@QAE@PAUCThing@@@Z; the PDB is NThingShot:: and CThing&).
// Layout: Ego_r.pdb CTCShotDefinitionNymphFun::TrapCreated (bool) at retail +0xa4 (Ego_r +0xA8: the retail projectile base is 4 bytes smaller).
#pragma optimize("s", on)
class CThing;
namespace NThingShot {
class CTCShotDefinitionProjectile {                                     // retail 0xA4 bytes
public:
    CTCShotDefinitionProjectile(CThing& thing);                       // 0x004DC991
    virtual ~CTCShotDefinitionProjectile();
    unsigned char _body[0xa4 - 4];
};
}
namespace NThingShot {
class CTCShotDefinitionNymphFun : public CTCShotDefinitionProjectile {
public:
    CTCShotDefinitionNymphFun(CThing& thing);
    bool TrapCreated;                    // +0xa4
};
}
NThingShot::CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun(CThing& thing)
    : CTCShotDefinitionProjectile(thing), TrapCreated(false)
{
}