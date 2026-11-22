#ifndef FABLE_CSAL_OPENAL_BUFFER_H
#define FABLE_CSAL_OPENAL_BUFFER_H
#ifdef FABLE_RETAIL_CODA_OPENAL_BUFFER_PASS26
#include "engine/CodaBufferDesc.h"
class CSalOpenALBuffer {
public:
    CSalOpenALBuffer();
    ~CSalOpenALBuffer();
    int Create(const _CBUFFERDESC&);
    void Release();
    bool IsCreated() const;
    int SetLocalDesc(const _CBUFFERDESC&);
    unsigned int GetID() const;
    int GetStereoID(unsigned int&);
private:
    int MakeStereoBuffer();
    unsigned int m_uiBuffer[2];
    unsigned int m_ffStatus;
    _CBUFFERDESC* m_pDesc;
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CSalOpenALBuffer) == 0x10);
#endif
#endif
