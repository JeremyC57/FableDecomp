// Behaviour test for 0x00c46a70 PKCTRList<T>::GetFirst: the list forwards to its container implementation (+4) and passes the result
// back unchanged. Self-contained: a recording container stands in for the libpk PKCContainer_00c17b70 interface.
#include <stdio.h>

struct Item { int id; };

class PKCContainer_00c17b70 {                    // recording fake of the container interface (same slot order)
public:
    int last; long arg; void* ret; long count;
    PKCContainer_00c17b70() : last(-1), arg(0), ret(0), count(0) { }
    virtual int IsArray() { last = 0; return 0; }
    virtual int IsTree() { last = 1; return 1; }
    virtual void MorphToArray(void*) { last = 2; }
    virtual void MorphToTree(void*) { last = 3; }
    virtual void Reserve(long n) { last = 4; arg = n; }
    virtual void Add(void*, void*) { last = 5; }
    virtual void Remove(void*, void*) { last = 6; }
    virtual void* Find(void*, const void*) { last = 7; return 0; }
    virtual void* FindByKey(void*, const void*) { last = 8; return 0; }
    virtual void* GetItemByIndex(long i) { last = 9; arg = i; return ret; }
    virtual long GetCount() { last = 10; return count; }
    virtual void* GetFirst() { last = 11; return ret; }
    virtual void* GetLast() { last = 12; return ret; }
    virtual void ForEach(void*) { last = 13; }
    virtual ~PKCContainer_00c17b70() { }
};

template <class T> class PKCTRList {
public:
    explicit PKCTRList(PKCContainer_00c17b70* impl) : m_pImpl(impl) { }
    virtual ~PKCTRList() { }
    virtual T* GetFirst();
protected:
    PKCContainer_00c17b70* m_pImpl;
};

template <class T> T* PKCTRList<T>::GetFirst()
{
    return static_cast<T*>(m_pImpl->GetFirst());
}

int main()
{
    int ok = 1;
    Item item; item.id = 1;
    PKCContainer_00c17b70 c;
    PKCTRList<Item> list(&c);
    c.ret = &item; if (list.GetFirst() != &item || c.last != 11) ok = 0;
    c.ret = 0; if (list.GetFirst() != 0 || c.last != 11) ok = 0;
    if (!ok) { printf("00c46a70_TEST FAIL\n"); return 1; }
    printf("00c46a70_TEST PASS\n");
    return 0;
}