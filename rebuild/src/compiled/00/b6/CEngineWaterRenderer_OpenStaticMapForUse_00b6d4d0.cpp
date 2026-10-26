// CEngineWaterRenderer::OpenStaticMapForUse(CStaticMapBankFile* file, CCharString seaEntryPrefix) -- remember the static map bank file and copy the by-value sea entry prefix into StaticMapSeaEntryPrefix; the by-value string is destroyed before returning
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D4D0, length 37 bytes (the oracle row carries 11 bytes of int3 padding).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0 (`add ecx,5ACh` forms the member's address), CCharString::~CCharString @ 0x0099EAE0
// on the argument slot (`lea ecx,[esp+8]`): a by-value class parameter the callee destroys. `ret 8`: pointer + 4-byte string. The engine
// header carries the Ego PDB layout (StaticMapBankFile +0x5ec, StaticMapSeaEntryPrefix +0x5f0); retail has them at +0x5a8/+0x5ac
// (the retail CArray is 12 bytes), so the retail layout is modelled locally.
#include "fable_string.h"

class CStaticMapBankFile;

class CEngineWaterRenderer { public:
    unsigned char       _pad0[0x5a8];
    CStaticMapBankFile* StaticMapBankFile;         // +0x5a8
    CCharString         StaticMapSeaEntryPrefix;   // +0x5ac
    void OpenStaticMapForUse(CStaticMapBankFile* file, CCharString seaEntryPrefix);
};

void CEngineWaterRenderer::OpenStaticMapForUse(CStaticMapBankFile* file, CCharString seaEntryPrefix)
{
    StaticMapBankFile = file;
    StaticMapSeaEntryPrefix = seaEntryPrefix;
}