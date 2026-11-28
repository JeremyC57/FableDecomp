#ifndef FABLE_CSAL_BUFFER_H
#define FABLE_CSAL_BUFFER_H
#ifdef FABLE_RETAIL_CSALSW_UPDATE_PASS30
#include "engine/CCodaObject.h"
#include "engine/CodaBufferDesc.h"
#include "engine/CSalOpenALBuffer.h"
// Both original GameCODA CodeView records name the second base and count.
// Retail GetDesc returns this+16, immediately after CCodaObject and this count.
class CCodaRefCounter {
    unsigned int m_uiRefCount;
};
class CSalBufferBase : public CCodaObject, public CCodaRefCounter {
public:
    const _CBUFFERDESC& GetDesc() const;
protected:
    _CBUFFERDESC m_Desc;
    unsigned int m_ffStatus;
};
class CSalBuffer : public CSalBufferBase {
#ifdef FABLE_RETAIL_CSALBUFFER_USE_PORT30
public:
    int Use();
private:
#endif
    CSalOpenALBuffer m_OALBuffer;
};
#endif
#endif
