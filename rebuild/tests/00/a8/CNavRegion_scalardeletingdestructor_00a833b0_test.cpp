// Behaviour test for 0x00A833B0, the scalar deleting destructor of a record holding a counted-pointer
// list at +4: the list is cleared, its sentinel node freed when non-null, then the object is deleted.
#include <cstdio>
#include <cstdlib>

static int g_log[16];
static int g_n;
static void* g_freedNode;
static void Log(int v) { g_log[g_n++] = v; }
static void test_free(void* p) { Log(2); g_freedNode = p; }

struct CCountedPointerList {
    void* m_pNode;
    void clear() { Log(1); }
    ~CCountedPointerList()
    {
        clear();
        void* node = m_pNode;
        if (node != 0)
            test_free(node);
    }
};

struct COwner {
    long                m_key;
    CCountedPointerList m_list;
    static void* operator new(size_t size) { return std::malloc(size); }
    static void operator delete(void* p) { Log(3); std::free(p); }
};

int main()
{
    int node;
    COwner* owner = new COwner;
    owner->m_key = 7;
    owner->m_list.m_pNode = &node;
    g_n = 0;
    delete owner;
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || g_freedNode != &node) { std::printf("00a833b0_TEST FAIL\n"); return 1; }
    owner = new COwner;
    owner->m_list.m_pNode = 0;
    g_n = 0; g_freedNode = 0;
    delete owner;
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 3 || g_freedNode != 0) { std::printf("00a833b0_TEST FAIL\n"); return 1; }
    std::printf("00a833b0_TEST PASS\n");
    return 0;
}