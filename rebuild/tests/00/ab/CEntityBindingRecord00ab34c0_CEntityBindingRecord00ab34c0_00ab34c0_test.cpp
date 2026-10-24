// Behaviour test for 0x00AB34C0: the binding record releases its owner pointer, destroys its binding vector (elements, then storage), then its name.
#include <cstdio>
#include <cstdlib>
static int g_log[32]; static int g_n = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
struct CCPPointerInfo { unsigned long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static void DeleteData(void* data) { (void)data; g_log[g_n++] = 50; }
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    void ReleaseInfo() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};
static CCPPointerInfo* NewInfo(unsigned long refs) { CCPPointerInfo* i = new CCPPointerInfo; i->RefCount = refs; i->DeleteFunc = DeleteData; i->Data = 0; return i; }
struct SNonscalarPtrIteratorTag { };
struct CBindingTarget;
typedef CCountedPointer<CBindingTarget> TBinding;
static void DestroyRange(TBinding* first, TBinding* last, const SNonscalarPtrIteratorTag&)
{
    g_log[g_n++] = 60;
    for (; first != last; ++first) first->~TBinding();
}
static void Free(void* block) { g_log[g_n++] = 61; std::free(block); }
struct CBindingVector {
    TBinding* _M_start; TBinding* _M_finish; TBinding* _M_end_of_storage;
    ~CBindingVector() { DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag()); if (_M_start) Free(_M_start); }
};
struct CBindingOwner;
struct CEntityBindingRecord {
    CCharString m_name; long m_value04; long m_value08; CBindingVector m_bindings; CCountedPointer<CBindingOwner> m_owner;
    ~CEntityBindingRecord() { }
};
int main()
{
    {
        CEntityBindingRecord r;
        r.m_name.id = 7;
        r.m_bindings._M_start = (TBinding*)std::malloc(2 * sizeof(TBinding));
        r.m_bindings._M_start[0].Data = 0; r.m_bindings._M_start[0].Info = NewInfo(1);
        r.m_bindings._M_start[1].Data = 0; r.m_bindings._M_start[1].Info = 0;
        r.m_bindings._M_finish = r.m_bindings._M_start + 2; r.m_bindings._M_end_of_storage = r.m_bindings._M_finish;
        r.m_owner.Data = 0; r.m_owner.Info = NewInfo(1);
    }
    // owner deleted (50), range destroyed (60, element 0 deleted 50), storage freed (61), name (7)
    bool ok1 = g_n == 5 && g_log[0] == 50 && g_log[1] == 60 && g_log[2] == 50 && g_log[3] == 61 && g_log[4] == 7;
    g_n = 0;
    {
        CEntityBindingRecord r;
        r.m_name.id = 9;
        r.m_bindings._M_start = 0; r.m_bindings._M_finish = 0; r.m_bindings._M_end_of_storage = 0;
        r.m_owner.Data = 0; r.m_owner.Info = 0;
    }
    bool ok2 = g_n == 2 && g_log[0] == 60 && g_log[1] == 9;   // empty vector: no free
    if (!(ok1 && ok2)) { std::printf("00ab34c0_TEST FAIL\n"); return 1; }
    std::printf("00ab34c0_TEST PASS\n");
    return 0;
}