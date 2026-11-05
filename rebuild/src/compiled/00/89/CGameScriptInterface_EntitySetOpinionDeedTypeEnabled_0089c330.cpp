// CGameScriptInterface::EntitySetOpinionDeedTypeEnabled(const CScriptThing& thing,EOpinionDeedType type,bool enabled) const -- Set one opinion mask flag.
// Leaf: ?EntitySetOpinionDeedTypeEnabled@CGameScriptInterface@@UBEXABVCScriptThing@@W4EOpinionDeedType@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: PDB exact enum/virtual const API; original DeedMask shared-header member. Retail is a12-byte vector of byte bools, not a packed-bit specialization (indexed byte store).
#include "engine/CTCBase.h"
#include "engine/CTCTargeted.h"
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CArray.h"

// Layout: no active CThing header; original UsingComponents+0x20, Components+0x44 from retail. Nine32-bit bitset words precede the16-byte CVectorMap. Only these members are used.

// Layout: no CTCEnemy header. PDB original bool fields at+12/+13 after the shared CTCBase; later lists/faction untouched and omitted.
class CTCEnemy:public CTCBase {public:bool FriendsWithEverythingFlag,EnableFollowersEnemyProxy;};
// Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CTCCreatureOpinionOfHero.h"
enum EOpinionDeedType {OPINION_DEED_TEST=1};enum EOpinionAttitudeType{OPINION_ATTITUDE_TEST=1};enum EOpinionReactionType{OPINION_REACTION_TEST=1};
// Layout: use shared opinion header opaque container storage via its actual STLport vector<bool> type; offsets+0x24,+0x30,+0x3c are confirmed directly. Script owner fields unused, real virtual declaration restores API.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetOpinionDeedTypeEnabled(const CScriptThing& thing,EOpinionDeedType type,bool enabled)const{CThing* p=thing.GetPThing();if(p){CTCCreatureOpinionOfHero* component;if(p->GetTC(TCI_CREATURE_OPINION_OF_HERO, &component))(*reinterpret_cast<std::vector<bool>*>(component->DeedMask))[type]=enabled;}}