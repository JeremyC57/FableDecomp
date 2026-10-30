// CTraderToRescue::OnPredicateFail() -- a trader to rescue died: when the script thing reports it was killed (by anything), the active quest is failed with the "TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED" message
// Leaf: OnPredicateFail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DFB100, length 131 bytes (padded to 144), `ret`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the empty literal @ 0x0122D70E), CScriptThing
// slot 18 MsgIsKilledBy(const CCharString&) (+0x48, 0x004AAAF0), ~CCharString @ 0x0099EAE0,
// CWideString::CWideString(const wchar_t*) @ 0x0099B6B0 (L"TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED" @ 0x012DEC88),
// CGameScriptInterface slot 655 GetActiveQuestName() (+0xa3c, by value) and slot 289 SetQuestAsFailed (+0x484:
// name, true, message, true), ~CWideString @ 0x0099B510.
// Layout = Ego_r.pdb CTraderToRescue (32 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass +0x14, PMasterData +0x18, BarIndex +0x1c.
// Shape: the killed-by string is a named local in its own block with the result in a bool declared outside (retail
// `lea edx ; push edx`); the message is a named local built before any argument push.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);       // 0x0099B6B0
    ~CWideString();                                // 0x0099B510
};
extern const unsigned short g_TextQuestFailedTradersDied[];   // 0x012DEC88 L"TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptThing { public:                              // vtable 0x01238C8C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48, 0x004AAAF0)
    long m_data[2];
};
struct CQ_TraderConflictGoodScript;
struct CQ_SunnyvaleMasterData;
class CTraderToRescue {
public:
    void*                        __vftable;        // +0x00
    CGameScriptInterface*        Game;             // +0x04
    CScriptThing                 Me;               // +0x08
    CQ_TraderConflictGoodScript* ParentClass;      // +0x14
    CQ_SunnyvaleMasterData*      PMasterData;      // +0x18
    long                         BarIndex;         // +0x1c
    void OnPredicateFail();
};
void CTraderToRescue::OnPredicateFail()
{
    bool killed;
    {
        CCharString anyone("");
        killed = Me.MsgIsKilledBy(anyone);
    }
    if (killed)
    {
        CWideString message(g_TextQuestFailedTradersDied);
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, message, true);
    }
}