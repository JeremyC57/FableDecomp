#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_NGAMETEXT_CDATABANK
#define FABLE_ENGINE_GFMAIN_NGAMETEXT_CDATABANK
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CBankFileAsync.h"
#include "engine/CResourceBank.h"
#include "engine/CIVCountedPointer.h"
namespace NGameText{
class CTextEntry;
class CDataBank:public CBankFileAsync,public CResourceBank{
 unsigned char Entries[12],TextEntryBuffer[12],GroupEntryBuffer[12],Narrators[12];unsigned long Seed;
public:CDataBank();virtual~CDataBank();CIVCountedPointer<const CTextEntry>GetPTextEntry(unsigned long,bool)const;
};
}
FABLE_STATIC_ASSERT(sizeof(NGameText::CDataBank)==0x214);
#endif

#else

#endif
