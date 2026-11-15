#ifndef FABLE_LIBPK_CDEFAULTAUDIOFILE_H
#define FABLE_LIBPK_CDEFAULTAUDIOFILE_H
#include "libpk/CAudioFile.h"
#include "rebuild_abi.h"
class PKCDefaultFileManager;
// Descriptive name; original native constructor00C32FC0 and vtable012B4F4C.
// Layout: manager+4,key+8,base vptr+0; base table012B4F38 has the same five slots.
class CDefaultAudioFile:public CAudioFile {
public:
    CDefaultAudioFile(PKCDefaultFileManager* manager,unsigned long key);
    virtual bool GetSize(unsigned long* size);
    virtual bool Read(unsigned long count,long unused,void* a,void* b,void* c,void* d);
    virtual unsigned long GetUnitSize();
    virtual void Close();
    virtual ~CDefaultAudioFile();
private:
    PKCDefaultFileManager* m_pManager;
    unsigned long m_key;
};
FABLE_STATIC_ASSERT(sizeof(CDefaultAudioFile)==12);
#endif
