// CScriptBase::RunSpawnedFunctions() -- step every spawned script thread once and drop the threads that report they are finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7C40, length 107 bytes (+5 int3 padding).
// PDB: CScriptBase::Threads is list<CCountedPointer<CSpawnedFuncBase> > (retail: the STLport head-node pointer at
// +0x04). Callees: CScriptBase::UpdateSpawnedFunction(CSpawnedFuncBase*) @ 0x00CB7950 (true = finished),
// operator delete @ 0x00BFE9BC (the count block), free @ 0x00BFEA14 (the list node, STLport malloc allocator).
// Shape: `it = Threads.erase(it)` / `++it` loop with end() re-read every pass; STLport's inline erase unlinks
// (next in edi), runs the counted pointer's inline destructor (null test, `dec [count]`, re-read, destroy +
// delete, both words zeroed; one zero register ebp) and frees the node.
struct CSpawnedFuncBase;

struct SCountBlock {
    long References;                                 // +0
    void (__fastcall* Destroy)(void*);               // +4
    void* Object;                                    // +8
};

template <class T> struct CCountedPointer {
    T* Data;
    T* GetPtr() const { return Data; }
    SCountBlock* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->References;
            if (Info->References == 0)
            {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

extern "C" void __cdecl free(void* p);              // 0x00BFEA14

template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> struct list {                     // STLport list: the head node pointer
    struct _Node_base { _Node_base* _M_next; _Node_base* _M_prev; };
    struct _Node : _Node_base { T _M_data; };
    struct iterator {
        _Node_base* _M_node;
        iterator(_Node_base* n) : _M_node(n) {}
        T& operator*() const { return ((_Node*)_M_node)->_M_data; }
        T* operator->() const { return &((_Node*)_M_node)->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _Node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator erase(iterator position)
    {
        _Node_base* next = position._M_node->_M_next;
        _Node_base* prev = position._M_node->_M_prev;
        _Node* n = (_Node*)position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        _Destroy(&n->_M_data);
        free(n);
        return iterator(next);
    }
};

class CScriptBase {
public:
    virtual ~CScriptBase();
    void RunSpawnedFunctions();
    bool UpdateSpawnedFunction(CSpawnedFuncBase* thread);   // 0x00CB7950
private:
    list<CCountedPointer<CSpawnedFuncBase> > Threads;      // +0x04
};

void CScriptBase::RunSpawnedFunctions()
{
    for (list<CCountedPointer<CSpawnedFuncBase> >::iterator it = Threads.begin(); it != Threads.end(); )
    {
        if (UpdateSpawnedFunction(it->GetPtr()))
            it = Threads.erase(it);
        else
            ++it;
    }
}