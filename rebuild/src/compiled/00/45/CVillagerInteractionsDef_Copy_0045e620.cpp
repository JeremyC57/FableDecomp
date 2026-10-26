// CVillagerInteractionsDef::Copy(const CParentDefClassBase& other) -- virtual copy (vtable 0x01233F24 slot 19): base assignment, then the two animation def-strings and the distance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E620, length 37 bytes. Pragma: optimize("s") (the def module copies every member through eax).
// Leaf: Copy
// Callee: CParentDefClassBase::operator= @ 0x00431ED0. PDB CVillagerInteractionsDef: Animation1, Animation2 (CDefString), Distance
// (float) -- here at +0x3c/+0x40/+0x44 (the retail CParentDefClassBase ends at +0x3c, the PDB's 4 bytes higher), so the base is local.
// Shape: no `mov eax,esi` -- the slot returns void (eax keeps the last copied word). Written as an explicit memberwise copy: the
// compiler-generated operator= is emitted OUT of line under optimize("s") and this body becomes a `jmp` thunk (as CPlayerInventoryDef's slot 19).
#pragma optimize("s", on)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x38];                                            // +0x04..+0x3c
    CParentDefClassBase& operator=(const CParentDefClassBase& other);     // 0x00431ED0
};
class CDefString { public: long TablePos; };
class CVillagerInteractionsDef : public CParentDefClassBase { public:
    CDefString Animation1;      // +0x3c
    CDefString Animation2;      // +0x40
    float      Distance;        // +0x44
    virtual void v00();
    virtual void Copy(const CParentDefClassBase& other);
};
void CVillagerInteractionsDef::Copy(const CParentDefClassBase& other)
{
    const CVillagerInteractionsDef& source = static_cast<const CVillagerInteractionsDef&>(other);
    CParentDefClassBase::operator=(source);
    Animation1 = source.Animation1;
    Animation2 = source.Animation2;
    Distance = source.Distance;
}