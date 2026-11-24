// Behaviour test for CEngineLandscapeMemoryManager::DeleteIndexBuffer(CIndexBuffer*) @ 0x00BDA090 on the shared headers: links the row object.
// The manager is raw storage (its constructor is not linked); only the three index-buffer disposal lists are constructed in place.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER
#include <stdio.h>
#include "engine/CEngineLandscapeMemoryManager.h"

static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_resource_storage[4];

static unsigned long Count(std::list<CIndexBuffer*>& l)
{
    unsigned long n = 0;
    for (std::list<CIndexBuffer*>::iterator it = l.begin(); it != l.end(); ++it) ++n;
    return n;
}

int main()
{
    CEngineLandscapeMemoryManager* manager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    for (int i = 0; i < 3; ++i) new (&manager->IndexBufferDisposalList[i]) std::list<CIndexBuffer*>();
    CIndexBuffer* a = reinterpret_cast<CIndexBuffer*>(g_resource_storage);
    CIndexBuffer* b = reinterpret_cast<CIndexBuffer*>(g_resource_storage + 1);
    manager->CurrentDisposalListIndex = 1;
    manager->DeleteIndexBuffer(a);                                   // queued on the current list
    manager->DeleteIndexBuffer(0);                                   // null: nothing queued
    bool ok = Count(manager->IndexBufferDisposalList[1]) == 1 && *manager->IndexBufferDisposalList[1].begin() == a;
    ok = ok && Count(manager->IndexBufferDisposalList[0]) == 0 && Count(manager->IndexBufferDisposalList[2]) == 0;
    manager->DeleteIndexBuffer(b);                                   // appended at the back
    std::list<CIndexBuffer*>::iterator it = manager->IndexBufferDisposalList[1].begin();
    ok = ok && *it == a; ++it; ok = ok && *it == b; ++it; ok = ok && it == manager->IndexBufferDisposalList[1].end();
    manager->CurrentDisposalListIndex = 2;
    manager->DeleteIndexBuffer(b);                                   // the index selects the list
    ok = ok && Count(manager->IndexBufferDisposalList[2]) == 1 && Count(manager->IndexBufferDisposalList[1]) == 2;
    printf(ok ? "00bda090_TEST PASS\n" : "00bda090_TEST FAIL\n");
    return ok ? 0 : 1;
}
