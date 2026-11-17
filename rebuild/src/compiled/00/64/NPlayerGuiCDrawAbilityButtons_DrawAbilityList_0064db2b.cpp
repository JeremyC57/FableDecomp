// NPlayerGui::CDrawAbilityButtons::DrawAbilityList(CEnginePrimitivePrepareInfo*, float) -- draw the hero's ability grid: for a live hero with the abilities component (0x6f), scale by 0.75, fetch the ability def list, and starting at the row after the scroll row draw every row that holds at least one ability (each column at its slot offset), then wrap round to the rows before the scroll row, numbering the drawn rows consecutively from 0
// Leaf: ?DrawAbilityList@CDrawAbilityButtons@NPlayerGui@@MAEXABVCEnginePrimitivePrepareInfo@@M@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064DB2B, length 546 bytes (frame pointer /Os TU, `ret 8`).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0, IS_THING_ALIVE 0x0043DBF0
//   (__fastcall, by reference; same-TU stand-in body), virtual slot 5 (slot-offset fill), std::bitset::test 0x00410DE0, LowerBound
//   0x004365B0 (const; same-TU stand-in body), CTCHeroAbilities ability-list fill 0x005F4B30, the ability vector dtor 0x005F9F14,
//   DrawAbilityButton 0x0064AE80, CRT free 0x00BFEA14 (offset vector storage).
// Layout: as the landed CDrawMeleeActionButtonEffects::Update 0x0064BE45 (same TU): PlayerManager +0x08, Columns +0x28 (PDB +0x2c,
//   retail 4 smaller); hero thing presence bits +0x20, interface map +0x44; abilities component scroll index +0x144. Modelled locally.
// Shape: /Os via optimize("y",off)+("s",on); the GetInterface out-param idiom with the null component tested in the same condition;
//   the drawn-row counter is initialised as `firstRow - 1` (initialising it from the scrollRow local swaps two registers).
// NF51 (2026-11-17): the local bitset / CVectorMap / CThing / IS_THING_ALIVE / CPlayer / LongVector / AbilityVector /
//   CTCHeroAbilities / CDrawAbilityButtons models and the same-TU LowerBound stand-in body replaced by the shared headers of the
//   landed same-TU DrawAbilityButton 0x0064AE80 (its define set): engine/NPlayerGui_CDrawAbilityButtons.h (PASS23; slot 5 is
//   GetAbilityButtonMapping), CThing::PeekTC (GETTC_EXPANDED; LowerBound const 0x004365B0), ThingAlive.h (IS_THING_ALIVE noinline),
//   CTCInventoryAbilities (interface 0x6F TCI_HERO_ABILITIES; GetPrioritisedAbilityDefs 0x005F4B30, GetHUDAssignedAbilityLevel, the FableWin.pdb inline reader of CurrentAssignedAbilityLevel +0x144: overlay opt-in _HUDLEVEL).
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_CRIME_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_ABILITY_CONTEXT_PASS23
#define FABLE_RETAIL_CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CURSOR_ORIGINAL_DECLARATIONS_PASS22
#define FABLE_RETAIL_NPLAYERGUI_CDRAWABILITYBUTTONS_PASS23
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LIFETIMES_PASS23
#define FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED
#define FABLE_RETAIL_CTCINVENTORYABILITIES_HUDLEVEL
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_USE
#define FABLE_RETAIL_CTCSPECIALABILITIES_GUI_DRAW_PASS23
#include "engine/EEnginePrimitiveType.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/NPlayerGui_CDrawAbilityButtons.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/ThingAlive.h"
#include "engine/NPlayerGui_Graphics.h"

#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"

namespace NPlayerGui {
void CDrawAbilityButtons::DrawAbilityList(const CEnginePrimitivePrepareInfo& info, float scale)
{
    const CThingPlayerCreature* hero = PlayerManager.GetMainPlayer().PeekPControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    std::vector<long> offsets;
    GetAbilityButtonMapping(offsets);
    const CTCInventoryAbilities* tc;
    if (hero->PeekTC(TCI_HERO_ABILITIES, &tc) && tc) {
        scale *= 0.75f;
        std::vector<CDefPointer<const CHeroAbilityDef> > abilities;
        tc->GetPrioritisedAbilityDefs(abilities);
        long firstRow = tc->GetHUDAssignedAbilityLevel() / Columns + 1;
        long rows = ((long)abilities.size() + 2) / Columns * Columns / Columns;
        long scrollRow = firstRow - 1;
        long drawn = firstRow - 1;
        long row;
        for (row = firstRow; row < rows; ++row) {
            bool any = false;
            long col;
            for (col = 0; col < Columns; ++col) {
                unsigned int index = offsets[col] + row * Columns - 1;
                if (index < abilities.size() && !abilities[index].IsNull()) {
                    any = true;
                    break;
                }
            }
            if (any) {
                ++drawn;
                for (col = 0; col < Columns; ++col)
                    DrawAbilityButton(info, abilities, col, offsets[col] + row * Columns - 1, drawn - firstRow, scale);
            }
        }
        for (row = 0; row < scrollRow; ++row) {
            bool any = false;
            long col;
            for (col = 0; col < Columns; ++col) {
                unsigned int index = offsets[col] + row * Columns - 1;
                if (index < abilities.size() && !abilities[index].IsNull()) {
                    any = true;
                    break;
                }
            }
            if (any) {
                ++drawn;
                for (col = 0; col < Columns; ++col)
                    DrawAbilityButton(info, abilities, col, offsets[col] + row * Columns - 1, drawn - firstRow, scale);
            }
        }
    }
}
}
