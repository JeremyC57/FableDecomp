// CTCContainerRewardHero::RewardHeroWithItems() -- give the controlled hero the container's reward: its gold through CTCHeroStats::AddMoney and its items through CTCHeroReceiveItems::GiveItems; an empty container shows TXT_SIGN_NOTHING as a subtitle (unless the chest def turns that off) and reports false
// Leaf: ?RewardHeroWithItems@CTCContainerRewardHero@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074EE90, 484 bytes, thiscall plain `ret`, returns bool. Callers 0x007507F6 / 0x0081235B /
//   0x00898D35. Ego_r.pdb ?RewardHeroWithItems@CTCContainerRewardHero@@QAE_NXZ (the catalog label is right); it is the only
//   caller besides 0x0074EC77 of GetRewardItems 0x0074E860 (FableWin/Ego_r ?GetRewardItems@CTCContainerRewardHero@@IAEX...@Z).
// Callees (PDB spellings): CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetPControlledCreature 0x00487DC0 (`add ecx,2Ch ; jmp GetPItem`: Ego_r CPlayer::ControlledCreature +0x2C),
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (non-const, out of line here; interfaces 0xCB
//   CTCHeroReceiveItems and 4 CTCHeroStats), CTCContainerRewardHero::GetRewardItems 0x0074E860,
//   CDefClassBase::GetSubDef<CChestDef> 0x0074F7D0, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, CTCBase::PeekComponent
//   0x00686D60, CSubtitleRenderer::SetText 0x00492E4B, CTCHeroStats::AddMoney 0x0057B327, CTCHeroReceiveItems::GiveItems
//   0x007CA9E0, ~vector<pair<long, CCharString> > 0x004E34B2, CCharString 0x0099EBF0 / 0x0099EAE0, ~CWideString 0x0099B510.
// Layout: Ego_r CMainGameComponent PTextBank +0x14 (CCountedPointer<NGameText::CDataBank>), PCutSceneMainFont +0x1614C
//   (CCountedPointer<CFontBank>); CPlayerGui SubtitleRenderer +0x18; CChestDef DisplayMessageOnEmpty +0x2C; CThing interface
//   bits +0x20, interface map +0x44, PBaseDef +0x70, dead bit +0x91 bit 0.
// Globals: g_pMainGameComponent 0x013B86A0 (GOTCHAS wave 25), g_pPlayerGui 0x013B8790 (majority spelling; descriptive).
// Shape: the retail TU compiles CVectorMap::LowerBound (landed 0x0040F020 text) and GetRewardItems ahead of this body, so
//   both are same-TU non-inline bodies under auto_inline(off): LowerBound's register facts keep the zero in ebx across
//   the lookup (the receiver spills to 0x18), and the GetRewardItems stand-in (its real first two steps only: gold = 0,
//   items.erase(begin, end)) homes the chest def and the subtitle temporaries in the dead gold/key slots. `gold <= 0 &&
//   items.empty()` first, then the gold arm, then the items arm gives retail's jump-threaded block order; the hero
//   stats lookup is the never-written-local form (its miss reads the receiver slot, as retail does); the font and the
//   text bank come through the Ego_r inline accessors CMainGameComponent::PeekCutSceneMainFont / GetTextBank.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"

class CCharStringData;
class CCharString { public:
    CCharString(const char* text, long length = -1);       // 0x0099EBF0 ??0CCharString@@QAE@PBDJ@Z
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
class CWideStringData;
class CWideString { public:
    ~CWideString();                                         // 0x0099B510
    CWideStringData* PStringData;
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Vector_base<_Tp, _Alloc> _Base;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector();                                              // <pair<long, CCharString> > 0x004E34B2 (out of line)
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
    iterator erase(iterator __first, iterator __last);      // <pair<long, CCharString> > 0x0074FB20 (out of line)
};
} // namespace std

class CTCBase;
enum ETCInterfaceType { };
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* _M_end_of_storage;                          // +0x08
    unsigned char Compare;                                  // +0x0C
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#pragma auto_inline(off)
CInterfaceMap::value_type* CInterfaceMap::LowerBound(const ETCInterfaceType& key)
{
    long count = _M_finish - _M_start;
    value_type* first = _M_start;
    while (count > 0) {
        long half = count / 2;
        value_type* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count = count - half - 1;
        } else
            count = half;
    }
    return first;
}
#pragma auto_inline(on)

class CThingBaseDef : public CDefClassBase { };
class CChestDef : public CSubDefClassBase { public:            // RTTI .?AVCChestDef@@ : CSubDefClassBase
    static CCharString __fastcall StatGetTypeName() { return CCharString("CChestDef", -1); }
    bool PersistOnOpening;                                  // +0x25
    long OpenParticleEffect;                                // +0x28
    bool DisplayMessageOnEmpty;                             // +0x2C
};

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const   // <CChestDef> 0x0074F7D0
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long Components_present[9];                    // +0x20 (std::bitset of interface types)
    CInterfaceMap Components;                               // +0x44
    unsigned char _pad54[0x70 - 0x54];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char ThingFlags;                               // +0x91 (bit 0: dead)
    bool IsDead() const { return (ThingFlags & 1) != 0; }
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    __forceinline unsigned long HasTC(long type) const
    {
        return Components_present[type >> 5] & (1UL << (type & 31));
    }
    template <class T> __forceinline T* GetTC(long type)
    {
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound((ETCInterfaceType)type);
        return (T*)((it == map->_M_finish || type < it->first) ? map->_M_finish : it)->second;
    }
    template <class T> __forceinline T* PeekTCOrUnset(long type)
    {
        T* component;
        if (HasTC(type))
            component = GetTC<T>(type);
        return component;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                               // 0x00449970
};
class CFontBank;
class CMainGameComponent;
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;   // +0x14
    unsigned char _pad1c[0x1614C - 0x1C];
    CCountedPointer<CFontBank> PCutSceneMainFont;      // +0x1614C
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }      // Ego_r ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ
    const CFontBank& PeekCutSceneMainFont() const { return *PCutSceneMainFont; } // Ego_r ?PeekCutSceneMainFont@CMainGameComponent@@QBEABVCFontBank@@XZ
};
class CSubtitleRenderer { public:
    void SetText(const CFontBank& font, const CWideString& text, unsigned long duration, float delay, bool a, bool b);  // 0x00492E4B
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                     // +0x18
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790

class CTCHeroStats { public:
    void AddMoney(long amount, bool showMessage);           // 0x0057B327
};
class CTCHeroReceiveItems { public:
    void GiveItems(const std::vector<std::pair<long, CCharString> >& items, CThing& giver, bool showMessage);   // 0x007CA9E0
};

class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
protected:
    CPlayerManager& GetPlayerManager();                     // 0x00686D40
    const CMainGameComponent& PeekComponent() const;        // 0x00686D60
};
class CTCContainerRewardHero : public CTCBase { public:
    bool RewardHeroWithItems();
protected:
    void GetRewardItems(std::vector<std::pair<long, CCharString> >& items, long& gold);   // 0x0074E860
};

#pragma auto_inline(off)
void CTCContainerRewardHero::GetRewardItems(std::vector<std::pair<long, CCharString> >& items, long& gold)
{
    gold = 0;
    items.erase(items.begin(), items.end());
}
#pragma auto_inline(on)

bool CTCContainerRewardHero::RewardHeroWithItems()
{
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead() && hero->HasTC(0xCB)) {
        CTCHeroReceiveItems* receiver = hero->GetTC<CTCHeroReceiveItems>(0xCB);
        long gold = 0;
        std::vector<std::pair<long, CCharString> > items;
        GetRewardItems(items, gold);
        if (gold <= 0 && items.empty()) {
            CDefPointer<const CChestDef> chestDef;
            bool found = ParentThing->GetPBaseDef()->GetSubDef(&chestDef);
            if (!found || chestDef->DisplayMessageOnEmpty) {
                CWideString text = g_pMainGameComponent->GetTextBank().GetTextBySymbol("TXT_SIGN_NOTHING");
                g_pPlayerGui->SubtitleRenderer.SetText(PeekComponent().PeekCutSceneMainFont(), text, 0x122, 0.0f, false, true);
            }
            return false;
        }
        if (gold > 0) {
            CTCHeroStats* stats = hero->PeekTCOrUnset<CTCHeroStats>(4);
            if (stats)
                stats->AddMoney(gold, true);
        }
        if (!items.empty()) {
            receiver->GiveItems(items, *ParentThing, true);
            return true;
        }
    }
    return false;
}