// CQ_OrchardFarmRaidScript::MakeTeamMemberComment(const CCharString& line, const CScriptThing& speaker, bool unused) -- if the comment timer has run out, start a conversation in which the speaker says "<TextSystemScriptCode><speaker data>_<line>" to the hero, then restart the timer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCDA80, length 251 bytes, `ret 0xc` (the third argument is unused).
// Callees: the interface global 0x0143E8F8 GetTimer(long) (slot 90, +0x168, 0x006E7710) and SetTimer(long, long) (slot 89,
// +0x164, 0x006E76E0); CScriptBase::IsActiveThreadTerminating 0x00CB7940 (result unused); through the game script
// interface (+0x40): AddNewConversation (slot 364, +0x5b0), GetHero (slot 70, +0x118, x2), AddPersonToConversation (slot
// 365, +0x5b4), AddLineToConversation (slot 366, +0x5b8); CScriptThing::GetDataString (virtual slot 3, +0xc, by value);
// operator+(const CCharString&, const CCharString&) 0x0099F570 and operator+(const CCharString&, const char*) 0x0099F600
// (both `__fastcall`, hidden result in ecx); ~CCharString 0x0099EAE0 (x4). Literal "_" 0x01244DB4.
// Layout: Ego_r.pdb CQ_OrchardFarmRaidScript - 0x14, -4 after CrateList: CommentTimer +0x4c, TextSystemScriptCode +0x84.
// Shape: the conversation index is a named local (ebx); the line is one `code + data + "_" + line` expression whose
// last temporary lands in the dead `speaker` argument slot (the reference is held in edi).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570 (hidden result in ecx)
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600 (hidden result in ecx)

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    virtual const CCharString& GetName() const;        // slot 1 (+0x4), 0x004AA8B0
    virtual CCharString GetDefName() const;            // slot 2 (+0x8), 0x004AA8D0
    virtual CCharString GetDataString() const;         // slot 3 (+0xc), 0x004AA900
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

class CTimer { public:
    long Index;
};

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

namespace NScript {
class CQ_OrchardFarmRaidScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    long                  CrateCount;                // +0x48
    CTimer                CommentTimer;              // +0x4c
    unsigned char         _pad50[0x34];
    CCharString           TextSystemScriptCode;      // +0x84
    bool MakeTeamMemberComment(const CCharString& line, const CScriptThing& speaker, bool unused);
};
} // namespace NScript
using NScript::CQ_OrchardFarmRaidScript;

bool NScript::CQ_OrchardFarmRaidScript::MakeTeamMemberComment(const CCharString& line, const CScriptThing& speaker, bool unused)
{
    if (g_pGameScriptInterface->GetTimer(CommentTimer.Index) > 0) {
        IsActiveThreadTerminating();
        return false;
    }
    long conversation = m_pInterface->AddNewConversation(speaker, false, false);
    m_pInterface->AddPersonToConversation(conversation, m_pInterface->GetHero());
    m_pInterface->AddLineToConversation(conversation, TextSystemScriptCode + speaker.GetDataString() + "_" + line, false, speaker, m_pInterface->GetHero());
    g_pGameScriptInterface->SetTimer(CommentTimer.Index, 5);
    return true;
}