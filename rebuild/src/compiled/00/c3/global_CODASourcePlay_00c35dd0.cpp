// CODA_SourcePlay
// Flags:
// Leaf: _CODA_SourcePlay@8
// Retail: 0x00c35dd0, 89 bytes, stdcall.
#define FABLE_RETAIL_CCODACONTEXT_SETERROR
#include "engine/CCodaSource.h"
#include "engine/CCodaContext.h"
extern CCodaContext CODA;
extern "C" CBOOL FABLE_STDCALL CODA_SourcePlay(CHANDLE hSource,CBOOL bLooping){
 CCodaSource* pSource=static_cast<CCodaSource*>(hSource);
 if(!pSource->IsBufferSet()){CODA.SetError(C_ERR_NOTALLOCATED);return 0;}
 int error=pSource->Play(bLooping!=0);
 if(error){CODA.SetError(error);return 0;}
 return 1;
}