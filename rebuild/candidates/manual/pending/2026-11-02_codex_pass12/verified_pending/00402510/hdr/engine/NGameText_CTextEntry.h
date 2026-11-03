#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_NGAMETEXT_CTEXTENTRY
#define FABLE_ENGINE_GFMAIN_NGAMETEXT_CTEXTENTRY
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CResource.h"
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
namespace NGameText{
class CTextEntry:public CResource{
 CCharString SpeechBank,Narrator,SoundName;CWideString Text;unsigned char Tags[12],PFileData[8];class CDataBank*Owner;
public:CWideString GetText()const;
};
}
FABLE_STATIC_ASSERT(sizeof(NGameText::CTextEntry)==0x44);
#endif

#else

#endif
