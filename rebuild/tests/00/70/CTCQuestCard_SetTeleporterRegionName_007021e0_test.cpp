// Behaviour test for CTCQuestCard::SetTeleporterRegionName 0x007021E0: the teleporter region name is assigned to the member string. The body forwards to the member at +0x24
// (`add ecx,24h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
#include <string>
struct CCharString { std::string Text; CCharString& operator=(const CCharString& o) { Text = o.Text; return *this; } };
struct CWideString { std::wstring Text; CWideString& operator=(const CWideString& o) { Text = o.Text; return *this; } };
#pragma pack(push, 4)
struct CTCQuestCard {                                  // retail is 32-bit: 4-byte packing keeps the +0x24 offset on a 64-bit host
    unsigned char Before[0x24];
    CCharString TeleporterRegionName;
    void SetTeleporterRegionName(const CCharString& name) { TeleporterRegionName = name; }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CTCQuestCard, TeleporterRegionName) != 0x24u) { std::printf("007021e0_TEST FAIL\n"); return 1; }
    CTCQuestCard o; CCharString v; v.Text = "BOWERSTONE";
    o.SetTeleporterRegionName(v);
    if (o.TeleporterRegionName.Text != v.Text) { std::printf("007021e0_TEST FAIL\n"); return 1; }
    std::printf("007021e0_TEST PASS\n");
    return 0;
}