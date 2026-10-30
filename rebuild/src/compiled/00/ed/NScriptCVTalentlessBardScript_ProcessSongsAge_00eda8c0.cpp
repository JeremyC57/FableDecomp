// NScript::CV_TalentlessBardScript::ProcessSongsAge() -- adds the age verse to the bard's song list: OLD above BardHeroAgeHigh, MEDIUM above BardHeroAgeMedium, YOUNG otherwise
// Leaf: ?ProcessSongsAge@CV_TalentlessBardScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDA8C0, 236 bytes (padded to 240), plain `ret`.
//   PDB name ?ProcessSongsAge@CV_TalentlessBardScript@NScript@@QAEXXZ.
// Callees: CGameScriptInterface slot 171 GetHeroAge() (+0x2ac, float); CCharString(const char*, long) 0x0099EBF0,
//   copy ctor 0x0099EC30, ~CCharString 0x0099EAE0; std::vector<CCharString>::push_back 0x0044BFF0 (out of line) and
//   its overflow helper _M_insert_overflow 0x00433530.
// Data: the script def 0x0143E90C (BardHeroAgeHigh +0x7d8, BardHeroAgeMedium +0x7dc: Ego_r.pdb 0x838/0x83c - 0x60).
// Strings: "TEXT_QST_163_AGE_OLD" 0x012F3B14, "TEXT_QST_163_AGE_MEDIUM" 0x012F3AFC, "TEXT_QST_163_AGE_YOUNG" 0x012F3AE4.
// Shape: STLport push_back sits in VC7.1's inline-once cost band (the lane inlines every site), so the first call site
//   uses a __forceinline copy of the same body and the other two call the out-of-line member; the first site is
//   expanded (placement copy with VC's null test, finish re-read and advanced, overflow helper with a __false_type
//   tag in a frame byte), the other two call the out-of-line copy; the age compares are `GetHeroAge() > (long)`
//   (`fild ; fcompp ; test ah,5 ; jp`).
// Layout (retail): Game +0x40, SongItems +0x68 (landed ProcessSongsQuestComplete 0x00ED9B40).
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
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x);               // 0x0044BFF0 (out of line)
    __forceinline void push_back_inline(const CCharString& x)   // the same STLport body, expanded at the first site
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CScriptDef { public:
    unsigned char _pad[0x7d8];
    long BardHeroAgeHigh;                               // +0x7d8
    long BardHeroAgeMedium;                             // +0x7dc
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
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
    void ProcessSongsAge();
};
void CV_TalentlessBardScript::ProcessSongsAge()
{
    CScriptDef* def = pGlobalScriptDef;
    if (Game->GetHeroAge() > def->BardHeroAgeHigh) {
        CCharString verse("TEXT_QST_163_AGE_OLD", -1);
        SongItems.push_back_inline(verse);
    }
    else {
        CScriptDef* def = pGlobalScriptDef;
        if (Game->GetHeroAge() > def->BardHeroAgeMedium) {
            CCharString verse("TEXT_QST_163_AGE_MEDIUM", -1);
            SongItems.push_back(verse);
        }
        else {
            CCharString verse("TEXT_QST_163_AGE_YOUNG", -1);
            SongItems.push_back(verse);
        }
    }
}
}