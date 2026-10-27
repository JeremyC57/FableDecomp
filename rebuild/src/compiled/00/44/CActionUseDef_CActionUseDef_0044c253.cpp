// CActionUseDef::CActionUseDef() -- default constructor: the CSubDefClassBase base, the vtable, then the CDefString member's default
// Leaf: ??0CActionUseDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C253, 22 bytes, thiscall, plain `ret` (size-optimised module).
// Identity (proven): stores vtable 0x0123200C (RTTI .?AVCActionUseDef@@); Ego_r.pdb CActionUseDef: AnimationGroup (CDefString) at +0x28.
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0.
// Shape (family M032, representative 0x0044C253): `optimize("s", on)` -- the base constructor out of line, the vtable store,
//   then the member's inline default constructor as a size-form store (`or dword ptr [esi + 0x28], 0xffffffff`).
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
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE = 0 };
class CActionUseDef : public CSubDefClassBase {
public:
    CActionUseDef();
    CDefString AnimationGroup;                            // +0x28
    bool SnapToActionPoint;                               // +0x2c
    bool IgnoreActionPoints;                              // +0x2d
    ETutorialCategory TutorialCategory;                   // +0x30
};
CActionUseDef::CActionUseDef()
{
}