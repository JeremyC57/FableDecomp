// Behaviour test for 0x0066E7BD NHeroInformationScreens::CManager::GetCurrentScreen (NF217, 2026-11-21, q12373): rewritten on the shared
//   engine/NHeroInformationScreens_CManager.h (the July test modelled the free stand-in CManager_GetCurrentScreen over local structs). Links
//   the row object. The manager is a raw word buffer in the header's LAYOUT: RegisteredScreens' begin pointer +0x04, CurrentScreenIndex +0x28.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#include "engine/NHeroInformationScreens_CManager.h"

using NHeroInformationScreens::CBase;
using NHeroInformationScreens::CManager;

int main()
{
    static unsigned long screen_words[4];
    const CBase* slots[4];
    slots[0] = reinterpret_cast<const CBase*>(&screen_words[0]);
    slots[1] = reinterpret_cast<const CBase*>(&screen_words[1]);
    slots[2] = 0;
    slots[3] = reinterpret_cast<const CBase*>(&screen_words[3]);
    static unsigned long manager_words[0x54 / 4];
    const CBase** begin = slots;
    std::memcpy(reinterpret_cast<char*>(manager_words) + 0x04, &begin, sizeof(begin));
    CManager* manager = reinterpret_cast<CManager*>(manager_words);
    long current = 1;
    std::memcpy(reinterpret_cast<char*>(manager_words) + 0x28, &current, sizeof(current));
    /* current points at a non-empty slot: returns that slot, index unchanged */
    if (manager->GetCurrentScreen() != slots[1]) { std::printf("FAIL a\n"); return 1; }
    std::memcpy(&current, reinterpret_cast<char*>(manager_words) + 0x28, sizeof(current));
    if (current != 1) { std::printf("FAIL b\n"); return 1; }
    /* current points at an empty slot: resets to 0 and returns slot 0 */
    current = 2;
    std::memcpy(reinterpret_cast<char*>(manager_words) + 0x28, &current, sizeof(current));
    if (manager->GetCurrentScreen() != slots[0]) { std::printf("FAIL c\n"); return 1; }
    std::memcpy(&current, reinterpret_cast<char*>(manager_words) + 0x28, sizeof(current));
    if (current != 0) { std::printf("FAIL d\n"); return 1; }
    std::printf("CManager_0066e7bd_TEST PASS\n");
    return 0;
}
