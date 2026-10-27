// CSmashableDef::CSmashableDef() -- default constructor: the CSubDefClassBase base, the vtable, then the CDefIndex member's default
// Leaf: ??0CSmashableDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C213, 22 bytes, thiscall, plain `ret` (size-optimised module).
// Identity (proven): stores vtable 0x01231F7C (RTTI .?AVCSmashableDef@@); Ego_r.pdb CSmashableDef: ReplacementObject (CDefIndex) at +0x28.
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0.
// Shape (family M031, representative 0x0044C213): `optimize("s", on)` -- the base constructor out of line, the vtable store,
//   then the member's inline default constructor as a size-form store (`and dword ptr [esi + 0x28], 0`).
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
class CDefIndex { public: CDefIndex() : Val(0) { } long Val; };   // Ego_r.pdb { long Val; }; FableWin.pdb ??0CDefIndex@@QAE@XZ
class CSmashableDef : public CSubDefClassBase {
public:
    CSmashableDef();
    bool Smashable;                                       // +0x25
    CDefIndex ReplacementObject;                          // +0x28
    long SmashParticleEmitter;                            // +0x2c
};
CSmashableDef::CSmashableDef()
{
}