// CQ_SummoningTheShipScript::MakeBriarRoseComment(const CCharString& line) -- unless a conversation is running: restart the commentary timer, and if Briar Rose is alive start a new conversation of her line "TEXT_QST_B02_BRIARROSE_<line>" to the hero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF20F0, length 254 bytes, `ret 4`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, operator+(const char*, const
// CCharString&) 0x0099F690 (`__fastcall`, hidden result in ecx), ~CScriptThing 0x004AA840 (out of line), CScriptThing's
// IsAlive inline (pointee slot 75, +0x12c); the interface global 0x0143E8F8 SetTimer(long, long) (slot 89, +0x164,
// 0x006E76E0); through the game script interface (+0x40): IsConversationActive(long) (slot 368, +0x5c0, 0x008907D0),
// GetThingWithScriptName (slot 72, +0x120), AddNewConversation(const CScriptThing&, bool, bool) (slot 364, +0x5b0,
// 0x008906C0), GetHero (slot 70, +0x118), AddPersonToConversation(long, const CScriptThing&) (slot 365, +0x5b4,
// 0x00890710), AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&)
// (slot 366, +0x5b8, 0x00890750). Literals "STS_BriarRose" 0x012DE19C, "TEXT_QST_B02_BRIARROSE_" 0x012DE25C.
// Layout: Ego_r.pdb CQ_SummoningTheShipScript - 0x14: CommentaryTimer +0x54, ConversationIndex +0x58.
// Shape: the new conversation index is a named local (kept in edi after the member store); the interface vtable is
// cached in ebx across each GetHero call; the two false exits share one `xor al,al` tail.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right); // 0x0099F690 (hidden result in ecx)

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCPPointerInfo;

class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C
    CScriptThing*   PImp_Data;                         // +0x04 (the game-side thing)
    CCPPointerInfo* PImp_Info;                         // +0x08
    virtual ~CScriptThing();                                   // 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const { return PImp_Data && PImp_Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

class CTimer { public:
    long Index;
};

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04 .. +0x40
};

namespace NScript {
class CQ_SummoningTheShipScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0xc];
    CTimer                CommentaryTimer;           // +0x54
    long                  ConversationIndex;         // +0x58
    bool MakeBriarRoseComment(const CCharString& line);
};
} // namespace NScript
using NScript::CQ_SummoningTheShipScript;

bool NScript::CQ_SummoningTheShipScript::MakeBriarRoseComment(const CCharString& line)
{
    if (m_pInterface->IsConversationActive(ConversationIndex))
        return false;
    g_pGameScriptInterface->SetTimer(CommentaryTimer.Index, 15);
    CScriptThing briar = m_pInterface->GetThingWithScriptName("STS_BriarRose");
    if (!briar.IsAlive())
        return false;
    long conversation = m_pInterface->AddNewConversation(briar, false, false);
    ConversationIndex = conversation;
    m_pInterface->AddPersonToConversation(conversation, m_pInterface->GetHero());
    m_pInterface->AddLineToConversation(conversation, "TEXT_QST_B02_BRIARROSE_" + line, false, briar, m_pInterface->GetHero());
    return true;
}