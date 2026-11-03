#ifndef FABLE_ENGINE_CINTELLIGENTPOINTER_H
#define FABLE_ENGINE_CINTELLIGENTPOINTER_H
#include "rebuild_abi.h"
class CBaseObject;
class CBaseObjectPointer;
// Ego_r TPI CBaseIntelligentPointer: independent polymorphic class, PData +4;
// retail GetPItem 00a01b50 receives this+0 and reads PData +4.
class CBaseIntelligentPointer {
public:
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CAMERA_TRANSITIONS)
    CBaseIntelligentPointer();
#endif
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    void SetPItem(CBaseObject*);
#endif
private:
    CBaseObjectPointer* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CAMERA_TRANSITIONS)
    CIntelligentPointer() { }
#endif
    #ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    CIntelligentPointer(T* pointer) { SetPointer(pointer); }
    void Set(T* pointer) { SetPointer(pointer); }
private:
    void SetPointer(T* pointer) { SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer))); }
public:
#endif
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
};
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
#endif
