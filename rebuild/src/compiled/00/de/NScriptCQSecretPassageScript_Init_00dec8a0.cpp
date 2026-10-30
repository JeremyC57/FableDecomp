// NScript::CQ_SecretPassageScript::Init() -- register the five regions of the Secret Passage quest (Q_SecretPassage): GraveyardSecretPassage_1, KrakenChamber, PrisonSecretPassage, PrisonSecretPassageExterior, PrisonTortureChamber
// Leaf: ?Init@CQ_SecretPassageScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEC8A0, 366 bytes, thiscall, plain `ret`.
// Identity (proven): the row is referenced once, from slot 3 (Init) of the RTTI-less quest-script vtable 0x012DDC30
//   (0x012DDC3C), which the landed NScript::CQ_SecretPassageScript::CQ_SecretPassageScript 0x00DEC7F0 stores (slots: 0 dtor
//   0x00DF10B0, 1 RegisterMain 0x00DECA10, 2 Main 0x00DECAD0, 3 Init, 4 OnPersist 0x00CBD4E0 = the CScriptBase default);
//   Ego_r.pdb ?Init@CQ_SecretPassageScript@NScript@@UAEXXZ. The quest name literal "Q_SecretPassage" 0x012DDC44 is the
//   one the factory registration 0x00CD74E8 uses. Catalog label sub_00dec8a0.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCharString::~CCharString 0x0099EAE0; through Game
//   (+0x40, Ego_r CGameScriptInterfaceBase*) slot 271 AddQuestRegion (+0x43c; ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "Q_SecretPassage" 0x012DDC44; regions 0x012DB938, 0x012DB928, 0x012DB914, 0x012DB8F8, 0x012DB8E0.
// Shape: the landed CQ_HangingTreeEvilScript::Init 0x00D68FF0 model -- each literal goes straight to a `const CCharString&`
//   parameter; VC builds the region temporary first (right-to-left) and destroys the quest-name temporary first.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
class CScriptBase { public:                             // retail 0x40 bytes (Ego_r 0x54: debug containers)
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
};

namespace NScript {
class CQ_SecretPassageScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40 (Ego_r CLevelScriptBase<T>::Game)
    void*                 Data;                         // +0x44
    virtual void Init();
};

void CQ_SecretPassageScript::Init()
{
    Game->AddQuestRegion("Q_SecretPassage", "GraveyardSecretPassage_1");
    Game->AddQuestRegion("Q_SecretPassage", "KrakenChamber");
    Game->AddQuestRegion("Q_SecretPassage", "PrisonSecretPassage");
    Game->AddQuestRegion("Q_SecretPassage", "PrisonSecretPassageExterior");
    Game->AddQuestRegion("Q_SecretPassage", "PrisonTortureChamber");
}
}   // namespace NScript