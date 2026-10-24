#include <cstdio>
#include <cstdlib>
// Behaviour test for CEnginePrimitiveAttachmentManager's scalar deleting destructor @ 0x00B3ADC0: the list is cleared,
// its sentinel freed (a null sentinel is not), then ~CEngineComponent and ~CBase in that order; delete only when flag 1.
static char g_log[16];
static int g_n = 0;
static int g_freed = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }
static void FreeBlock(void* p) { ++g_freed; free(p); }
class CBase {
public:
    virtual ~CBase() { Log('B'); }
};
class CEngineComponent {
public:
    virtual ~CEngineComponent() { Log('E'); }
    void**        GlobalPointerAddress;
    bool          RenderEnabled;
    unsigned char KeepAlive;
};
struct SAttachedPrimitiveNode { SAttachedPrimitiveNode* Next; SAttachedPrimitiveNode* Prev; };
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) FreeBlock(__p); }
};
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
class CAttachedPrimitiveList {
public:
    _STLP_alloc_proxy<SAttachedPrimitiveNode*, SAttachedPrimitiveNode, allocator<SAttachedPrimitiveNode> > _M_node;
    void clear() { Log('C'); }                          // 0x00B3A760 stand-in
    ~CAttachedPrimitiveList()
    {
        clear();
        Log('F');
        _M_node.deallocate(_M_node._M_data, 1);
    }
};
class CEnginePrimitiveAttachmentManager : public CBase, public CEngineComponent {
public:
    CAttachedPrimitiveList AttachedPrimitives;
    virtual ~CEnginePrimitiveAttachmentManager() { }
};
int main()
{
    CEnginePrimitiveAttachmentManager* m = new CEnginePrimitiveAttachmentManager;
    m->AttachedPrimitives._M_node._M_data = (SAttachedPrimitiveNode*)malloc(sizeof(SAttachedPrimitiveNode));
    CBase* asBase = m;
    delete asBase;                                      // through the scalar deleting destructor
    if (g_freed != 1 || g_log[0] != 'C' || g_log[1] != 'F' || g_log[2] != 'E' || g_log[3] != 'B' || g_n != 4) { std::printf("00b3adc0_TEST FAIL\n"); return 1; }
    g_n = 0; g_freed = 0;
    {
        CEnginePrimitiveAttachmentManager local;        // flags 0 path: destroyed in place, sentinel null
        local.AttachedPrimitives._M_node._M_data = 0;
    }
    if (g_freed != 0 || g_n != 4 || g_log[2] != 'E' || g_log[3] != 'B') { std::printf("00b3adc0_TEST FAIL\n"); return 1; }
    std::printf("00b3adc0_TEST PASS\n");
    return 0;
}