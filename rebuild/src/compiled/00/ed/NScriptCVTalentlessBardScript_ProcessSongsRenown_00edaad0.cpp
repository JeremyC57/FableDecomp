// NScript::CV_TalentlessBardScript::ProcessSongsRenown() -- adds the renown verse to the bard's song list by the hero's renown level: NONE for 0..2, SOME for 3..5, LOTS for 6..7, nothing otherwise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDAAD0, 162 bytes + an eight-entry jump table (row 208), plain `ret`.
//   PDB name ?ProcessSongsRenown@CV_TalentlessBardScript@NScript@@QAEXXZ.
// Callees: CGameScriptInterface slot 162 GetHeroRenownLevel() (+0x288); CCharString(const char*, long) 0x0099EBF0,
//   ~CCharString 0x0099EAE0; std::vector<CCharString>::push_back(const CCharString&) 0x0044BFF0 (out of line).
// Strings: "TEXT_QST_163_RENOWN_NONE" 0x012F3BE0, "TEXT_QST_163_RENOWN_SOME" 0x012F3BC4, "TEXT_QST_163_RENOWN_LOTS" 0x012F3BA8.
// Shape: a dense switch whose three case groups each pass the literal as an IMPLICIT conversion temporary (each gets
//   its own frame word -- a named local per case block shares one word under the RTM compiler -- and is re-addressed
//   with `lea` for the push) and return through their own epilogue; the default arm falls out to the shared exit.
// Layout (retail): Game +0x40, SongItems +0x68 (landed ProcessSongsQuestComplete 0x00ED9B40).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CCharStringVector {                              // STLport vector<CCharString> (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void push_back(const CCharString& x);               // 0x0044BFF0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_TalentlessBardScript { public:
    unsigned char _base[0x40];
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    unsigned char _flags48[0x20];                       // +0x48 .. stBard, BardWanderTime
    CCharStringVector SongItems;                        // +0x68
    void ProcessSongsRenown();
};
void CV_TalentlessBardScript::ProcessSongsRenown()
{
    switch (Game->GetHeroRenownLevel())
    {
    case 0:
    case 1:
    case 2: SongItems.push_back("TEXT_QST_163_RENOWN_NONE");
        return;
    case 3:
    case 4:
    case 5: SongItems.push_back("TEXT_QST_163_RENOWN_SOME");
        return;
    case 6:
    case 7: SongItems.push_back("TEXT_QST_163_RENOWN_LOTS");
        return;
    }
}
}