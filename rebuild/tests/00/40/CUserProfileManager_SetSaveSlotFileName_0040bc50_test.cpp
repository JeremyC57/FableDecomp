// behaviour test for 0x0040BC50 CUserProfileManager::SetSaveSlotFileName: assigns SaveGameNames[slot] from the argument,
// then saves once (links the row's object)
#include <cstdio>
#include <cstring>
#include "fable_string.h"
class CUserProfileManager {
public:
    unsigned char _pad00[0xC0];
    CWideString*  SaveGameNames;
    void Save();
    void SetSaveSlotFileName(long slot, const CWideString& fileName);
};
static int g_saves = 0, g_assigns = 0;
static const void* g_lhs = 0;
static const void* g_rhs = 0;
void CUserProfileManager::Save() { ++g_saves; }
const CWideString& CWideString::operator=(const CWideString& other) { ++g_assigns; g_lhs = this; g_rhs = &other; return *this; }
int main()
{
    static unsigned char slots[4 * sizeof(void*)];
    static unsigned char name[sizeof(void*)];
    CUserProfileManager m;
    std::memset(&m, 0, sizeof(m));
    m.SaveGameNames = (CWideString*)slots;
    m.SetSaveSlotFileName(2, *(const CWideString*)name);
    if (g_assigns != 1 || g_lhs != slots + 2 * sizeof(void*) || g_rhs != name || g_saves != 1 || sizeof(m) != 0xC4) {
        std::printf("CEngine_0040bc50_TEST FAIL\n"); return 1;
    }
    std::printf("CEngine_0040bc50_TEST PASS\n");
    return 0;
}
