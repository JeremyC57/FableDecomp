// PKCAutoFile::PKCAutoFile(CAudioFile* file) -- the libpk auto-closing file holder: keep the opened file in m_pFile (+0).
// Leaf: ??0PKCAutoFile@@QAE@PAVCAudioFile@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C31D80, 11 bytes (`mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; ret 4`).
// Identity (NF56, round 7, queue q10486): libpk/PKCAutoFile.h (descriptive libpk name, no libpk PDB; the retail assertion text names
//   PKCAutoFile.cpp for GetChecked 0x00C31DB0, nf44:123). Both callers construct it on the result of the processor's slot-0 open and destroy
//   it with 0x00C31D90: _global::Process_00c172f0 0x00C172F0 (already binds this spelling) and LoadBankFile 0x00C174A0 (local PKCAutoFile(void*)
//   model, NAME_FIXES). The catalog name __EH_epilog3 and the old stand-in T::Assign(int) were wrong: the body is a one-member constructor
//   (the stored argument, `this` returned in eax).
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/PKCAutoFile.h"

PKCAutoFile::PKCAutoFile(CAudioFile* file)
    : m_pFile(file)
{
}
