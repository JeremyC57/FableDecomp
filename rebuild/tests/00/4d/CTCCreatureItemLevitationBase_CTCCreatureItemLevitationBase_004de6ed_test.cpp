#include <cstdio>
// Behaviour test for CTCCreatureItemLevitationBase::~CTCCreatureItemLevitationBase (0x004DE6ED): links the row object (its DestroyOne
// emission helper runs the implicit destructor) and checks it destroys the LevitatingItems list at +0x0C, then the CTCBase base.
class CThing;
static int g_list = 0, g_base = 0, g_order = 0;
static const void* g_listThis = 0;
static const void* g_baseThis = 0;
class CTCBase {
public:
    virtual ~CTCBase();
};
CTCBase::~CTCBase() { g_base = ++g_order; g_baseThis = this; }
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;
    ~list();
};
template <> list<CThing*>::~list() { g_list = ++g_order; g_listThis = this; }
}
class CTCCreatureItemLevitationBase;
void DestroyOne(CTCCreatureItemLevitationBase* object);
int main()
{
    void* buf[8] = { 0 };
    DestroyOne((CTCCreatureItemLevitationBase*)buf);
    if (g_list != 1 || g_base != 2) { std::printf("BAD_ORDER\n"); return 1; }
    if (g_listThis != (char*)buf + 0xc || g_baseThis != (void*)buf) { std::printf("BAD_THIS\n"); return 1; }
    std::printf("DESTVAL_004de6ed_OK\n");
    return 0;
}
