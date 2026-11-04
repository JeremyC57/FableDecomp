// NScript::CQ_OpeningGraveyardSecretPassageScript::Init() -- register the three graveyard quest regions, then clear the parent's flag and the two state bools
// Leaf: ?Init@CQ_OpeningGraveyardSecretPassageScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC25B0, length 241 bytes, plain `ret` (vtable 0x012DA11C slot 3).
// Callees: CCharString::CCharString(const char*, long = -1) @ 0x0099EBF0 (x6), CGameScriptInterface slot 271
// AddQuestRegion(const CCharString&, const CCharString&) (+0x43c, 0x00891820, x3), CCharString::~CCharString
// @ 0x0099EAE0 (x6). Literals @ 0x012C4DD0 (quest) and 0x012D49B8 / 0x012DA130 / 0x012D49A8 (regions).
// Label: the propagated class name is doubtful -- this body sits in the vtable 0x012DA11C (no RTTI) stored by the
// ctor 0x00DC2570 (four CCharStrings at +0x58), not in CQ_OpeningGraveyardSecretPassageScript's (ctor 0x00DEC7F0); its slot-4
// transfer names +0x71 "QuestStartScreenShown" and +0x72 "HaveOpenedDemonDoor" (the graveyard part of
// Q_OpeningGraveyardSecretPassage).
// Layout (retail script-object base): interface +0x40, parent data +0x44 (its +0x11c byte is cleared).
// Shape: each call's literals are implicit conversion temporaries (region first: arguments right to left),
// destroyed in reverse after the call; one `xor al,al` feeds the three byte stores in source order.
// Naming wave 35: identity outer script vtable 012da11c slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00dc27c0 (slot 2) registers 14 entity scripts whose PDB Alloc outer is CQ_OpeningGraveyardSecretPassageScript; PDB ?Init@CQ_OpeningGraveyardSecretPassageScript@NScript@@; refs=1; replaces CQ_SecretPassageScript::Init: CQ_SecretPassageScript::Init is at 00dec8a0 (its own vtable slot) (wave3/labels_alloc.tsv); PDB ?Init@CQ_OpeningGraveyardSecretPassageScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CQ_SecretPassageScript@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own); class CQ_SecretPassageScript renamed CQ_OpeningGraveyardSecretPassageScript); same bytes.
class CCharString;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CGraveyardParentData {                        // the script's parent data object (+0x44)
    unsigned char _pad0[0x11c];
    bool m_bRegionScriptActive;                      // +0x11c
};

namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript {
public:
    unsigned char _base[0x3c];   // (-4: now the vptr of the virtual member)
    CGameScriptInterface*  m_pInterface;             // +0x40
    CGraveyardParentData*  m_pParent;                // +0x44
    unsigned char          _pad48[0x29];
    bool                   QuestStartScreenShown;    // +0x71
    bool                   HaveOpenedDemonDoor;      // +0x72
public:
    virtual void Init();                          // PDB ?Init@CQ_OpeningGraveyardSecretPassageScript@NScript@@UAEXXZ
};
}
using NScript::CQ_OpeningGraveyardSecretPassageScript;

void NScript::CQ_OpeningGraveyardSecretPassageScript::Init()
{
    m_pInterface->AddQuestRegion("Q_OpeningGraveyardSecretPassage", "Graveyard");
    m_pInterface->AddQuestRegion("Q_OpeningGraveyardSecretPassage", "GraveyardPath");
    m_pInterface->AddQuestRegion("Q_OpeningGraveyardSecretPassage", "GraveyardCircle");
    m_pParent->m_bRegionScriptActive = false;
    HaveOpenedDemonDoor = false;
    QuestStartScreenShown = false;
}