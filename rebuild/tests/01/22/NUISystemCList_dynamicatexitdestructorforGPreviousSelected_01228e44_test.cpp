// Behaviour test for 0x01228E44 NUISystem::CList::`dynamic atexit destructor for 'GPreviousSelected'': the row's pragma, defines,
// include and definition verbatim. ~list is declared only there (the landed out-of-line copy 0x0053C9BC); this test supplies that
// body (STLport ~_List_base: clear, then free the sentinel) and checks that the compiler-registered atexit destructor runs it once,
// on the static, after main returns, with the static constructed empty (sentinel linked to itself) beforehand.
#include <stdio.h>
#pragma optimize("s", on)
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY
#include "engine/NUISystem_CList.h"

std::list<NUISystem::CList*> NUISystem::CList::GPreviousSelected;   // 0x013B8A98

static int g_ok = 0;
static int g_dtors = 0;
template <> std::list<NUISystem::CList*, std::allocator<NUISystem::CList*> >::~list()
{
    ++g_dtors;
    int right = (this == &NUISystem::CList::GPreviousSelected) && begin() == end();
    this->clear();
    this->_M_node.deallocate(this->_M_node._M_data, 1);
    if (g_ok && right && g_dtors == 1) printf("AUTO_TINY_01228e44_TEST PASS\n");
    else printf("AUTO_TINY_01228e44_TEST FAIL\n");
}

int main()
{
    std::list<NUISystem::CList*>& l = NUISystem::CList::GPreviousSelected;
    if (!(l.begin() == l.end()) || g_dtors != 0) { printf("AUTO_TINY_01228e44_TEST FAIL\n"); return 1; }
    g_ok = 1;
    return 0;   // the atexit destructor prints PASS
}
