// CSubtitleRenderer::GetGraphicFromEmbeddedTag(const CWideString& tag) -- the graphic for an embedded button tag: the tag is upper-cased, the controller button names (A, B, X, Y, DPAD / D-PAD and its four directions, the sticks and stick clicks under both spellings, BACK, START, the triggers, BLACK, WHITE) give the player GUI def's controller graphics, anything else is looked up in the def's script sprites (0 when unknown)
// Leaf: ?GetGraphicFromEmbeddedTag@CSubtitleRenderer@@ABEJABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00490BE0, 1646 bytes, thiscall `ret 4` (this unused); the frame-pointer size-optimised
//   CSubtitleRenderer TU (`push ebp ; mov ebp,esp ; push ecx ; push ecx`, `leave`), as the landed neighbours 0x0048F40A / 0x0048F939.
// Identity (proven): (1) Ego_r.pdb + FableWin.pdb ?GetGraphicFromEmbeddedTag@CSubtitleRenderer@@ABEJABVCWideString@@@Z (private
//   const, returns long, one const CWideString&) -- the only CSubtitleRenderer member of that shape; (2) its one caller is
//   CSubtitleRenderer::ProcessEmbeddedGraphicTags (0x0049241E: `mov ecx,edi` = this, the tag pushed, the long result tested > 0);
//   (3) the body maps button tag names to CPlayerGuiDef::Controller*Graphic. Catalog label "ProcessEmbeddedGraphicTags_ResolveButtonName"
//   is behavioural.
// Callees: CWideString::ToCharString 0x0099B2C0, CCharString::GetAsUpperCase 0x0099EE20 (Ego_r / FableWin ?GetAsUpperCase@CCharString@@QBE?AV1@XZ;
//   the body calls toupper 0x00BFED16 -- landed callers spell it "ToUpper": NAME_FIXES), CCharString::ToWideString 0x0099E480,
//   ~CCharString 0x0099EAE0, ~CWideString 0x0099B510, CPlayerGui::PeekGuiDef 0x00438570 (static __fastcall), wcscmp 0x00BFE94C (import),
//   the STLport operator==(const wstring&, const unsigned short*) 0x00404280, _Rb_tree<CCharString, pair<const CCharString, long>, ...>
//   ::_M_find<CCharString> 0x0042B467. Data: L"" 0x0122D70C, the 28 tag literals 0x01238230 .. 0x01238480 (pairing checked by relocation).
// Layout: engine/CPlayerGuiDef.h (generated, retail STLport layout; opt-in FABLE_RETAIL_CPLAYERGUIDEF_SCRIPTSPRITES types ScriptSprites):
//   ControllerAGraphic .. ControllerTriggerWhiteGraphic +0x648 .. +0x690 (Ego_r +0x684 .. +0x6CC), ScriptSprites +0x934 (Ego_r +0x9B0).
// Shape: the upper-cased tag is a named CWideString that VC7.1 homes in the dead parameter slot [ebp+8]; each tag test is its own
//   `if (tag == L"X") return ...;` (the two-spelling buttons as two separate ifs -- an `||` inverts the null-case branch of the first
//   compare); the inline CWideString::operator==(const wchar_t*) of fable_string.h (FABLE_RETAIL_CWIDESTRING_TEXT_EQ + FORCEINLINE_EQ,
//   chunk 784); the script-sprite fallback written found-first (`if (it != end) return it->second; return 0;`) puts retail's zero
//   block last, before the shared ~CWideString exit.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING_GETASUPPERCASE
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_CPLAYERGUIDEF_SCRIPTSPRITES
#define FABLE_RETAIL_CSUBTITLERENDERER_GRAPHICTAG
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
typedef unsigned short wchar_t;                                   // VC7.1 without /Zc:wchar_t
extern "C" int __cdecl wcscmp(const wchar_t* a, const wchar_t* b);   // 0x00BFE94C
#include "fable_string.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CPlayerGui.h"

long CSubtitleRenderer::GetGraphicFromEmbeddedTag(const CWideString& tag) const
{
    CWideString upper_tag = tag.ToCharString().GetAsUpperCase().ToWideString();
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    if (upper_tag == L"A")
        return def.ControllerAGraphic;
    if (upper_tag == L"B")
        return def.ControllerBGraphic;
    if (upper_tag == L"X")
        return def.ControllerXGraphic;
    if (upper_tag == L"Y")
        return def.ControllerYGraphic;
    if (upper_tag == L"DPAD")
        return def.ControllerDPadGraphic;
    if (upper_tag == L"D-PAD")
        return def.ControllerDPadGraphic;
    if (upper_tag == L"DPAD DOWN")
        return def.ControllerDPadDownGraphic;
    if (upper_tag == L"D-PAD DOWN")
        return def.ControllerDPadDownGraphic;
    if (upper_tag == L"DPAD LEFT")
        return def.ControllerDPadLeftGraphic;
    if (upper_tag == L"D-PAD LEFT")
        return def.ControllerDPadLeftGraphic;
    if (upper_tag == L"DPAD RIGHT")
        return def.ControllerDPadRightGraphic;
    if (upper_tag == L"D-PAD RIGHT")
        return def.ControllerDPadRightGraphic;
    if (upper_tag == L"DPAD UP")
        return def.ControllerDPadUpGraphic;
    if (upper_tag == L"D-PAD UP")
        return def.ControllerDPadUpGraphic;
    if (upper_tag == L"LEFT STICK")
        return def.ControllerThumbLeftGraphic;
    if (upper_tag == L"LEFT THUMBSTICK")
        return def.ControllerThumbLeftGraphic;
    if (upper_tag == L"RIGHT STICK")
        return def.ControllerThumbRightGraphic;
    if (upper_tag == L"RIGHT THUMBSTICK")
        return def.ControllerThumbRightGraphic;
    if (upper_tag == L"LEFT STICK CLICK")
        return def.ControllerThumbClickLeftGraphic;
    if (upper_tag == L"LEFT THUMBSTICK CLICK")
        return def.ControllerThumbClickLeftGraphic;
    if (upper_tag == L"RIGHT STICK CLICK")
        return def.ControllerThumbClickRightGraphic;
    if (upper_tag == L"RIGHT THUMBSTICK CLICK")
        return def.ControllerThumbClickRightGraphic;
    if (upper_tag == L"BACK")
        return def.ControllerBackGraphic;
    if (upper_tag == L"START")
        return def.ControllerStartGraphic;
    if (upper_tag == L"LEFT TRIGGER")
        return def.ControllerTriggerLeftGraphic;
    if (upper_tag == L"RIGHT TRIGGER")
        return def.ControllerTriggerRightGraphic;
    if (upper_tag == L"BLACK")
        return def.ControllerTriggerBlackGraphic;
    if (upper_tag == L"WHITE")
        return def.ControllerTriggerWhiteGraphic;
    std::map<CCharString, long>::const_iterator it = def.ScriptSprites.find(upper_tag.ToCharString());
    if (it != def.ScriptSprites.end())
        return it->second;
    return 0;
}
