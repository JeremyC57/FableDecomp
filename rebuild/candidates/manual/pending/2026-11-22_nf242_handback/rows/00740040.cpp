// NCreatureMode::CModeSitBench::CModeSitBench(CThing&) -- base CModeSitBase(thing), own vtable, the CAnimFilterOfGroup member at +0x2c
// built from CCharString("SIT_BENCH"): string assigned, CRC of the name stored
// Leaf: ??0CModeSitBench@NCreatureMode@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00740040, 116 bytes, `ret 4`: CModeSitBase::CModeSitBase(CThing&) 0x008D7CA0, the vtable
//   0x01267BFC (12 slots: ??_E 0x007400E0, GetType 0x007400C0, CModeSitBase AnimUpdate / FrameUpdate 0x008D6AC0 / 0x008D6C80,
//   GetAnimFilter 0x007400D0, CModeSitBase::OnActivate 0x008D6580, then the CModeBase slots 6..11), CCharString(const char*, long)
//   0x0099EBF0, the filter's inline constructor (vptr 0x01267568, CCharString() 0x0099E4B0, operator= 0x0099EFB0, CCRC::Calc
//   0x004014A0 over the name), ~CCharString 0x0099EAE0 on the temporary.
// NF240 (q12674): ported off the row-local CModeSitBench / CModeSitBase / CAnimFilterOfGroup / CCharString / CCRC models onto
//   engine/NCreatureMode_CModeSitFloor.h (the PASS27 class; Ego_r.pdb / FableWin.pdb / Ego_d.pdb ??0CModeSitBench@NCreatureMode@@QAE@AAVCThing@@@Z,
//   the spelling 0x00742790 and 0x00938380 bind) with the filter's inline constructor (engine/CAnimFilterOfGroup.h INLINE_CTOR, as
//   the sibling mode ctor 0x00743600). The vtable is now ??_7CModeSitBench@NCreatureMode@@6B@ with every slot in PDB spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_FRAMEUPDATE
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ANIMUPDATE
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ONACTIVATE
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_PASS27
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_IMPLICIT_DTOR
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/NCreatureMode_CModeSitFloor.h"

NCreatureMode::CModeSitBench::CModeSitBench(CThing& thing)
    : CModeSitBase(thing), Filter("SIT_BENCH")
{
}
