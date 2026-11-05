// CGameScriptInterface::CloseChest(const CScriptThing& thing) -- Forward to the owned component after checking script lifetime.
// Leaf: ?CloseChest@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: PDB exact virtual const signature; own interface kind60 and external callee007506c0 CTCChest::Close.
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
class CTCEnemy:public CTCBase {public:bool FriendsWithEverythingFlag,EnableFollowersEnemyProxy;void AddEnemyThing(const CThing*);void RemoveEnemyThing(const CThing*);void RemoveAllyThing(const CThing&);};
// Layout: shared CScriptThing header is flattened. A genuine polymorphic interface is required for GetPThing slot11 and IsAlive slot75; uninvoked slots have descriptive placeholders only. PImp data/info follow at+4/+8.
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Layout: only external method receivers; no active chest/mode-manager headers, no derived fields read. Base offsets agree with CTCBase.
class CTCChest:public CTCBase {public:void Close();void DisplayLockedMessage()const;};
class CTCCreatureModeManager:public CTCBase {public:void ResetModes();};
// Layout: shared script header flattened; genuine virtual API below, no owner fields touched.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::CloseChest(const CScriptThing& thing)const{if(thing.IsAlive()){CThing* p=thing.GetPThing();CTCChest* component;if(p->GetTC(TCI_CONTAINER, &component))component->Close();}}