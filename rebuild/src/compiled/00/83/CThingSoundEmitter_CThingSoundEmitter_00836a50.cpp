// CThingSoundEmitter::~CThingSoundEmitter() -- hand the thing's sound emitter back to the sound manager (CASoundManager::DestroyPEmitter), clear the attached-thing intelligent pointer, then the member and CThing base destructors
// Leaf: ??1CThingSoundEmitter@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836A50, length 128 bytes (plain `ret`). Vtables: CThingSoundEmitter
// 0x01272C24 (stored at entry), CIntelligentPointer<CThing> 0x01237F1C (the temporary's).
// Callees: CASoundManager::DestroyPEmitter (PSoundManager 0x013B8394 slot 5, +0x14),
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, ::SetPItem 0x00A01B90, ::GetPItem 0x00A01B50,
// ::~CBaseIntelligentPointer 0x00A01C10, CThing::~CThing 0x004C8310.
// Owner note (audio layer): this is the CThingSoundEmitter thing class (true class, vtable 0x01272C24 verified by
// rtti-vtables.tsv); retail layout (Ego_r.pdb is packed here and lacks the +0x98 sound object, as recorded by the
// landed ctor 0x008367B0): sound object +0x98, AttachedToThing +0x9c, PAttachedThing (CIntelligentPointer<CThing>,
// 8 bytes) +0x9d (unaligned: `#pragma pack(1)`), ContinuePlayingAfterAttachedThingDeath +0xa5, Pos +0xa6.
// Shape: the clearing assignment `PAttachedThing = CIntelligentPointer<CThing>(0)` written out: a named cleared
// pointer (inline ctor stores the derived vtable after the base ctor; implicit dtor calls the base dtor), its
// GetPItem result in a named local, then the member's SetPItem -- only the named item local forms the member
// address (`lea edi,[esi+9Dh]`) after the GetPItem call as retail does; edi is reused for the member's dtor.
// Model (agentSND2, 2026-11-10): the shared engine/CThingSoundEmitter.h with the opt-ins FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
//   (+0x98 PSoundEmitter, the virtual destructor) and _MEMBERS (PAttachedThing / Pos typed, packed), engine/CASoundManager.h's
//   PSoundManager and slot 5 DestroyPEmitter; was a local CSoundSystem::ReleaseSoundObject stand-in and a local class model.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_MEMBERS
#include "engine/CASoundManager.h"
#include "engine/CThingSoundEmitter.h"

CThingSoundEmitter::~CThingSoundEmitter()
{
    if (PSoundEmitter)
        PSoundManager->DestroyPEmitter(PSoundEmitter);
    CIntelligentPointer<const CThing> cleared(0);
    const CThing* item = cleared.Get();
    PAttachedThing.Set(item);
}
