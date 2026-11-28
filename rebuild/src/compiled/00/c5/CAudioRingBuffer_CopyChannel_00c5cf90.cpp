// 00c5cf90 CSalSwBuffer::CopyChannel
// Leaf: ?CopyChannel@CSalSwBuffer@@QAEXII@Z
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CSALSW2D_CALCPARAMS_PASS29
#define FABLE_RETAIL_CSALSW_UPDATE_PASS30
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_CSALSWBUFFER_ISEMPTY
#define FABLE_RETAIL_CODA_OPENAL_BUFFER_PASS26
#include "engine/CSalSwBaseVoice.h"
#include "engine/CCodaSource.h"
#include "engine/CodaCaps.h"
#include "engine/CSalPlatform.h"
#include "engine/CCodaEAX.h"
#include "engine/CSalOpenALReverb.h"
#include "engine/CSalBuffer.h"
#include "engine/CodaPCM.h"
#include <string.h>


void CSalSwBuffer::CopyChannel(unsigned int nDestChannel, unsigned int nSrcChannel)
{
    unsigned int channelSize=GetNumObjects()*GetObjectSize();
    memcpy((unsigned char*)m_pvBuffer+nDestChannel*channelSize,
           (const unsigned char*)m_pvBuffer+nSrcChannel*channelSize,channelSize);
}
