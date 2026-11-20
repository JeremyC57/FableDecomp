// CGameScriptInterface::AddLogBookEntry(const CWideString& name, const CWideString& abbreviated_name, const CWideString& text,
//   CHeroLogBook::CHeroLogBookEntry::ECategory category) const -- add an entry to the main player's hero log book (given = true)
// Leaf: ?AddLogBookEntry@CGameScriptInterface@@UBEXABVCWideString@@00W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088FE00, length 48 bytes.
// Callees: the player getter @ 0x00449970 (labelled "Catch@0040d959", propagated), CHeroLogBook::AddLogEntry @ 0x0047A9F0 (labelled "CHeroLogBook_AddLogEntry"). Shape: the four arguments re-read as the pushes shift the stack, `ret 10h`.
// NF176 (2026-11-20, q4255): the CHeroLogBook_Retail stand-in and the local CPlayer model are gone: engine/CHeroLogBook.h
//   (Ego_r.pdb / FableWin.pdb ?AddEntry@CHeroLogBook@@QAEXABVCWideString@@00W4ECategory@CHeroLogBookEntry@1@_N@Z, retail 0x0047A9F0) and
//   engine/CPlayer.h (FABLE_RETAIL_PLAYER: the inline FableWin.pdb ?GetPlayerHeroLogBook@CPlayer@@QBEAAVCHeroLogBook@@XZ = *PHeroLogBook_Data
//   +0xF8). The row's own leaf is the Ego_r.pdb 0x008CCB80 / FableWin.pdb 0x02A725F0 virtual const
//   ?AddLogBookEntry@CGameScriptInterface@@UBEXABVCWideString@@00W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@Z (was the stand-in
//   QBEXJJJJ@Z with four longs); the body passes the three string references and the category through unchanged.
struct CDisplayEngine_Retail;
class CWorld;
#define FABLE_RETAIL_PLAYER
#include "engine/CHeroLogBook.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                 World;                           // +0x04 (engine/CGameScriptInterface.h)
    long                    Component;
    CDisplayEngine_Retail*  DisplayEngine;                   // +0x0c
    long                    DefinitionManager;
    CPlayerManager*  PlayerManager;                   // +0x14
    unsigned char           _pad18[0x15];
    bool                    AllowScreenFadingIfAlreadyFaded; // +0x2d
    virtual void AddLogBookEntry(const CWideString& name, const CWideString& abbreviated_name, const CWideString& text,
                                 CHeroLogBook::CHeroLogBookEntry::ECategory category) const;   // vtable +0x134 (header slot 308 / 4)
};

void CGameScriptInterface::AddLogBookEntry(const CWideString& name, const CWideString& abbreviated_name, const CWideString& text,
                                           CHeroLogBook::CHeroLogBookEntry::ECategory category) const
{
    PlayerManager->GetMainPlayer().GetPlayerHeroLogBook().AddEntry(name, abbreviated_name, text, category, true);
}
