// CTB_Bard::GetRandomSpeech() -- rebuild the bard's song list from the hero's renown/morality/age/marriage/clothes/quests and pick a random song that differs from the last three (script-frame retry), remembering it
// Leaf: GetRandomSpeech
// Retail 0x00ED99D0, 368 bytes, `ret 4` (hidden CCharString result).
// Callees: CCharString() 0x0099E4B0, CCharString(const CCharString&) 0x0099EC30, operator= 0x0099EFB0, operator!= 0x0099E900
// (labelled less<CCharString>::operator()), ~CCharString 0x0099EAE0, vector<CCharString>::erase(first, last) 0x0043336A,
// CV_TalentlessBardScript::ProcessSongsRenown 0x00EDAAD0 / Morality 0x00EDA9B0 / Age 0x00EDA8C0 / Marriage 0x00EDABA0 /
// Clothes 0x00ED9BB0 / QuestComplete 0x00ED9B40, rand 0x00BFEB16, IsDone 0x00F35B30, CGameScriptInterface slot 7
// NewScriptFrame (+0x1c).
// Layout = Ego_r.pdb CTB_Bard (44 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
// first_time_talked_to +0x1C, given_gold +0x1D, lastRandomSpeech1..3 +0x20/+0x24/+0x28; CV_TalentlessBardScript::SongItems
// retail +0x68 (PDB 0x7C - 0x14, 12-byte vectors).
// Shape: an empty string local and a copy of it (the "no speech" result) precede the pick; the song count is the signed
// vector size; the retry is `while (!IsActiveThreadTerminating()) { pick; if (new) { found = true; break; } NewScriptFrame(); }` and the
// found handling sits after the loop behind `if (!found) return noSpeech;` (VC7.1 threads the flag away and lays the
// found block out before the shared copy-construct tail; handling it inside the loop moves it past the epilogue), with
// IsDone checks before and after the history rotation.
namespace NScript { class CPredicateBase; }
extern "C" int __cdecl rand();                             // 0x00BFEB16
class CCharString { public:
    void* PStringData;
    CCharString();                                         // 0x0099E4B0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool operator!=(const CCharString& other) const;       // 0x0099E900
};
struct CCharStringVector {                                 // STLport vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* erase(CCharString* first, CCharString* last);   // 0x0043336A
    void clear() { erase(_M_start, _M_finish); }
    long size() const { return _M_finish - _M_start; }
    CCharString& operator[](long i) { return _M_start[i]; }
};
struct CV_TalentlessBardScript {
    unsigned char     _pad00[0x68];
    CCharStringVector SongItems;                           // +0x68
    void ProcessSongsRenown();                             // 0x00EDAAD0
    void ProcessSongsMorality();                           // 0x00EDA9B0
    void ProcessSongsAge();                                // 0x00EDA8C0
    void ProcessSongsMarriage();                           // 0x00EDABA0
    void ProcessSongsClothes();                            // 0x00ED9BB0
    void ProcessSongsQuestComplete();                      // 0x00ED9B40
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CTB_Bard : public CEntityScriptBase {
public:
    void*                    __vftable;                    // +0x00
    CGameScriptInterface*    Game;                         // +0x04
    CScriptThing             Me;                           // +0x08
    CV_TalentlessBardScript* ParentClass;                  // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;                  // +0x18
    bool                     first_time_talked_to;         // +0x1c
    bool                     given_gold;                   // +0x1d
    CCharString              lastRandomSpeech1;            // +0x20
    CCharString              lastRandomSpeech2;            // +0x24
    CCharString              lastRandomSpeech3;            // +0x28
    CCharString GetRandomSpeech();
};

CCharString CTB_Bard::GetRandomSpeech()
{
    CCharString empty;
    CCharString noSpeech(empty);
    CCharString speech;
    ParentClass->SongItems.clear();
    ParentClass->ProcessSongsRenown();
    ParentClass->ProcessSongsMorality();
    ParentClass->ProcessSongsAge();
    ParentClass->ProcessSongsMarriage();
    ParentClass->ProcessSongsClothes();
    ParentClass->ProcessSongsQuestComplete();
    long count = ParentClass->SongItems.size();
    bool found = false;
    while (!IsActiveThreadTerminating()) {
        speech = ParentClass->SongItems[rand() % count];
        if (speech != lastRandomSpeech1 && speech != lastRandomSpeech2 && speech != lastRandomSpeech3) {
            found = true;
            break;
        }
        Game->NewScriptFrame();
    }
    if (!found)
        return noSpeech;
    if (IsActiveThreadTerminating())
        return noSpeech;
    lastRandomSpeech3 = lastRandomSpeech2;
    lastRandomSpeech2 = lastRandomSpeech1;
    lastRandomSpeech1 = speech;
    if (IsActiveThreadTerminating())
        return noSpeech;
    return speech;
}