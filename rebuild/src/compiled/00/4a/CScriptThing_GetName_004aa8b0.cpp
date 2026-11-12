// CScriptThing_Methods::GetName  retail 0x004aa8b0
// mov eax,[ecx+4]; test eax,eax; jnz V; mov eax,offset CScriptThing::NullString; ret
// V: mov edx,[eax]; mov ecx,eax; jmp/call [edx+4]  (virtual slot 1 on PImp_Data)
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (Ego_r.pdb + FableWin.pdb `dynamic initializer
//   for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09), reached through the hdr opt-in
//   FABLE_RETAIL_CSCRIPTTHING_NULLSTRING on the flattened engine/CScriptThing.h; was g_def_004aa8b0 (an int).
#define FABLE_RETAIL_CSCRIPTTHING_NULLSTRING   // hdr overlay (agentNF26)
#include "engine/CScriptThing.h"  // retyped onto the PDB layout; byte parity re-verified

// The header models the pointee's vtable as a plain `void* __vftable`; the
// virtual-slot dispatch is reproduced through a local vtable view cast from PImp_Data.
struct CScriptThingVirt {
  virtual void* d0(){return 0;}
  virtual void* GetName();
};

struct CScriptThing_Methods : CScriptThing {
  void* GetName();
};

void* CScriptThing_Methods::GetName(){
  CScriptThingVirt* p=(CScriptThingVirt*)this->PImp_Data;
  if(!p) return &NullString;
  return p->GetName();
}
