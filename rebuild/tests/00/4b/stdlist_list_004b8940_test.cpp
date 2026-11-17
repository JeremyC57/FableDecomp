// Behaviour test for 0x004B8940 std::list<CIntelligentPointer<CThing> >::~list (NF72 round 7, q10902): the destructor calls the
// out-of-line _List_base::clear (0x004B8050) on this, then frees the header node behind a null test.
#include <cstdio>
class CThing;
template <class T> class CIntelligentPointer;
namespace std {
template <class T> class allocator;
template <class T, class A> class _List_base { public: void clear(); };
template <class T, class A> class list { public: void* _M_node; ~list(); };
}
typedef CIntelligentPointer<CThing> TPointer;
typedef std::list<TPointer, std::allocator<TPointer> > TList;
static int g_clears = 0;
static void* g_clearThis = 0;
template <> void std::_List_base<TPointer, std::allocator<TPointer> >::clear() { ++g_clears; g_clearThis = this; }
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
static void* g_freed = (void*)-1;
static int g_frees = 0;
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return;
    g_freed = p; ++g_frees;
}
int main()
{
    g_free_hook = 1;
    static unsigned char raw[sizeof(TList)];
    TList* l = (TList*)raw;
    int header = 7;
    l->_M_node = &header;
    l->~TList();
    bool ok1 = g_clears == 1 && g_clearThis == l && g_frees == 1 && g_freed == &header;
    l->_M_node = 0; g_freed = (void*)-1;
    l->~TList();
    bool ok2 = g_clears == 2 && g_frees == 1 && g_freed == (void*)-1;
    if (ok1 && ok2) std::printf("004b8940_TEST PASS\n");
    else std::printf("FAIL c=%d f=%d\n", g_clears, g_frees);
    return 0;
}
