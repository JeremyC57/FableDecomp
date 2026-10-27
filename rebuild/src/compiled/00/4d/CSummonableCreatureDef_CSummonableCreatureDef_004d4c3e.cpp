// CSummonableCreatureDef::CSummonableCreatureDef() -- default constructor: the CSubDefClassBase base, the vtable, then the CDefString member's default
// Leaf: ??0CSummonableCreatureDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4C3E, 22 bytes, thiscall, plain `ret` (size-optimised module).
// Identity (proven): stores vtable 0x0123C3A4 (RTTI .?AVCSummonableCreatureDef@@); Ego_r.pdb CSummonableCreatureDef: SummonedBrain (CDefString) at +0x2c after the float Rank.
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0.
// Shape (family M032, representative 0x0044C253): `optimize("s", on)` -- the base constructor out of line, the vtable store,
//   then the member's inline default constructor as a size-form store (`or dword ptr [esi + 0x2c], 0xffffffff`).
#pragma optimize("s", on)
class CDefinitionManager;
class CResource {                                         // vptr + CResource members (Ego_r.pdb, 0x1c bytes)
public:
    virtual ~CResource();
    unsigned char _body[0x18];
};
#pragma pack(push, 1)
class __declspec(novtable) CDefClassBase : public CResource {
public:
    CDefinitionManager* PDefManager;                      // +0x1c
    unsigned long GlobalIndex;                            // +0x20
    unsigned char Setup : 1;                              // +0x24
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
};
#pragma pack(pop)
class CSubDefClassBase : public CDefClassBase {
public:
    CSubDefClassBase();                                   // 0x0044C0C0
};
class CDefString { public: CDefString() : TablePos(-1) { } long TablePos; };   // Ego_r.pdb { long TablePos; }
class CSummonableCreatureDef : public CSubDefClassBase {
public:
    CSummonableCreatureDef();
    float Rank;                                           // +0x28
    CDefString SummonedBrain;                             // +0x2c
};
CSummonableCreatureDef::CSummonableCreatureDef()
{
}