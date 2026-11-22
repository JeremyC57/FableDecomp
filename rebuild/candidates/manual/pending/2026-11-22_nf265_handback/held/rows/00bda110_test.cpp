// Behaviour test for CEngineLandscapeMemoryManager::DeleteTexture(CTexture*) @ 0x00BDA110 on the shared headers: links the row object.
// The manager is raw storage (its constructor is not linked); only the three texture disposal lists are constructed in place.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETETEXTURE
#include <stdio.h>
#include "engine/CEngineLandscapeMemoryManager.h"

static double g_manager_storage[sizeof(CEngineLandscapeMemoryManager) / sizeof(double) + 1];
static double g_texture_storage[4];

static unsigned long Count(std::list<CTexture*>& l)
{
    unsigned long n = 0;
    for (std::list<CTexture*>::iterator it = l.begin(); it != l.end(); ++it) ++n;
    return n;
}

int main()
{
    CEngineLandscapeMemoryManager* manager = reinterpret_cast<CEngineLandscapeMemoryManager*>(g_manager_storage);
    for (int i = 0; i < 3; ++i) new (&manager->TextureDisposalList[i]) std::list<CTexture*>();
    CTexture* a = reinterpret_cast<CTexture*>(g_texture_storage);
    CTexture* b = reinterpret_cast<CTexture*>(g_texture_storage + 1);
    manager->CurrentDisposalListIndex = 1;
    manager->DeleteTexture(a);                                   // queued on the current list
    manager->DeleteTexture(0);                                   // null: nothing queued
    bool ok = Count(manager->TextureDisposalList[1]) == 1 && *manager->TextureDisposalList[1].begin() == a;
    ok = ok && Count(manager->TextureDisposalList[0]) == 0 && Count(manager->TextureDisposalList[2]) == 0;
    manager->DeleteTexture(b);                                   // appended at the back
    std::list<CTexture*>::iterator it = manager->TextureDisposalList[1].begin();
    ok = ok && *it == a; ++it; ok = ok && *it == b; ++it; ok = ok && it == manager->TextureDisposalList[1].end();
    manager->CurrentDisposalListIndex = 2;
    manager->DeleteTexture(b);                                   // the index selects the list
    ok = ok && Count(manager->TextureDisposalList[2]) == 1 && Count(manager->TextureDisposalList[1]) == 2;
    printf(ok ? "00bda110_TEST PASS\n" : "00bda110_TEST FAIL\n");
    return ok ? 0 : 1;
}
