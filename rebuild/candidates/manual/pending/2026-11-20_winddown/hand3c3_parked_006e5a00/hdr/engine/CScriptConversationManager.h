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
class CScriptConversationManager {
public:
    CScriptConversationManager();                                              // 0x006E6150
#ifdef FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_REMOVECONVERSATION
    // Opt-in (hand3c3, row 0x006E5A00 CScriptConversation::FrameUpdate): Ego_r.pdb / Ego_d.pdb
    // ?RemoveConversation@CScriptConversationManager@@QAEXJ_N@Z, out of line in retail at 0x006E5990 (landed; FrameUpdate pushes 0
    // and its ConversationIndex, with Manager in ecx, at four exits). Declaration only.
    void RemoveConversation(long conversation_index, bool stop_speech);        // 0x006E5990
#endif
private:
    std::map<long, CScriptConversation> Conversations;                         // +0x00
    long NextConversationIndex;                                                // +0x0C
    bool AlwaysShowText;                                                       // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CScriptConversationManager) == 0x14);
#endif
