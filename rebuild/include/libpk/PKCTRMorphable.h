#ifndef FABLE_LIBPK_PKCTRMORPHABLE_H
#define FABLE_LIBPK_PKCTRMORPHABLE_H
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/PKCTRList.h"
#include "rebuild_abi.h"
// NativeC185B0 sourcePKCTRMorphable.cpp; exact15-slot interface,ownednode+4.
class PKCTRMorphable : public PKCContainer_00c17b70 {
public:
    PKCTRMorphable(long mode);
    virtual ~PKCTRMorphable();
    virtual int IsArray();
    virtual int IsTree();
    virtual void MorphToArray(void*);
    virtual void MorphToTree(void*);
    virtual void Reserve(long);
    virtual void Add(void*,void*);
    virtual void Remove(void*,void*);
    virtual void* Find(void*,const void*);
    virtual void* FindByKey(void*,const void*);
    virtual void* GetItemByIndex(long);
    virtual long GetCount();
    virtual void* GetFirst();
    virtual void* GetLast();
    virtual void ForEach(void*);
    PKCContainer_00c17b70* Implementation;
};
FABLE_STATIC_ASSERT(sizeof(PKCTRMorphable)==8);
#endif
#endif
