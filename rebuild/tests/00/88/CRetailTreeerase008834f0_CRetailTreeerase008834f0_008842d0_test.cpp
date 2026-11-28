#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_BYPASSCOLORFILTER
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_SCREENFILTERENTRY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_INSERT_FILTER_PASS30
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST_SIZE
#define FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
#define FABLE_RETAIL_STLP_TREE_MCOPY_BODY
#define FABLE_RETAIL_STLP_MAP_COPY_CTOR_BODY
#define FABLE_RETAIL_STLP_TREE_COPY_CTOR_BODY
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_SET_FIND
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
#include "engine/ThingAlive.h"
#include "engine/CMainGameComponent.h"

#include <cstdio>
#include <cstdlib>

typedef NScreenEffect::CScreenEffectPriorityStack Stack;
typedef Stack::CScreenFilterEntry Entry;
typedef Entry::CThingGraphicByPassFlagState State;
typedef std::map<CThing*,State> Map;
static int livePointers;
static void check(bool good) { if (!good) std::abort(); }

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { ++livePointers; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --livePointers; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return reinterpret_cast<CBaseObject*>(PData); }
void CBaseIntelligentPointer::SetPItem(CBaseObject* p) { PData = reinterpret_cast<CBaseObjectPointer*>(p); }

namespace std {
// A one-node or two-node tree has no red parent on insertion.
template<> void __cdecl _Rb_global<bool>::_Rebalance(_Rb_tree_node_base* node,_Rb_tree_node_base*& root) {
    check(node == root || node->_M_parent->_M_color);
    node->_M_color = false;
    root->_M_color = true;
}
}
int main() {
    unsigned int storage[2] = {0,0};
    CThing* keys[2] = { reinterpret_cast<CThing*>(storage), reinterpret_cast<CThing*>(storage + 1) };
    for (int reverse = 0; reverse != 2; ++reverse) {
        for (int count = 0; count != 3; ++count) {
            {
                Entry entry(15);
                check(!entry.reset.size());
                Map& src = entry.reset;
                for (int i = 0; i != count; ++i) {
                    CThing* k = keys[reverse ^ i];
                    State& state = src[k];
                    check(!state.thing.Get() && !state.bypassReset && src.size() == (unsigned int)i+1);
                    state.thing.SetPItem(k);
                    state.bypassReset = true;
                    check(&state == &src[k] && src[k].thing.Get() == k && src[k].bypassReset);
                }
                check(livePointers == count);
                Map destination;
                destination[keys[0]].bypassReset = false;
                destination = src;
                check(destination.size() == src.size() && livePointers == count*2);
                destination = destination;
                check(destination.size() == src.size());
                for (int j = 0; j != count; ++j) {
                    CThing* k = keys[reverse ^ j];
                    check(destination[k].bypassReset && destination[k].thing.Get() == k);
                    destination[k].bypassReset = false;
                    check(src[k].bypassReset);
                }
                destination.clear();
                check(!destination.size() && livePointers == count);
                destination.clear();
                check(livePointers == count);
            }
            check(!livePointers);
        }
    }
    std::puts("008842d0_TEST PASS");
    return 0;
}
