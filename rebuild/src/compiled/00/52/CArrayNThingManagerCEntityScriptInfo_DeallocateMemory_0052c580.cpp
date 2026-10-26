// CArray<NThingManager::CEntityScriptInfo>::DeallocateMemory() -- release the array's storage: erase every element, detach and zero the three pointers, destroy the detached range and free the block
// Leaf: ?DeallocateMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052C580, length 123 bytes (+ int3 padding); called twice by NThingManager::CEntityRuntimePersistence::Clear 0x005236F0 (landed; its two persistent entity-script
// arrays). The manifest's CArray<std::pair<unsigned long,...>> label is propagated; `imul 2AAAAAABh ; sar 2` is the 24-byte stride.
// Callees: the element copy-down 0x0052BD10 (__fastcall: first in ecx, last in edx, result + an empty tag + a null word on the
// stack), the range destroy 0x0052BD70 (__fastcall first, last, tag), free 0x00BFEA14.
// Shape: the CArray<CMapInfo>::DeallocateMemory 0x0051A350 form (same TU-independent template): erase(begin(), end()) is
// `copy(end, end, begin)` + the destroy of the left-over tail + the finish store; the detach goes through `Exchange(slot, 0)`
// (a store through a `T*&` keeps the member reload after it); both empty tags are one-byte temporaries homed in the
// `push ecx` frame word.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct CEntityScriptInfo {                      // 0x18 bytes (NThingManager, landed Clear 0x005236F0)
    unsigned char _body[0x18];
};
struct SCopyTag { SCopyTag() { } };
struct __false_type { __false_type() { } };

CEntityScriptInfo* __fastcall CopyDown(CEntityScriptInfo* first, CEntityScriptInfo* last, CEntityScriptInfo* result, const SCopyTag& tag, long* distance);   // 0x0052BD10
void __fastcall DestroyRange(CEntityScriptInfo* first, CEntityScriptInfo* last, const __false_type& tag);   // 0x0052BD70

template<class T> class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    void erase(T* f, T* l)
    {
        T* i = CopyDown(l, _M_finish, f, SCopyTag(), (long*)0);
        DestroyRange(i, _M_finish, __false_type());
        _M_finish = i;
    }
    void DeallocateMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<CEntityScriptInfo>::DeallocateMemory()
{
    if (capacity() > 0) {
        erase(_M_start, _M_finish);
        CEntityScriptInfo* p = Exchange(_M_start, (CEntityScriptInfo*)0);
        CEntityScriptInfo* l = Exchange(_M_finish, (CEntityScriptInfo*)0);
        _M_end_of_storage = 0;
        DestroyRange(p, l, __false_type());
        if (p)
            free(p);
    }
}