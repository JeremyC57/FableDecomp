// Behaviour test for CThingManager::ClearPersistenceDataForSection 0x00520C70: the section is cleared in the entity runtime persistence. The body forwards to the member at +0xA0
// (`add ecx,A0h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
#include <string>
#include <map>
struct CCharString { std::string Text; };
struct CEntityRuntimePersistence { std::map<std::string,int> Data; void ClearPersistenceDataForSection(const CCharString& s) { Data.erase(s.Text); } };
#pragma pack(push, 4)
struct CThingManager {                                  // retail is 32-bit: 4-byte packing keeps the +0xA0 offset on a 64-bit host
    unsigned char Before[0xA0];
    CEntityRuntimePersistence EntityRuntimePersistence;
    void ClearPersistenceDataForSection(const CCharString& section) { EntityRuntimePersistence.ClearPersistenceDataForSection(section); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CThingManager, EntityRuntimePersistence) != 0xA0u) { std::printf("00520c70_TEST FAIL\n"); return 1; }
    CThingManager m; m.EntityRuntimePersistence.Data["A"] = 1; m.EntityRuntimePersistence.Data["B"] = 2;
    CCharString s; s.Text = "A";
    m.ClearPersistenceDataForSection(s);
    if (m.EntityRuntimePersistence.Data.size() != 1 || m.EntityRuntimePersistence.Data.count("B") != 1) { std::printf("004c7bc0_TEST FAIL\n"); return 1; }
    std::printf("00520c70_TEST PASS\n");
    return 0;
}