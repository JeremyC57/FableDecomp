// CSubDefClassBase::CSubDefClassBase() -- the definition's default constructor: CResource, the def base defaults, the vtable
// Leaf: ??0CSubDefClassBase@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C0C0, 29 bytes, thiscall, plain `ret`.
// Identity (proven): stores vtable 0x01231D54 (RTTI .?AVCSubDefClassBase@@); Ego_r.pdb CSubDefClassBase: no members of its own (the base of the sub-definitions: CSmashableDef, COpinionOfHeroDef, CActionUseDef ... call it).
// Callees: CResource::CResource() 0x009FBEC0.
// Shape (family M030, representative 0x0044C0C0): the inline `__declspec(novtable)` CDefClassBase constructor (flag bits
//   cleared in place, PDefManager zeroed; pack(1)), then this class's vtable store.
// Header debt: engine/CDefClassBase.h is a flat layout without the base chain / constructor.
class CDefinitionManager;
class CResource {
public:
    CResource();                                          // 0x009FBEC0
    virtual ~CResource();
    unsigned char _body[0x18];
};
#pragma pack(push, 1)
class __declspec(novtable) CDefClassBase : public CResource {
public:
    CDefClassBase() : PDefManager(0) { Setup = 0; Template = 0; DefaultValsApplied = 0; }
    CDefinitionManager* PDefManager;                      // +0x1c
    unsigned long GlobalIndex;                            // +0x20
    unsigned char Setup : 1;                              // +0x24
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;
};
#pragma pack(pop)
class CSubDefClassBase : public CDefClassBase {
public:
    CSubDefClassBase();
};
CSubDefClassBase::CSubDefClassBase()
{
}