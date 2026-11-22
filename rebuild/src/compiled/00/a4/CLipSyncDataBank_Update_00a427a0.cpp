// CLipSyncDataBank::Update() -- the per-frame bank update: skipped while SkipUpdate is set, else the base CBankFile::Update (a tail jump)
// Leaf: ?Update@CLipSyncDataBank@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A427A0: `mov al,[ecx+178h] ; test al,al ; jne ret ; jmp 0x009CEC60`. Callee: CBankFile::Update 0x009CEC60.
// Identity: Ego_r.pdb and FableWin.pdb ?Update@CLipSyncDataBank@@UAEXXZ (public virtual: vtable 0x0129D3D4 slot 7 holds 0x00A427A0, where the
//   CBankFile-derived banks' vtables hold 0x009CEC60); SkipUpdate +0x178 (Ego_r.pdb member, engine/CLipSyncDataBank.h).
// Names (agentNF254, 2026-11-22, CODEXP27 L111): was the stand-in ?Update@CLipSyncDataBank_Methods@@QAEXXZ on a _Methods struct derived from the
//   flat CLipSyncDataBank model, binding 0x009CEC60 as ?DoUpdate@CLipSyncDataBank_Methods@@QAEXXZ; now the retail class
//   (FABLE_RETAIL_CLIPSYNCDATABANK_CLASS: CLipSyncDataBank : CBankFileAsync) with the new add-only declaration opt-in
//   FABLE_RETAIL_CLIPSYNCDATABANK_UPDATE, calling the base by its own spelling ?Update@CBankFile@@UAEXXZ (the 0x009CEC60 self leaf).
// Prelude: the #define / #include block of the landed same-TU 0x00A42980 CLipSyncDataBankEntry::BeginLoading (the CLASS opt-in's user).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CLIPSYNCDATABANK_CLASS
#define FABLE_RETAIL_CLIPSYNCDATABANK_UPDATE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"
#include "engine/CLipSyncDataBank.h"

void CLipSyncDataBank::Update()
{
    if (!SkipUpdate)
        CBankFile::Update();
}
