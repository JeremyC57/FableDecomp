// NUISystem::CSlider::CSlider(void* def) -- front-end slider: CSprite base from the def, this class's three vtables, the action vector and the action-list / arrow / accept-button pointers zeroed, then Initialize()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054C050, length 102 bytes (`ret 4`).
// Callees: CSprite::CSprite(def) @ 0x0041B800, CSlider::Initialize @ 0x0054B4B0. Vtables (rtti-vtables.tsv NUISystem::CSlider):
// 0x01248D04 at +0, 0x01248CE0 at +4 (CObserver sub-object), 0x01248CD8 at +0x18.
// Layout: Ego_r.pdb CSlider (0x220) members sit 0x30 lower in retail (Actions PDB +0x1f0 -> +0x1c0, 12-byte retail vector);
// PreviousValue (+0x1e0) is not initialised by the ctor. Base model as the landed NUISystem::CMouseCursor ctor 0x0055C650.
// Shape: one zero register for the init-list zero members (vector ctor first), `mov ecx,esi` scheduled among the stores.
// Leaf: ??0CSlider
struct IPrimary_0054c050 { virtual void v00(); };
struct CObserver {                                 // the secondary base at +4 (0x14 bytes)
    virtual void a00();
    unsigned char _pad04[0x10];
};
struct ISecondaryB_0054c050 { virtual void b00(); };
struct CSprite : IPrimary_0054c050, CObserver, ISecondaryB_0054c050 {
    CSprite(void* def);                            // 0x0041B800
};

namespace NUISystem {
class CAction;
class CComponent;
struct CActionPointer { CAction* Data; void* Info; };
struct CActionVector {                             // STLport vector<CCountedPointer<CAction>>, 12 bytes
    CActionPointer* _M_start;
    CActionPointer* _M_finish;
    CActionPointer* _M_end_of_storage;
    CActionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CActionList;
enum EActionType { ACTION_NONE = 0 };

class CSlider : public CSprite { public:                         // vtables 0x01248D04 / 0x01248CE0 / 0x01248CD8
    unsigned char _pad01c[0x1c0 - 0x1c];
    CActionVector Actions;                         // +0x1c0 (PDB +0x1f0)
    CActionList*  PActionsOnBack;                  // +0x1cc
    CActionList*  PActionsOnSelect;                // +0x1d0
    EActionType   PDefAction;                      // +0x1d4
    CComponent*   PLeftArrow;                      // +0x1d8
    CComponent*   PRightArrow;                     // +0x1dc
    float         PreviousValue[2];                // +0x1e0 (C2DVector)
    CComponent*   PAcceptButton;                   // +0x1e8
    virtual void v00();
    virtual void a00();
    virtual void b00();
    CSlider(void* def);
    void Initialize();                             // 0x0054B4B0
};
}

NUISystem::CSlider::CSlider(void* def)
    : CSprite(def),
      PActionsOnBack(0),
      PActionsOnSelect(0),
      PDefAction(ACTION_NONE),
      PLeftArrow(0),
      PRightArrow(0),
      PAcceptButton(0)
{
    Initialize();
}