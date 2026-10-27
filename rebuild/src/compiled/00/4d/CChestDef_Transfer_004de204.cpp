// CChestDef::Transfer(CPersistContext& context) -- persist every member without member names
// Leaf: ?Transfer@CChestDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE204, 79 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): slot 18 of vtable 0x0123ACDC (RTTI .?AVCChestDef@@; Ego_r.pdb ?Transfer@CChestDef@@UAEXAAVCPersistContext@@@Z);
//   the 6 Transfer calls take, in call order, the members at +0x28, +0x25, +0x2c, +0x30, +0x34, +0x38 with the per-type callees 0x00431102, 0x0043314A, 0x0043314A, 0x00431143, 0x00431102, 0x00431102, which are
//   Ego_r.pdb CChestDef's members OpenParticleEffect, PersistOnOpening, DisplayMessageOnEmpty, OpenAnimationForCreature, OpenerObject, OpenersRequired.
// Shape (family M050, representative 0x0046651B): `context.Transfer(Member);` per member; `optimize("s", on)` (each member
//   address formed with `lea eax,[esi+N]`, the last as `add esi,N`).
// Header debt: the def base is modelled by its retail size (pack(1), vptr + members) with the virtual Transfer only.
#pragma optimize("s", on)
class CCharString { public: void* PStringData; };
class CPersistContext {
public:
    template <class T> void Transfer(T& value);
};
#pragma pack(push, 1)
class CDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x25 - 4];
};
#pragma pack(pop)
class CChestDef : public CDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    bool                     PersistOnOpening;              // +0x25
    long                     OpenParticleEffect;            // +0x28
    bool                     DisplayMessageOnEmpty;         // +0x2c
    CCharString              OpenAnimationForCreature;      // +0x30
    long                     OpenerObject;                  // +0x34
    long                     OpenersRequired;               // +0x38
};
void CChestDef::Transfer(CPersistContext& context)
{
    context.Transfer(OpenParticleEffect);
    context.Transfer(PersistOnOpening);
    context.Transfer(DisplayMessageOnEmpty);
    context.Transfer(OpenAnimationForCreature);
    context.Transfer(OpenerObject);
    context.Transfer(OpenersRequired);
}