// NPlayerGui::CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo& prepare_info) -- when the main player's controlled creature is alive and its fishing component (interface 0xB1) has its stage initialised in stages 2..4: mark drawing, build the strain and distance texts, push the line-distance progress (0 in stages 2/3) and a strain colour (red = 255*strain, green = 255*(1-strain)) to the quest-info mini-game bar
// Grade: DIFFER(469v469) -- frame only: the two implicit CCharString temporaries share [ebp-0xc] (retail: [ebp-0xc] and [ebp-0x10]), so distance_text sits at [ebp-0x10] (retail [ebp-0x14]) and the frame is 0x14 (retail 0x18); plus the colour's constant B/A stores scheduled before the quest-info ecx load (retail after)
// Chunk 646 retry (2026-10-31) tried: moved onto the landed same-TU sibling model (0x0064A060 CDrawStatUpdates::Update: visible noinline const LowerBound + IS_THING_ALIVE<const CThingPlayerCreature*> template body, PDB GetMainPlayer()/PeekPControlledCreature, CDrawBase members) -- this packed the hero temporary, the GetTC key and strain_text into one slot as retail (was split); then explicit CCharString temporaries (474), an inline GetText(const CCharString&) member, non-const GetTextBySymbol return, a named distance symbol pointer, colour as a call temporary, a quest-info reference local, dropping optimize("s") (492), early return for the hero test -- none moved the char-temporary slots
// Leaf: ?Draw@CDrawFishingGame@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00649E8B, 469 bytes, ebp frame, `ret 4`; /Os TU (optimize("s") + "y" off, as the sibling).
//   Ego_r.pdb ?Draw@CDrawFishingGame@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature const 0x00487DD0, IS_THING_ALIVE<const
//   CThingPlayerCreature*> 0x0043DBF0, std::bitset<274>::test 0x00410DE0, CVectorMap LowerBound const 0x004365B0,
//   CTCFishing::GetPFishingRodDef const 0x007B0880, CCharString 0x0099EBF0 / 0x0099EAE0, NGameText::CDataBank::GetTextBySymbol
//   0x009C95E0, NGameText::ConstructString(CWideString&, long) 0x009C7B70, _ftol 0x00BFEA70, NPlayerGui::CDrawQuestInfo::
//   UpdateMiniGame(float, long, long) 0x00644EFD / UpdateMiniGame(const CRGBColour&) 0x00644F85, ~CWideString 0x0099B510.
// Layout: Ego_r.pdb CTCFishing (Stage +0x14, StageInitialised +0x18, Distance +0x2c, MaxDistance +0x30, Strain +0x34),
//   CFishingRodDef (MaxStrain +0x34, MaxLengthMetres +0x38), CPlayerGui::PDrawQuestInfo retail +0x1c8 (PDB +0x1d0),
//   CMainGameComponent::PTextBank +0x14; CDrawBase as the landed sibling.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
    const _Tp* end() const { return _M_finish; }
};
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                    // 0x00410DE0 (bitset<274>)
};
}
#include "engine/CPlayerManager.h"
#include "engine/CCountedPointer.h"
class CThing;
enum ETCInterfaceType { };
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    const value_type* LowerBound(const K& key) const;       // 0x004365B0
    std::vector<value_type> Data;
    Cmp  Compare;
    bool Dirty;
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> UsingComponents;                       // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char _pad_0x54[0x91 - 0x54];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    __forceinline const CTCBase* GetTC(ETCInterfaceType type) const
    {
        const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
        const ETCInterfaceType key = type;
        const std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        const std::pair<ETCInterfaceType, CTCBase*>* end = map->Data.end();
        return (it == end || type < it->first ? end : it)->second;
    }
};
template <class K, class V, class Cmp>
__declspec(noinline) const std::pair<K, V>* CVectorMap<K, V, Cmp>::LowerBound(const K& key) const   // 0x004365B0: the retail body, out of line
{
    long count = Data._M_finish - Data._M_start;
    const value_type* first = Data._M_start;
    while (count > 0) {
        long half = count / 2;
        const value_type* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count = count - half - 1;
        } else
            count = half;
    }
    return first;
}
class CThingPlayerCreature : public CThing { };
// IS_THING_ALIVE 0x0043DBF0 (T = const CThingPlayerCreature*): the header template, visible here and called out of line.
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !thing->Dead;
}
#pragma auto_inline(on)
class CPlayer {
public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
namespace NGameText {
bool __fastcall ConstructString(CWideString& text, long value);   // 0x009C7B70
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class CMainGameComponent {
public:
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;        // +0x14
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }
};
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CFishingRodDef {
public:
    unsigned char _pad00[0x34];
    float MaxStrain;                                        // +0x34
    float MaxLengthMetres;                                  // +0x38
};
enum EFishingStage { };
class CTCFishing : public CTCBase {
public:
    unsigned char _pad0c[0x14 - 0x0c];
    EFishingStage Stage;                                    // +0x14
    bool StageInitialised;                                  // +0x18
    unsigned char _pad19[0x2c - 0x19];
    float Distance;                                         // +0x2c
    float MaxDistance;                                      // +0x30
    float Strain;                                           // +0x34
    const CFishingRodDef* GetPFishingRodDef() const;        // 0x007B0880
};
namespace NPlayerGui {
class CDrawQuestInfo {
public:
    void UpdateMiniGame(float progress, long a, long b);    // 0x00644EFD
    void UpdateMiniGame(const CRGBColour& colour);         // 0x00644F85
};
}
class CPlayerGui {
public:
    unsigned char _pad_0x00[0x1c8];
    CCountedPointer<NPlayerGui::CDrawQuestInfo> PDrawQuestInfo;   // +0x1C8 (Ego_r.pdb +0x1D0)
};
class CEnginePrimitiveHandle;
class CWorld;
class CGraphicDataBank;
class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
class CDrawBase {
public:
    virtual ~CDrawBase();
    CPlayerGui*         PlayerGui;                          // +0x04
    CPlayerManager*     PlayerManager;                      // +0x08
    CWorld*             World;                              // +0x0C
    CMainGameComponent* Component;                          // +0x10
    CGraphicDataBank*   Bank;                               // +0x14
    bool                Drawing;                            // +0x18
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;   // +0x1C
};
class CDrawFishingGame : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& prepare_info);
};

void CDrawFishingGame::Draw(const CEnginePrimitivePrepareInfo& prepare_info)
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(hero) && hero->HasTC((ETCInterfaceType)0xb1)) {
        const CTCFishing* fishing = (const CTCFishing*)hero->GetTC((ETCInterfaceType)0xb1);
        if (fishing->StageInitialised) {
            Drawing = true;
            long stage = fishing->Stage;
            if (stage != 0 && stage >= 2 && stage <= 4) {
                const CFishingRodDef* rod = fishing->GetPFishingRodDef();
                CWideString strain_text = Component->GetTextBank().GetTextBySymbol("TEXT_GUI_FISHING_STRAIN");
                NGameText::ConstructString(strain_text, (long)fishing->Strain);
                float max_strain = fishing->Strain;
                if (rod)
                    max_strain = rod->MaxStrain;
                float strain = fishing->Strain / max_strain;
                if (strain > 1.0f)
                    strain = 1.0f;
                unsigned char red = (unsigned char)(strain * 255.0f);
                unsigned char green = (unsigned char)((1.0f - strain) * 255.0f);
                CWideString distance_text = Component->GetTextBank().GetTextBySymbol("TEXT_GUI_FISHING_DISTANCE");
                NGameText::ConstructString(distance_text, (long)fishing->Distance);
                float max_distance = fishing->Distance;
                if (rod)
                    max_distance = rod->MaxLengthMetres;
                if (fishing->Stage == 3 || fishing->Stage == 2)
                    PlayerGui->PDrawQuestInfo->UpdateMiniGame(0.0f, 0, 0);
                else
                    PlayerGui->PDrawQuestInfo->UpdateMiniGame(fishing->Distance / max_distance, 0, 0);
                CRGBColour colour(red, green, 0, 0xff);
                PlayerGui->PDrawQuestInfo->UpdateMiniGame(colour);
            }
        }
    }
}
}   // namespace NPlayerGui
