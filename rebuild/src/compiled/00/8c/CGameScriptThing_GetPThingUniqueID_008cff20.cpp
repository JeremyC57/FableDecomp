// CGameScriptThing::GetPThingUniqueID() const -- the unique ID of the thing this script handle refers to (ThingUID)
// Leaf: ?GetPThingUniqueID@CGameScriptThing@@UBE_KXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CFF20, 7 bytes: `mov eax,[ecx+18h] ; mov edx,[ecx+1Ch] ; ret` (unsigned __int64 in edx:eax).
// Identity (two sources): (1) slot 12 of the CGameScriptThing vtable 0x012852CC (RTTI .?AVCGameScriptThing@@ : CScriptThing), the
//   CScriptThing GetPThingUniqueID slot (engine/CScriptThing.h: slot 12, base 0x004AAA70); (2) Ego_r.pdb / FableWin.pdb
//   ?GetPThingUniqueID@CGameScriptThing@@UBE_KXZ, and +0x18 is the Ego_r member ThingUID (ulong64).
// Header: engine/CGameScriptThing.h (FABLE_RETAIL_CGAMESCRIPTTHING, FABLE_RETAIL_CGAMESCRIPTTHING_UNIQUEID new), engine/CScriptThing.h
//   (FABLE_RETAIL_NSCRIPT), engine/CCountedPointer.h, engine/CIntelligentPointer.h, engine/C3DVector.h, fable_string.h (the contract types).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTTHING
#define FABLE_RETAIL_CGAMESCRIPTTHING_UNIQUEID
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptThing.h"
FableNScriptUInt64 CGameScriptThing::GetPThingUniqueID() const
{
    return ThingUID;
}