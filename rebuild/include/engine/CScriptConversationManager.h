#ifndef FABLE_ENGINE_CSCRIPTCONVERSATIONMANAGER_H
#define FABLE_ENGINE_CSCRIPTCONVERSATIONMANAGER_H
// Chunk 2051 (row 0x004A6E30 CWorld::Init). Ego_r.pdb CScriptConversationManager (Dinkumware 24 bytes; retail STLport 0x14, the
// allocation in Init 0x004A7439 `push 14h`): Conversations map<long, CScriptConversation> +0x00, NextConversationIndex +0x0C,
// AlwaysShowText +0x10. Non-polymorphic (FableWin ??_GCScriptConversationManager@@QAEPAXI@Z, ??1 QAE). Retail's
// boost::checked_delete<CScriptConversationManager> 0x004AE810 expands the map's destructor in place (clear via _M_erase
// 0x004AE490, then the header node freed) -- no user destructor.
#include "rebuild_abi.h"
#include "stlport/fable_map.h"
#include "engine/CScriptConversation.h"
#ifdef FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_ADDLINETOCONVERSATION
class CThing;
class CCharString;
#endif
class CScriptConversationManager {
public:
    CScriptConversationManager();                                              // 0x006E6150
#ifdef FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_ADDLINETOCONVERSATION
    // Per-TU opt-in (NF202, q12195; rows 0x006E5950 self / 0x00890750 caller): Ego_r.pdb 0x0070EE80 + FableWin.pdb 0x02409E00
    // ?AddLineToConversation@CScriptConversationManager@@QAEXJABVCThing@@PBV2@ABVCCharString@@_N@Z. Declaration only.
    void AddLineToConversation(long conversation, const CThing& speaker, const CThing* listener, const CCharString& line,
                               bool play_during_cut_scenes);                  // 0x006E5950
#endif
#ifdef FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_REMOVECONVERSATION
    // Opt-in (NF433, row 0x008907C0 CGameScriptInterface::RemoveConversation): Ego_r.pdb / FableWin.pdb
    // ?RemoveConversation@CScriptConversationManager@@QAEXJ_N@Z, out of line in retail at 0x006E5990 (the row's tail jump). Declaration only.
    void RemoveConversation(long conversation, bool remove_now);               // 0x006E5990
#endif
private:
    std::map<long, CScriptConversation> Conversations;                         // +0x00
    long NextConversationIndex;                                                // +0x0C
    bool AlwaysShowText;                                                       // +0x10
};
FABLE_STATIC_ASSERT_AT(19, sizeof(CScriptConversationManager) == 0x14);
#endif
