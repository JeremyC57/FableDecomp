// std::list<NSpeechGainManager::CActiveDialogueInfo>::_M_create_node(const CActiveDialogueInfo&) -- allocates a 0x20-byte list node and copy-constructs the element into it (links left unset)
// Leaf: ?_M_create_node@?$list@VCActiveDialogueInfo@NSpeechGainManager@@V?$allocator@VCActiveDialogueInfo@NSpeechGainManager@@@std@@@std@@IAEPAU?$_List_node@VCActiveDialogueInfo@NSpeechGainManager@@@2@ABVCActiveDialogueInfo@NSpeechGainManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E4CB0, length 96 bytes.
// Callees: malloc @ 0x00BFEA0E (0x20 bytes: two links + the 0x18-byte element), CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Label: the manifest's "CCreatureAction_JackFloatUp::Clone" is propagated nonsense -- `this` (ecx) is never read, the one
// stack argument is the element, and the result is the raw node (its +8 is null-checked: the placement new of a class with
// a constructor). The element is CActiveDialogueInfo (Ego_r.pdb: CIntelligentPointer<CThing> ThingSpeaking +0, ulong
// SoundID +8, EDialogueLayer Layer +0xc, bool IsLayerHead +0x10, float OutputGain +0x14 = 0x18 bytes): the vtable stored
// into the node is CIntelligentPointer's (0x01238C6C) and the four trailing members are copied one register each (the
// implicit memberwise copy; OutputGain is modelled as a dword because retail moves it with `mov eax`). Callers:
// 0x006E33E0 / 0x006E4E35 (the dialogue manager's list inserts). The retail list is SGI-shaped here (create-node then
// link, unlike the Dinkumware _Buynode(next, prev) families): modelled as a member of the list so `this` is reserved.
// Name (NF94 q11395): STLport 4.6 _list.h protected `_List_node<_Tp>* _M_create_node(const _Tp& __x)` (descriptive-by-library: Ego_r /
//   FableWin are Dinkumware builds, _List_val::_Buynode); the landed caller 0x006E31E0 binds this spelling. The row is now the
//   shared stlport/fable_containers.h list body over engine/NSpeechGainManager.h's CActiveDialogueInfo, with the opt-in set of
//   0x006E31E0 (whose TU emits the same COMDAT out of line); was a local CRetailList_CActiveDialogueInfo model.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_ATTACHEDDIALOGUE
#define FABLE_RETAIL_CTCSOUNDPLAYER_SETSPECIFICSAMPLEVOLUME
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ONDIALOGUESTART
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ACTIVEDIALOGUEINFO
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGlobalDef.h"
#include "engine/ThingAlive.h"
#include "engine/NSpeechGainManager.h"
template std::_List_node<NSpeechGainManager::CActiveDialogueInfo>*
std::list<NSpeechGainManager::CActiveDialogueInfo>::_M_create_node(const NSpeechGainManager::CActiveDialogueInfo&);
