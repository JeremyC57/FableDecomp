#ifndef FABLE_LIBPK_PKCTRREDBLACKTREE_H
#define FABLE_LIBPK_PKCTRREDBLACKTREE_H
#if defined(FABLE_RETAIL_LIBPK_EVENT_SCRIPT_CTOR_PASS23)
#include "libpk/PKCTRList.h"
#include "rebuild_abi.h"
// Descriptive libpk identity, independently supported by native source-file strings.
// Native vtable012b2f0c has the exact15-slot container prefix; size40 from
// enclosing constructor/member boundaries and tree constructor C194F0 stores.
struct PKCTRAllocator;
struct PKCTRRedblackNode {
    PKCTRRedblackNode* Left;
    PKCTRRedblackNode* Right;
    PKCTRRedblackNode* Parent;
    long Colour;
    void* Key;
};
class PKCTRRedblackTree : public PKCContainer_00c17b70 {
public:
    PKCTRRedblackTree(PKCTRAllocator* allocator);
    virtual ~PKCTRRedblackTree();
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
public:
    PKCTRAllocator* PAllocator;
    PKCTRAllocator* POwnedAllocator;
    PKCTRRedblackNode Sentinel;
    PKCTRRedblackNode* Root;
    long Count;
};
FABLE_STATIC_ASSERT(sizeof(PKCTRRedblackTree)==40);
#endif
#endif
