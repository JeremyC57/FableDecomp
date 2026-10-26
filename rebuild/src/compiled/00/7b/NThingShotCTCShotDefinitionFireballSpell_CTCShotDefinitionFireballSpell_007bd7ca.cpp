// NThingShot::CTCShotDefinitionFireballSpell::~CTCShotDefinitionFireballSpell() -- fades the shot's effect thing (the base's PEffect, +0xc) out if it is still alive, then the CTCShotDefinitionProjectile base is destroyed
// Leaf: ??1CTCShotDefinitionFireballSpell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BD7CA, length 64 bytes; called from the scalar deleting destructor at
// 0x004DCB23.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), IS_THING_ALIVE<CThing*>(CThing* const&) @ 0x0040DCE0
// (__fastcall; the manifest instantiation name "CThingBuilding*" is a folded twin), the __fastcall effect fade-out
// helper @ 0x0072AC40 (null/alive test, then the effect's interface-0x4B component; manifest
// "CTCPhysicalShieldSpell::FadeOutEffect"), ~CTCShotDefinitionProjectile @ 0x004DC9D2. Vtable 0x01241064 =
// NThingShot::CTCShotDefinitionFireballSpell.
// Layout: PEffect (CIntelligentPointer<CThing>) at +0xc (PDB CTCShotDefinitionBase +0x0c).
// Shape: a frame-pointer body (`push ebp ; mov ebp,esp ; ... leave`, so no tail jump to the base destructor) =
// `#pragma optimize("y", off)` + `("s", on)` (`leave`); the GetPItem result is a temporary bound to IS_THING_ALIVE's `const T&` (`[ebp-4]`).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CBaseObject;
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                    // +4
    CBaseObject* GetPItem() const;           // 0x00A01B50
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    virtual void v00();
    CThing* Get() const { return (CThing*)GetPItem(); }
};

bool __fastcall IS_THING_ALIVE(CThing* const& thing);   // 0x0040DCE0
void __fastcall FadeOutEffect(CThing* effect);          // 0x0072AC40

namespace NThingShot {

struct CTCShotDefinitionProjectile {
    virtual ~CTCShotDefinitionProjectile();  // 0x004DC9D2
    unsigned char              _pad04[8];
    CIntelligentPointer_CThing PEffect;      // +0x0c
    unsigned char              _body[0xa4 - 0x14];
};

struct CTCShotDefinitionFireballSpell : CTCShotDefinitionProjectile {
    virtual ~CTCShotDefinitionFireballSpell();
};

CTCShotDefinitionFireballSpell::~CTCShotDefinitionFireballSpell()
{
    if (IS_THING_ALIVE(PEffect.Get()))
        FadeOutEffect(PEffect.Get());
}

}