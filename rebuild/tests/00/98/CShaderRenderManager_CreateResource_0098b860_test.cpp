// Behaviour test for 0098b860: body copied from src/0098b860.cpp (CShaderRenderManager::CreateResource), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SCountBlock {
    long m_references; void (__fastcall* m_pfnDestroy)(void*); void* m_pObject;
    SCountBlock(void (__fastcall* destroy)(void*), void* object) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};
static int g_deleteData = 0; static void* g_deleted = 0;
template<class T> class CCountedPointer { public:
    T* Object; SCountBlock* Count;
    static void __fastcall DeleteData(void* object) { ++g_deleteData; g_deleted = object; }
    CCountedPointer(T* object) : Object(object), Count(object ? new SCountBlock(DeleteData, object) : 0) {}
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    ~CCountedPointer()
    {
        if (Count) {
            if (--Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
    }
    T* operator->() const { return Object; }
};
class CShaderRenderManager;
static int g_ctor = 0, g_init = 0; static CShaderRenderManager* g_initManager = 0;
class CShaderResource { public:
    CShaderResource() { ++g_ctor; memset(_body, 0, sizeof _body); }
    void Initialise(CShaderRenderManager& manager) { ++g_init; g_initManager = &manager; }
    unsigned char _body[0x3c];
};
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
static int g_overflow = 0;
struct CShaderResourceArray {
    CCountedPointer<CShaderResource>* m_start; CCountedPointer<CShaderResource>* m_finish; CCountedPointer<CShaderResource>* m_end_of_storage;
    void _M_insert_overflow(CCountedPointer<CShaderResource>* position, const CCountedPointer<CShaderResource>& x, const __false_type&, unsigned long fill_len, bool atend)
    { ++g_overflow; CHECK(position == m_finish && fill_len == 1 && atend); new (m_finish) CCountedPointer<CShaderResource>(x); ++m_finish; ++m_end_of_storage; }
    void push_back(const CCountedPointer<CShaderResource>& x)
    {
        if (m_finish != m_end_of_storage) {
            new (m_finish) CCountedPointer<CShaderResource>(x);
            ++m_finish;
        }
        else
            _M_insert_overflow(m_finish, x, __false_type(), 1, true);
    }
};
class CShaderRenderManager { public:
    void* __vftable; unsigned char _pad004[0x3a4]; CShaderResourceArray ResourceManagers;
    CCountedPointer<CShaderResource> CreateResource();
};
CCountedPointer<CShaderResource> CShaderRenderManager::CreateResource()
{
    CCountedPointer<CShaderResource> resource(new CShaderResource);
    resource->Initialise(*this);
    ResourceManagers.push_back(resource);
    return resource;
}
int main()
{
    unsigned char storage[sizeof(CCountedPointer<CShaderResource>) * 4]; memset(storage, 0, sizeof storage);
    CShaderRenderManager mgr; mgr.ResourceManagers.m_start = (CCountedPointer<CShaderResource>*)storage; mgr.ResourceManagers.m_finish = mgr.ResourceManagers.m_start; mgr.ResourceManagers.m_end_of_storage = mgr.ResourceManagers.m_start + 1;
    {
        CCountedPointer<CShaderResource> r = mgr.CreateResource();
        CHECK(g_ctor == 1 && g_init == 1 && g_initManager == &mgr && g_overflow == 0);
        CHECK(mgr.ResourceManagers.m_finish - mgr.ResourceManagers.m_start == 1 && mgr.ResourceManagers.m_start[0].Object == r.Object);
        CHECK(r.Count->m_references == 2 && g_deleteData == 0);        // the array's copy + the returned one
    }
    CHECK(mgr.ResourceManagers.m_start[0].Count->m_references == 1 && g_deleteData == 0);
    {
        CCountedPointer<CShaderResource> r2 = mgr.CreateResource();   // array full: overflow path
        CHECK(g_ctor == 2 && g_overflow == 1 && r2.Count->m_references == 2 && mgr.ResourceManagers.m_start[1].Object == r2.Object);
    }
    mgr.ResourceManagers.m_start[1].~CCountedPointer<CShaderResource>();   // dropping the last reference destroys the resource
    CHECK(g_deleteData == 1 && g_deleted == mgr.ResourceManagers.m_start[1].Object);
    if (fails == 0) printf("0098b860_TEST PASS\n"); else printf("0098b860_TEST FAIL (%d)\n", fails);
    return fails;
}