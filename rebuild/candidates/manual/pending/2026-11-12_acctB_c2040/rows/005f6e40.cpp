// CTCInventoryAbilities::OnSerialise(CPersistContext& context, bool inGameSave) -- persist the hero's abilities by def name: levels, button assignments and the received order; rebuild the assigned palettes, then serialise the base inventory
// Leaf: ?OnSerialise@CTCInventoryAbilities@@UAEXAAVCPersistContext@@_N@Z
// Inline-budget: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ + FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND (existing shared-header opt-ins) --
//   retail expands CCharString::operator==(const char*) (0x005F7310..0x005F7337 + 0x005F7437: null test, inline strcmp `repe cmpsb`,
//   else CBasicString<char>::operator==(const char*) 0x004115A0) and CVectorMap<long,EHeroAbility>::Find (0x005F6F19..0x005F6F2E);
//   c2 inline trace (bin/inl.sh, probe/noeq.cpp, probe/nofind.cpp): this optimize("s") TU expands plain inlines only up to sz 15
//   (largest expanded 15, smallest refused 22), operator== sz 39 (f 18.75) and Find sz 37 (f 12.5) are refused without a score, so
//   no control-flow spelling or /d2inlT reaches them; the opt-ins are the CTCInventoryWeapons-TU / 0x00E36D50 devices.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6E40, 1746 bytes, thiscall `ret 8`; frame-pointer size TU (`optimize("y", off)` +
//   `("s", on)`: `push ebp ; mov ebp,esp ; sub esp,3Ch ... leave`). Starts right after 0x005F6E3D `ret 0Ch`, ends at 0x005F750F `ret 8`;
//   the bytes from 0x005F7512 are separate tiny accessor COMDATs (extent confirmed, no SPLITS entry).
// Identity: Ego_r.pdb + FableWin.pdb ?OnSerialise@CTCInventoryAbilities@@UAEXAAVCPersistContext@@_N@Z (public virtual); its only
//   reference is vtable 0x01256AD4 slot 1 (0x01256AD8), the OnSerialise slot of the CTCInventory* family (CTCInventoryClothing
//   0x005B369A), and it ends with the base call NInventory::CTCInventoryBase::OnSerialise 0x005BFD5E (`ret 8`, same arguments).
// Callees (all bound in reloc_bindings.tsv at the same addresses; see NAME_FIXES.tsv for the stand-in spellings replaced):
//   CPersistContext::IsSaving 0x004AEF50 (out of line in this TU), TransferVector<CCharString> 0x0049B8D0 / <long> 0x005815D6,
//   CDefClassBase vtable slot 10 GetInstantiationName (+0x28), CDefString::GetString 0x00415DD0, vector<CCharString>::push_back
//   0x0044BFF0 / erase(first,last) 0x0043336A (clear) / ~vector 0x00414EA0, vector<long>::push_back 0x00485FF3,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, GetNumAssignableSpecialMoveButtons 0x005EDCF0,
//   CVectorMap<long,EHeroAbility>::LowerBound 0x005F7F4C, CTCBase::PeekWorld 0x00686A80 / GetDefinitionManager 0x006869D0 /
//   PeekPlayerManager 0x00686D50, CPlayerManager::GetMainPlayer const 0x00449960, CGamePlayerInterface::
//   GetNumberOfAssignableSpecialMoveButtons 0x00445940, CDefinitionManager::GetPDefFromInstantiationName 0x009ADA40,
//   InitialiseAbilities 0x005EEDE2, SetAbilityAsAvailable 0x005F6BD6, SetAbilityToButton 0x005F6423, GetMiscThingsDef 0x005EE3B3,
//   vector<EHeroAbility>::erase(first,last) 0x005F8F89 / resize(n, const T&) 0x005F9DF8 / push_back 0x005F9BCD, free 0x00BFEA14.
// Literals: "HeroAbility" 0x01256F94, "AbilityLevels" 0x01256F84, "AbilityButton" 0x01256F74, "ReceivedAbilities" 0x01256F60,
//   "" 0x0122D70E. World flag: CWorld::IsSerialisingAboutToLoadHeroState() (FableWin.pdb inline accessor, [world+0x101]).
// Shape (how each retail detail was reached; see NOTES.md for the grade log):
//   - every def name pushed into a name vector is a NAMED CCharString in its own block (`lea eax,<name>` after GetString, the
//     GetString result pointer pushed ahead of the virtual GetInstantiationName call, the destructor before the levels push);
//     the empty names are implicit `""` conversions (re-addressed with `lea`);
//   - the received-order save loop reads the ability into a named EHeroAbility first (`cmp [eax+4],esi ; je ; mov eax,[eax+4]`);
//   - the received-order load loop names the element (`const CCharString& name`, homed at [ebp-14h] for the lookup push);
//   - the palette count is an if / else of two assignments (retail stores [ebp-10h] in each arm and parks the received-order
//     compare blocks before the second arm);
//   - the load-path palette size is read into a named local before `resize` (c2 scheduler: `lea eax,[ebp-14h]` ahead of the
//     zero store, 0x005F72A8);
//   - the load path writes GetNumAssignableSpecialMoveButtons' chain in place (retail has the chain there and calls 0x005EDCF0 in
//     the save loop; with the member's body visible c2 refuses it at every site of this size TU: sz 20); GetMiscThingsDef has its retail body in the TU (above OnSerialise, as in retail's TU; it grades
//     RELOCATION_MATCH against 0x005EE3B3), so the misc def is known not to escape: in-place `dec [ecx+4]` release, no null test.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CPERSISTCONTEXT_ISSAVING_OUTOFLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ABOUTTOLOADHEROSTATE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_SPECIALMOVEBUTTONS
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ONSERIALISE
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_INITIALISE
#define FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#include "engine/CWorld.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CPersistContext.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"

#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
// Same-TU body of the landed GetMiscThingsDef (0x005EE3B3, compiled above OnSerialise in retail's TU): with it VC7.1 knows the
// hidden result is not captured, so the callers' misc def releases in place (`dec dword ptr [ecx+4] ; jne`, 0x005F72BF / 0x005F74F4).
#if defined(_MSC_VER)
__declspec(noinline)
#endif
const CDefPointer<const NUISystem::CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    long index;
    {
        CCharString name("UI_MISC_THINGS_DEF");
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    {
        CDefPointer<const NUISystem::CUIMiscThingsDef> def;
        CGameDefinitionManager::Get().GetDef(index, &def);
        return def;
    }
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

void CTCInventoryAbilities::OnSerialise(CPersistContext& context, bool inGameSave)
{
    if (context.IsSaving()) {
        std::vector<CCharString> ability_names;
        std::vector<long> ability_levels;
        for (long ability = 0; ability < MAX_NUMBER_OF_HERO_ABILITIES; ++ability) {
            if (Abilities[ability].first >= 0 && !Abilities[ability].second.IsNull()) {
                {
                    CCharString name = Abilities[ability].second->GetInstantiationName().GetString();
                    ability_names.push_back(name);
                }
                ability_levels.push_back(Abilities[ability].first);
            }
        }
        context.TransferVector("HeroAbility", ability_names);
        context.TransferVector("AbilityLevels", ability_levels);
        ability_names.clear();
        for (long button = 0; button < GetNumAssignableSpecialMoveButtons(); ++button) {
            CVectorMap<long, EHeroAbility>::iterator it = AbilityButtons.Find(button);
            if (it != AbilityButtons.end()) {
                if (!Abilities[it->second].second.IsNull()) {
                    CCharString name = Abilities[it->second].second->GetInstantiationName().GetString();
                    ability_names.push_back(name);
                } else
                    ability_names.push_back("");
            } else
                ability_names.push_back("");
        }
        context.TransferVector("AbilityButton", ability_names);
        std::vector<CCharString> received_names;
        for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
            EHeroAbility ability = ReceivedAbilityOrder[i];
            if (Abilities[ability].first >= 0) {
                if (!Abilities[ability].second.IsNull()) {
                    CCharString name = Abilities[ability].second->GetInstantiationName().GetString();
                    received_names.push_back(name);
                }
            } else
                received_names.push_back("");
        }
        context.TransferVector("ReceivedAbilities", received_names);
    } else {
        std::vector<CCharString> ability_names;
        if (!PeekWorld().IsSerialisingAboutToLoadHeroState()) {
            InitialiseAbilities();
            Initialised = true;
        }
        context.TransferVector("HeroAbility", ability_names);
        std::vector<long> ability_levels;
        context.TransferVector("AbilityLevels", ability_levels);
        if (!PeekWorld().IsSerialisingAboutToLoadHeroState()) {
            bool initialised = Initialised;
            Initialised = false;
            for (unsigned long i = 0; i < ability_names.size(); ++i) {
                CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromInstantiationName(ability_names[i]);
                if (!def.IsNull()) {
                    CDefPointer<const CHeroAbilityDef> ability_def(def);
                    SetAbilityAsAvailable(ability_def->Ability, true, false);
                    if (i < ability_levels.size())
                        Abilities[ability_def->Ability].first = ability_levels[i];
                }
            }
            Initialised = initialised;
        }
        ability_names.clear();
        context.TransferVector("AbilityButton", ability_names);
        long num_buttons = PeekPlayerManager().GetMainPlayer().PlayerInterface->GetNumberOfAssignableSpecialMoveButtons();
        for (long button = 0; button < num_buttons; ++button) {
            CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromInstantiationName(ability_names[button]);
            if (!def.IsNull()) {
                CDefPointer<const CHeroAbilityDef> ability_def(def);
                SetAbilityToButton(ability_def->Ability, button, true);
            }
        }
        if (!PeekWorld().IsSerialisingAboutToLoadHeroState()) {
            CDefPointer<const NUISystem::CUIMiscThingsDef> misc = GetMiscThingsDef();
            ReceivedAbilityOrder.clear();
            unsigned long total = misc->TotalSpellsInPalettes; ReceivedAbilityOrder.resize(total, HERO_ABILITY_NULL);
        }
        std::vector<CCharString> received_names;
        context.TransferVector("ReceivedAbilities", received_names);
        if (!PeekWorld().IsSerialisingAboutToLoadHeroState()) {
            for (unsigned long i = 0; i < received_names.size(); ++i) {
                const CCharString& name = received_names[i];
                if (name == "")
                    ReceivedAbilityOrder[i] = HERO_ABILITY_NULL;
                else {
                    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromInstantiationName(name);
                    if (!def.IsNull()) {
                        CDefPointer<const CHeroAbilityDef> ability_def(def);
                        ReceivedAbilityOrder[i] = ability_def->Ability;
                    }
                }
            }
        }
    }
    if (!PeekWorld().IsSerialisingAboutToLoadHeroState()) {
        CDefPointer<const NUISystem::CUIMiscThingsDef> misc = GetMiscThingsDef();
        AssignedAbilities.clear();
        AssignedAbilitiesSave.clear();
        unsigned long count;
        if (ReceivedAbilityOrder.size() <= misc->TotalSpellsInPalettes)
            count = ReceivedAbilityOrder.size();
        else
            count = misc->TotalSpellsInPalettes;
        for (unsigned long i = 0; i < count; ++i) {
            AssignedAbilities.push_back(ReceivedAbilityOrder[i]);
            AssignedAbilitiesSave.push_back(ReceivedAbilityOrder[i]);
        }
        unsigned long assigned = AssignedAbilities.size();
        if (assigned < misc->TotalSpellsInPalettes) {
            for (unsigned long j = 0; j < misc->TotalSpellsInPalettes - assigned; ++j) {
                AssignedAbilities.push_back(HERO_ABILITY_NULL);
                AssignedAbilitiesSave.push_back(HERO_ABILITY_NULL);
            }
        }
    }
    NInventory::CTCInventoryBase::OnSerialise(context, inGameSave);
}
