#ifndef FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLESUBCOMPONENTGENERICTYPE_H
#define FABLE_ENGINE_CPERSISTCONTEXT_TRANSFERABLESUBCOMPONENTGENERICTYPE_H
class CPersistContext;
class CPersistContext_TransferableSubComponentGenericType {
public:
    virtual void Transfer(CPersistContext&)=0;
    virtual bool AreDefaultValsApplied()=0;
    virtual void SetDefaulValsApplied(bool)=0;
};
#endif
