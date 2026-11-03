#ifndef FABLE_ENGINE_CINTELLIGENTPOINTER_H
#define FABLE_ENGINE_CINTELLIGENTPOINTER_H
#include "rebuild_abi.h"
class CBaseObject;
class CBaseObjectPointer;
// Ego_r TPI CBaseIntelligentPointer: independent polymorphic class, PData +4;
// retail GetPItem 00a01b50 receives this+0 and reads PData +4.
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
private:
    CBaseObjectPointer* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
};
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
#endif
