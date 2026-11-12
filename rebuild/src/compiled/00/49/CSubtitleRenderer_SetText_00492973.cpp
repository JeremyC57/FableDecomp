// CSubtitleRenderer::SetText(const CFontBank& font, const CWideString& text, const CWideString& name, const CWideString& button0_text, const CWideString& button1_text, const CWideString& button2_text, unsigned long flags, float time, bool update_while_paused, bool change_player_mode) -- while text is showing, queue the request (CSetTextInfo); otherwise take the font, reset the text group, resolve the text and captions through the text tags, clear the button overrides, reset timers and state, preload the text in the font, pause the game for blocking text (marking the living hero's item display), switch the player mode, and drop pending "conversation click past" messages
// Leaf: ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@1111KM_N2@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00492973, 568 bytes, thiscall `ret 28h`. Ego_r.pdb / FableWin.pdb name above (Ego_r 0x0049BBF8).
// Callees: CSetTextInfo ctor 0x0048DDF6 / dtor 0x0043E3D0; deque<CSetTextInfo>::push_back 0x004941C0 (inline queue::push);
//   CWideString::operator=(const wchar_t*) 0x0099B800; GetTextFromTextTag 0x0048F6BD; ProcessEmbeddedGraphicTags 0x00492381;
//   ProcessEmbeddedHighlightTags 0x00491AD2; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   SetButton1OverrideText 0x0048F7C0 / SetButton2OverrideText 0x0048F881; CFontBank::Preload (vtable slot 4);
//   CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0; IS_THING_ALIVE<CThingPlayerCreature*>
//   0x0043DBD0; bitset<274>::test 0x00410DE0; CVectorMap::LowerBound 0x0040F020 (forced-inline CThing::GetTC); Pause 0x0048DD94;
//   InitPlayerMode 0x0048F939; the found list's constructor 0x00493BC2 / destructor 0x00493BDD (Ego_r.pdb names the same TU's
//   COMDATs ??0/??1?$list@PBVCMessageEvent@@V?$allocator@PBVCMessageEvent@@@std@@@std@@ at 0x0049C87F / 0x0049CB20, a Dinkumware
//   build; retail is STLport, and the spelling here, the STLport base _List_base<const CMessageEvent*> ctor/dtor, is UNPROVEN by any
//   PDB: it rests on the STLport code shape (ctor `and [esi],0 ; push 0Ch ; call malloc ; self-link ; ret 4` with one allocator&
//   argument, dtor `call clear 0x00493911 ; free sentinel`) plus the retail call graph (the out-of-line list(const allocator&) at
//   0x00493CCA forwards its argument to 0x00493BC2 and the list destructor at 0x00493CDC is `jmp 0x00493BDD`, i.e. both are base-class
//   members). Landed rows: global_ListNodeCreateCircularSentinel_00493bc2.cpp / std_list_T_00493bdd_dtor_00493bdd.cpp;
//   NAME_FIXES.tsv carries the proposed respelling);
//   CMessageEventManager::FindAllMessages<CMessageEventFilter_IsOfType> 0x00493EAB; CMessageEventManager::RemoveMessage 0x00493B5B.
// Shape: no /GS. Argument 3..6 map to NameText +0x60, Button2Text +0x68, Button1Text +0x64, Button3Text +0x6C (retail order). The
//   found list, its allocator temp and the filter live in dead argument slots. What made it match: the COMDATs this TU emits after
//   the function (list base ctor/dtor = STLport header bodies, RemoveMessage = landed row body, FindAllMessages = landed row loop on
//   shared headers, byte-proven against 0x00493EAB) have their bodies visible but out of line, so VC7.1 knows
//   the found list's sentinel survives RemoveMessage: it stays in edi and the removal loop is the unrotated jmp-to-test form.
// Pragmas (the established spelling of this TU: every landed sibling, e.g. CSubtitleRenderer_SetText_00492e4b / _00492f69 /
//   _00493049, Clear_00492bab, RemoveMessage_00493b5b, FindAllMessages_00493eab, uses optimize("y", off) + ("s", on), no addmatch flags):
//   optimize("y", off) -- retail keeps the frame pointer: `push ebp ; mov ebp,esp` at 0x00492973, arguments read as [ebp+0Ch..2Ch],
//     `leave` at 0x00492BA7 (default /Oy would address them off esp and drop the frame).
//   optimize("s", on) -- size-optimised code: `or dword ptr [esi+2Ch],-1` at 0x004929C5 (TextGroupIndex = -1), `fldz ; fstp
//     [esi+8Ch]` at 0x00492A77 (TextTimer = 0.0f), `and dword ptr [esi+24h],0` at 0x00492A85 (TextCharactersShowing = 0); /O2 code
//     stores immediates with `mov` instead.
//   auto_inline(off) around the carried bodies -- retail calls all four out of line from this function (call 0x00493BC2 at
//     0x00492B66, call 0x00493EAB at 0x00492B7C, call 0x00493B5B at 0x00492B91, call 0x00493BDD at 0x00492B9F) although their bodies
//     were in the TU (auto_inline(off) keeps the template bodies out of line; RemoveMessage, marked inline, also needs
//     __declspec(noinline)); switched back on before SetText
//     so SetText itself compiles under the default inlining.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDALL
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS
#define FABLE_RETAIL_CTCHERORECEIVEITEMS_DISPLAYINGITEMS
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXTINFO
#define FABLE_RETAIL_CSUBTITLERENDERER_PAUSE
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONOVERRIDE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_LIST_CLEAR_NOINLINE
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE
#define FABLE_RETAIL_STLP_QUEUE
#define FABLE_RETAIL_STLP_DEQUE_PUSH_BACK_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_deque.h"
#include "engine/ThingAlive.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CTCHeroReceiveItems.h"
#include "engine/CFontBank.h"
#include "engine/CSubtitleRenderer.h"

// Callee bodies retail's TU compiled before SetText and emitted as trailing COMDATs (Ego_r puts the same COMDATs right after
// SetText's TU: list ctor 0x0049C87F, dtor 0x0049CB20, FindAllMessages 0x0049CB32, RemoveMessage 0x0049CCAB), visible here but
// kept out of line. Provenance of each body:
// - _List_base<_Tp,_Alloc>(const _Alloc&) / ~_List_base() (0x00493BC2 / 0x00493BDD): the STLport header's own #else bodies in
//   stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE declares them, these are the definitions it omits), NOT the
//   landed rows global_ListNodeCreateCircularSentinel_00493bc2.cpp / std_list_T_00493bdd_dtor_00493bdd.cpp, which are different
//   made-up models (a __fastcall malloc helper and a placeholder list_T_00493bdd). Both compile to the retail bytes of those addresses
//   (ctor: and [this],0 / malloc(0Ch) / self-link / ret 4; dtor: clear 0x00493911 then free of the sentinel).
// - CMessageEventManager::RemoveMessage (0x00493B5B): not a template. A header-inline member in the original (Ego_r places it among
//   this TU's trailing COMDATs, 0x0049CCAB, next to SetText's TU), which VC7.1 compiled out of line; spelled `inline` +
//   __declspec(noinline) so it stays a COMDAT called out of line, as retail does. Body: the landed row
//   CMessageEventManager_RemoveMessage_00493b5b.cpp verbatim, except that the shared STLport list has no `_Node` typedef, so the
//   node type is named std::_List_node<CMessageEvent> directly (the same type).
// - CMessageEventManager::FindAllMessages<CMessageEventFilter_IsOfType> (0x00493EAB): a member template (Ego_r ??$FindAllMessages@...
//   0x0049CB32); the landed row CMessageEventManager_FindAllMessages_00493eab.cpp's loop (filter each message, push_back its address,
//   return the found list's size) spelled on the shared headers. Its size() is STLport 4.6's own list::size(),
//   `distance(begin(), end())` through the free __distance input-iterator loop (shared opt-in FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE),
//   which is what the landed row's free Distance(first, last) models: no `this` register, so the found list stays in eax
//   (`mov eax,[ebp+0Ch] ; mov edx,[eax]` at 0x00493EE4). The same text graded against 0x00493EAB (leaf FindAllMessages, 81 bytes) is
//   RELOCATION_MATCH, so this is the byte-proven body of that address, not a stand-in. (The default list::size() for-loop spelling
//   gave DIFFER(81v81) there: `mov ecx,[ebp+0Ch] ; mov edx,[ecx]`.)
#pragma auto_inline(off)
namespace std {
template<class _Tp,class _Alloc> _List_base<_Tp,_Alloc>::_List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
    _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
}
template<class _Tp,class _Alloc> _List_base<_Tp,_Alloc>::~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
}
inline __declspec(noinline) void CMessageEventManager::RemoveMessage(const CMessageEvent* event)
{
    typedef std::_List_node<CMessageEvent> _Node;
    std::_List_node_base* head = Messages._M_node._M_data;
    for (std::_List_node_base* node = head->_M_next; node != head; node = node->_M_next) {
        if (&((_Node*)node)->_M_data == event) {
            Messages.erase(std::list<CMessageEvent>::iterator((_Node*)node));
            return;
        }
    }
}
template<class TFilter> const long CMessageEventManager::FindAllMessages(const TFilter& filter, std::list<const CMessageEvent*>& found) const
{
    for (std::list<CMessageEvent>::const_iterator it = Messages.begin(); it != Messages.end(); ++it)
        if (filter(*it))
            found.push_back(&*it);
    return found.size();
}
#pragma auto_inline(on)

typedef std::queue<CSubtitleRenderer::CSetTextInfo, std::deque<CSubtitleRenderer::CSetTextInfo> > CTextInfoQueue;

void CSubtitleRenderer::SetText(const CFontBank& font, const CWideString& text, const CWideString& name,
                                const CWideString& button0_text, const CWideString& button1_text, const CWideString& button2_text,
                                unsigned long flags, float time, bool update_while_paused, bool change_player_mode)
{
    if (DisplayingText) {
        CSetTextInfo info(text, button0_text, button1_text, button2_text, flags, time, update_while_paused, change_player_mode);
        reinterpret_cast<CTextInfoQueue&>(TextInfoQueue).push(info);
        return;
    }
    PFont = const_cast<CFontBank*>(&font);
    TextGroupIndex = -1;
    TextGroupHasEntriesRemaining = false;
    reinterpret_cast<CWideString&>(TextGroupTag_PStringData) = L"";
    GetTextFromTextTag(text, reinterpret_cast<CWideString&>(Text_PStringData));
    ProcessEmbeddedGraphicTags();
    ProcessEmbeddedHighlightTags();
    GetTextFromTextTag(name, reinterpret_cast<CWideString&>(NameText_PStringData));
    GetTextFromTextTag(button0_text, reinterpret_cast<CWideString&>(Button2Text_PStringData));
    GetTextFromTextTag(button1_text, reinterpret_cast<CWideString&>(Button1Text_PStringData));
    GetTextFromTextTag(button2_text, reinterpret_cast<CWideString&>(Button3Text_PStringData));
    SetButton1OverrideText("");
    SetButton2OverrideText("");
    TextTimer = 0.0f;
    MaxDisplayTime = time;
    TextCharactersShowing = 0;
    Button1Alpha = 0xff;
    Button2Alpha = 0xff;
    DisplayingText = true;
    DisplayFlags = flags;
    UpdateTimer = true;
    AllCharactersDisplayed = true;
    if (flags & 0x10)
        AllCharactersDisplayed = false;
    PFont->Preload(reinterpret_cast<CWideString&>(Text_PStringData), true);
    if (!(DisplayFlags & 0x4c) && !(DisplayFlags & 0x100)) {
        UpdateWhilePaused = update_while_paused;
    } else {
        if (DisplayFlags & 0x40) {
            CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (IS_THING_ALIVE(hero) && hero->HasTC(TCI_HERO_RECEIVE_ITEMS))
                static_cast<CTCHeroReceiveItems&>(hero->GetTC(TCI_HERO_RECEIVE_ITEMS)).DisplayingItems = true;
        }
        Pause(true);
    }
    if (change_player_mode)
        InitPlayerMode();
    CMessageEventManager& manager = CMainGameComponent::Get()->GetWorld().GetMessageEventManager();
    std::list<const CMessageEvent*> found;
    if (manager.FindAllMessages(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_CLICK_PAST), found) > 0) {
        for (std::list<const CMessageEvent*>::iterator it = found.begin(); it != found.end(); ++it)
            manager.RemoveMessage(*it);
    }
}