#ifndef FABLE_RETAIL_HEADER_CDATASTREAM
#define FABLE_RETAIL_HEADER_CDATASTREAM
#include "engine/CBaseClassNonCopyable.h"
class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream();
    virtual void SetPosition(unsigned long)=0;
    virtual unsigned long GetPosition() const=0;
    virtual void Close()=0;
};
#endif
