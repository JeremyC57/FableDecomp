#ifndef FABLE_LIBPK_CAUDIOFILE_H
#define FABLE_LIBPK_CAUDIOFILE_H
// Descriptive original-name-unavailable interface: native base vtable012B4F38.
// Slots are independently confirmed by the default/external file-handle adapters.
class CAudioFile {
public:
    virtual bool GetSize(unsigned long* size)=0;
    virtual bool Read(unsigned long count,long unused,void* a,void* b,void* c,void* d)=0;
    virtual unsigned long GetUnitSize()=0;
    virtual void Close()=0;
    virtual ~CAudioFile();
};
#endif
