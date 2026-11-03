#ifndef FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTORGENERICTYPE_H
#define FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLEVECTORGENERICTYPE_H
class CPersistContext; class CCharString; class CStringParser;
// Complete original vptr-only interface, public slots0/4/8/c; both PDBs.
class CPersistContext_TransferableVectorGenericType {
public:
    virtual long Size()=0;
    virtual void Resize(long)=0;
    virtual void TransferOut(CPersistContext&,CCharString&,long)=0;
    virtual void TransferIn(CPersistContext&,CStringParser&,long)=0;
};
#endif
