// NScript::CV_TalentlessBardScript::ProcessSongsClothes() -- adds the clothing verse to the bard's song list: wearing nothing, dressed as a woman, in Will robes, in armour or as an apprentice (first that applies)
// Grade: DIFFER(298v306) -- one instruction: retail zeroes its one frame word (`mov [esp+4],0`, the CCharString temporary's slot) at entry before IsHeroNaked; everything else matches.
// Leaf: ?ProcessSongsClothes@CV_TalentlessBardScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED9BB0, 306 bytes, plain `ret`. PDB ?ProcessSongsClothes@CV_TalentlessBardScript@NScript@@QAEXXZ.
// Callees: CGameScriptInterface slot 188 IsHeroNaked() (+0x2F0); IsHeroDressedAsAWoman 0x00EDA5E0, IsHeroDressedInWillRobe
//   0x00EDA0C0, IsHeroDressedInArmour 0x00ED9CF0, IsHeroDressedAsApprentice 0x00EDA490; CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; vector<CCharString>::push_back 0x0044BFF0.
// Layout (retail): Game +0x40, SongItems +0x68 (landed ProcessSongsQuestComplete 0x00ED9B40 / ProcessSongsRenown 0x00EDAAD0).
class CCharString {
public:
    class CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class vector;
template <> class vector<CCharString, allocator<CCharString> > { public:   // STLport vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void push_back(const CCharString& x);               // 0x0044BFF0
};
}
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TalentlessBardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _flags48[0x20];                       // +0x48 .. stBard, BardWanderTime
    std::vector<CCharString, std::allocator<CCharString> > SongItems;   // +0x68
    bool IsHeroDressedInArmour();                       // 0x00ED9CF0
    bool IsHeroDressedInWillRobe();                     // 0x00EDA0C0
    bool IsHeroDressedAsApprentice();                   // 0x00EDA490
    bool IsHeroDressedAsAWoman();                       // 0x00EDA5E0
    void ProcessSongsClothes();
};

void CV_TalentlessBardScript::ProcessSongsClothes()
{
    if (Game->IsHeroNaked()) {
        if (IsActiveThreadTerminating())
            return;
        SongItems.push_back("TEXT_QST_163_WEAR_NOTHING");
    } else if (IsHeroDressedAsAWoman()) {
        if (IsActiveThreadTerminating())
            return;
        SongItems.push_back("TEXT_QST_163_WEAR_DRESS");
    } else if (IsHeroDressedInWillRobe()) {
        if (IsActiveThreadTerminating())
            return;
        SongItems.push_back("TEXT_QST_163_WEAR_WILL");
    } else if (IsHeroDressedInArmour()) {
        if (IsActiveThreadTerminating())
            return;
        SongItems.push_back("TEXT_QST_163_WEAR_ARMOUR");
    } else if (IsHeroDressedAsApprentice()) {
        if (IsActiveThreadTerminating())
            return;
        SongItems.push_back("TEXT_QST_163_WEAR_APPRENTICE");
    }
}
}   // namespace NScript
