// Behaviour test for CEngineLandscapeMemoryManager::DeleteVertexBuffer(CVertexBufferWin32*) @ 0x00BDA0D0 on the shared headers: links the row object.
// The manager is raw storage (its constructor is not linked); only the three vertex-buffer disposal lists are constructed in place.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER
#include <stdio.h>
#include "engine/CEngineLandscapeMemoryManager.h"

static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_resource_storage[4];

static unsigned long Count(std::list<CVertexBufferWin32*>& l)
{
    unsigned long n = 0;
    for (std::list<CVertexBufferWin32*>::iterator it = l.begin(); it != l.end(); ++it) ++n;
    return n;
}

int main()
{
    CEngineLandscapeMemoryManager* manager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    for (int i = 0; i < 3; ++i) new (&manager->VertexBufferDisposalList[i]) std::list<CVertexBufferWin32*>();
    CVertexBufferWin32* a = reinterpret_cast<CVertexBufferWin32*>(g_resource_storage);
    CVertexBufferWin32* b = reinterpret_cast<CVertexBufferWin32*>(g_resource_storage + 1);
    manager->CurrentDisposalListIndex = 1;
    manager->DeleteVertexBuffer(a);                                   // queued on the current list
    manager->DeleteVertexBuffer(0);                                   // null: nothing queued
    bool ok = Count(manager->VertexBufferDisposalList[1]) == 1 && *manager->VertexBufferDisposalList[1].begin() == a;
    ok = ok && Count(manager->VertexBufferDisposalList[0]) == 0 && Count(manager->VertexBufferDisposalList[2]) == 0;
    manager->DeleteVertexBuffer(b);                                   // appended at the back
    std::list<CVertexBufferWin32*>::iterator it = manager->VertexBufferDisposalList[1].begin();
    ok = ok && *it == a; ++it; ok = ok && *it == b; ++it; ok = ok && it == manager->VertexBufferDisposalList[1].end();
    manager->CurrentDisposalListIndex = 2;
    manager->DeleteVertexBuffer(b);                                   // the index selects the list
    ok = ok && Count(manager->VertexBufferDisposalList[2]) == 1 && Count(manager->VertexBufferDisposalList[1]) == 2;
    printf(ok ? "00bda0d0_TEST PASS\n" : "00bda0d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
