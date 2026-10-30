// CV_TalentlessBardScript::ProcessSongsMarriage() -- adds the marriage verses: BACHELOR if the hero never married, else MARRIED / DIVORCE / MURDER for each marriage fact that holds
// Leaf: ?ProcessSongsMarriage@CV_TalentlessBardScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDABA0, 287 bytes, plain `ret`.
// Callees: CGameScriptInterface slots 211 GetHeroHasMarried (+0x34c), 212 GetHeroHasCurrentMarriage (+0x350),
//   213 GetHeroHasDivorcedMarriage (+0x354), 215 GetHeroHasMurderedWife (+0x35c); CCharString(const char*, long)
//   0x0099EBF0, CCharString(const CCharString&) 0x0099EC30 (the placement copy), ~CCharString 0x0099EAE0;
//   vector<CCharString>::_M_insert_overflow 0x00433530, vector<CCharString>::push_back 0x0044BFF0.
// Strings: "TEXT_QST_163_WED_BACHELOR" 0x012F3C4C, "..._MARRIED" 0x012F3C30, "..._DIVORCE" 0x012F3C14, "..._MURDER" 0x012F3BFC.
// Shape: retail expands STLport push_back only at the first (bachelor) site -- placement copy under VC's null test,
//   `++_M_finish` re-read, `_M_insert_overflow(pos, x, __false_type(), 1, true)` with the tag in the frame's spare
//   byte -- and calls the out-of-line push_back at the other three; the one inline site is a `__forceinline` copy of
//   the STLport body. Every literal is an implicit conversion temporary sharing one frame word.
// Layout (retail): Game +0x40, SongItems +0x68 (landed ProcessSongsQuestComplete 0x00ED9B40).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
struct CCharStringVector {                              // STLport vector<CCharString> (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x);              // 0x0044BFF0 (the out-of-line copy)
    __forceinline void push_back_inlined(const CCharString& x)   // the same STLport body, expanded at one site
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
};
namespace NScript {
class CV_TalentlessBardScript : public CQuestScriptObject {
public:
    unsigned char _flags48[0x20];                       // +0x48 .. stBard, BardWanderTime
    CCharStringVector SongItems;                        // +0x68
    void ProcessSongsMarriage();
};
} // namespace NScript
using NScript::CV_TalentlessBardScript;
void NScript::CV_TalentlessBardScript::ProcessSongsMarriage()
{
    if (!Game->GetHeroHasMarried()) {
        SongItems.push_back_inlined("TEXT_QST_163_WED_BACHELOR");
        return;
    }
    if (Game->GetHeroHasCurrentMarriage())
        SongItems.push_back("TEXT_QST_163_WED_MARRIED");
    if (Game->GetHeroHasDivorcedMarriage())
        SongItems.push_back("TEXT_QST_163_WED_DIVORCE");
    if (Game->GetHeroHasMurderedWife())
        SongItems.push_back("TEXT_QST_163_WED_MURDER");
}