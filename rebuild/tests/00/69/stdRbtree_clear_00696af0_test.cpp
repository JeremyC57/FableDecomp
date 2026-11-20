// Behaviour test for 0x00696AF0 std::_Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >, ...>::clear
// on the shared headers: the row's defines, includes and explicit instantiation verbatim. _M_erase (out of line in retail, the landed
// 0x00696900) is a test specialization that records the node it is given. A tree image with a count of 3 must erase from the root
// (the header's parent), relink the header's left / right to itself, null its parent and zero the count; an empty tree must not call
// _M_erase. A mutant that skips the count test, erases from the wrong node or leaves a link fails.
#include <stdio.h>
#include <string.h>
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
namespace NCreatureAction { class CActionEventBase; }

typedef std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > TValue;
typedef std::_Rb_tree<CCharString, TValue, std::_Select1st<TValue>, std::less<CCharString>, std::allocator<TValue> > TTree;
static void* g_erased = 0;
static int g_calls = 0;
template<> void TTree::_M_erase(std::_Rb_tree_node<TValue>* x) { g_erased = x; ++g_calls; }

template void std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > > >::clear();

int main()
{
    void* header[8]; void* root[8];           // node words: colour +0, parent +4, left +8, right +0xC
    memset(header, 0, sizeof(header)); memset(root, 0, sizeof(root));
    header[1] = root; header[2] = (void*)0x1111; header[3] = (void*)0x2222;
    void* image[4] = { header, (void*)3, 0, 0 };   // _M_header._M_data +0, _M_node_count +4
    TTree* tree = (TTree*)image;
    tree->clear();
    int ok = g_calls == 1 && g_erased == (void*)root && header[1] == 0 && header[2] == (void*)header && header[3] == (void*)header
          && image[1] == 0 && image[0] == (void*)header;
    tree->clear();                             // empty: no erase
    ok = ok && g_calls == 1;
    printf(ok ? "CTCCARRYABLE_ONKILL_00696AF0_TEST PASS\n" : "00696AF0_TEST FAIL\n");
    return ok ? 0 : 1;
}
