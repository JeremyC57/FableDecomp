// NThingShot::CTCShotDefinitionProjectile::CTCShotDefinitionProjectile(CThing*) -- CTCShotDefinitionBase(parent), own vtable, then the default-constructed Line1 primitive handle (+0x9c)
// Leaf: ??0CTCShotDefinitionProjectile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC991, length 49 bytes.
// Callees: NThingShot::CTCShotDefinitionBase::CTCShotDefinitionBase(CThing*) @ 0x007BF460, CBaseClass::CBaseClass @
// 0x0099A2D0 (the handle's base). Vtables 0x01240CCC = NThingShot::CTCShotDefinitionProjectile, 0x0122F598 =
// CEnginePrimitiveHandle (rtti-vtables.tsv). Callers: the FireballSpell (0x004DCAE9) and five sibling constructors.
// Layout: engine/CTCShotDefinitionProjectile.h (PDB) puts Line1 at +0xa0; the retail base CTCShotDefinitionBase is 0x9c
// bytes (12-byte HitThingsList vector, see the landed DrainLife constructor 0x004DB15E), so Line1 sits at +0x9c and the
// class is modelled locally.
// Shape: size TU (`push dword ptr [esp+0Ch]`, `and dword ptr [edi+4],0`) = `#pragma optimize("s", on)`; the handle is a
// member object with a `__forceinline` default constructor (a plain inline one is called out of line under "s"; CBaseClass() out of line, its own vtable, a null counted pointer from
// the embedded CIVCountedPointer's inline constructor), addressed through edi.
#pragma optimize("s", on)
class CThing;

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CIVCountedPointeeBase;

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
};

class CEnginePrimitiveHandle : public CBaseClass { public: // vtable 0x0122F598
    CIVCountedPointer Data;                  // +4
    __forceinline CEnginePrimitiveHandle() { }
};

namespace NThingShot {

class CTCShotDefinitionBase { public:
    virtual void v00();
    unsigned char _body[0x9c - 4];           // CTCBase + the shot fields (retail 0x9c bytes)
    CTCShotDefinitionBase(CThing* parent);   // 0x007BF460
};

struct CTCShotDefinitionProjectile : CTCShotDefinitionBase {
    CEnginePrimitiveHandle Line1;            // +0x9c (PDB +0xa0)
    virtual void v00();
    CTCShotDefinitionProjectile(CThing* parent);
};

CTCShotDefinitionProjectile::CTCShotDefinitionProjectile(CThing* parent)
    : CTCShotDefinitionBase(parent)
{
}

}