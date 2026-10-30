// CV_TalentlessBardScript::ProcessSongsMorality() -- adds the morality verse to the bard's song list by the hero's morality category: EVIL 0, BAD 1..2, NEUT 3, GOOD 4..5, PERFECT 6, nothing otherwise
// Leaf: ?ProcessSongsMorality@CV_TalentlessBardScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDA9B0, 248 bytes + a seven-entry jump table (row 288 with padding), plain `ret`.
// Callees: CGameScriptInterface slot 158 GetHeroMoralityCategory() (+0x278); CCharString(const char*, long) 0x0099EBF0,
//   ~CCharString 0x0099EAE0; std::vector<CCharString>::push_back(const CCharString&) 0x0044BFF0 (out of line).
// Strings: "TEXT_QST_163_MORAL_EVIL" 0x012F3B90, "..._BAD" 0x012F3B78, "..._NEUT" 0x012F3B60, "..._GOOD" 0x012F3B48,
//   "..._PERFECT" 0x012F3B2C.
// Shape: the sibling ProcessSongsRenown (landed 0x00EDAAD0): a dense switch whose case groups each pass the literal as
//   an IMPLICIT conversion temporary (one frame word per case, five words) and return through their own epilogue; the
//   last case falls into the shared exit used by the out-of-range default.
// Layout (retail): Game +0x40, SongItems +0x68 (landed ProcessSongsQuestComplete 0x00ED9B40).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];
};
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
};
namespace NScript {
class CV_TalentlessBardScript : public CQuestScriptObject {
public:
    unsigned char _flags48[0x20];                       // +0x48 .. stBard, BardWanderTime
    CCharStringVector SongItems;                        // +0x68
    void ProcessSongsMorality();
};
} // namespace NScript
using NScript::CV_TalentlessBardScript;
void NScript::CV_TalentlessBardScript::ProcessSongsMorality()
{
    switch (Game->GetHeroMoralityCategory())
    {
    case 0: SongItems.push_back("TEXT_QST_163_MORAL_EVIL");
        return;
    case 1:
    case 2: SongItems.push_back("TEXT_QST_163_MORAL_BAD");
        return;
    case 3: SongItems.push_back("TEXT_QST_163_MORAL_NEUT");
        return;
    case 4:
    case 5: SongItems.push_back("TEXT_QST_163_MORAL_GOOD");
        return;
    case 6: SongItems.push_back("TEXT_QST_163_MORAL_PERFECT");
        return;
    }
}