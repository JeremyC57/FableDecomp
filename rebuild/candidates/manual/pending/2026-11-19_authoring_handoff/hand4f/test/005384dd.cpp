// Behaviour test for 0x005384DD NUISystem::CList::AddActionOnSelected: the row's pragmas, defines, includes and body verbatim, run non-virtually on
// a zeroed CList image (the list pointer at +0x1B0, the other action list at +0x1AC). The first call must create the list and
// append a copy of the counted pointer (reference count 1 -> 2); the second must reuse the same list (two nodes, count 3); an existing list
// must be kept; the other action list must stay untouched.
#include <stdio.h>
#include <string.h>
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE
#include "engine/NUISystem_CList.h"

void NUISystem::CList::AddActionOnSelected(CCountedPointer<NUISystem::CAction>& action)
{
    if (!PActionsOnSelected)
        PActionsOnSelected = new std::list<CCountedPointer<NUISystem::CAction> >;
    PActionsOnSelected->push_back(action);
}

struct RawNode { RawNode* next; RawNode* prev; void* data; CCPPointerInfo* info; };
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static double image[0x400 / 8];
int main()
{
    memset(image, 0, sizeof(image));
    NUISystem::CList* list = (NUISystem::CList*)image;
    char* base = (char*)image;
    static int payload = 42;
    CCPPointerInfo info(1, 0, &payload);
    void* raw[2] = { &payload, &info };
    CCountedPointer<NUISystem::CAction>& action = *(CCountedPointer<NUISystem::CAction>*)raw;
    list->NUISystem::CList::AddActionOnSelected(action);
    RawNode** slot = (RawNode**)(base + 0x1B0);
    RawNode* first = *slot;
    check(first != 0, "list created");
    check(*(void**)(base + 0x1AC) == 0, "other action list untouched");
    RawNode* sentinel = first ? *(RawNode**)first : 0;
    check(sentinel && sentinel->next != sentinel && sentinel->next->next == sentinel, "one node");
    check(sentinel && sentinel->next->data == &payload && sentinel->next->info == &info, "node holds the action");
    check(info.RefCount == 2, "reference taken");
    list->NUISystem::CList::AddActionOnSelected(action);
    check(*slot == first, "same list reused");
    check(sentinel && sentinel->next->next != sentinel && sentinel->next->next->next == sentinel && sentinel->prev == sentinel->next->next, "two nodes in order");
    check(info.RefCount == 3, "second reference");
    static RawNode other_sentinel;
    other_sentinel.next = other_sentinel.prev = &other_sentinel;
    static RawNode* existing = &other_sentinel;
    *slot = (RawNode*)&existing;
    list->NUISystem::CList::AddActionOnSelected(action);
    check(*slot == (RawNode*)&existing && other_sentinel.next != &other_sentinel, "existing list kept and appended to");
    if (!g_fail) printf("005384dd_TEST PASS\n");
    return g_fail;
}
