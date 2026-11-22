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
// NF239 (2026-11-22): the row now calls STLport's _List_base<CThing*> destructor (0x004AB490) from the shared header, so the test
//   stubs that out-of-line member (declared only under FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE) instead of a local std::list model.
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "stlport/fable_containers.h"
template <> std::_List_base<CThing*, std::allocator<CThing*> >::~_List_base() { g_list = ++g_order; g_listThis = this; }
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
