// Behaviour test for CDrawNPCName::SetName 0x00657230: the NPC name is assigned to the member wide string. The body forwards to the member at +0x28
// (`add ecx,28h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
#include <string>
struct CCharString { std::string Text; CCharString& operator=(const CCharString& o) { Text = o.Text; return *this; } };
struct CWideString { std::wstring Text; CWideString& operator=(const CWideString& o) { Text = o.Text; return *this; } };
#pragma pack(push, 4)
struct CDrawNPCName {                                  // retail is 32-bit: 4-byte packing keeps the +0x28 offset on a 64-bit host
    unsigned char Before[0x28];
    CWideString Name;
    void SetName(const CWideString& name) { Name = name; }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CDrawNPCName, Name) != 0x28u) { std::printf("00657230_TEST FAIL\n"); return 1; }
    CDrawNPCName o; CWideString v; v.Text = L"Guard";
    o.SetName(v);
    if (o.Name.Text != v.Text) { std::printf("00657230_TEST FAIL\n"); return 1; }
    std::printf("00657230_TEST PASS\n");
    return 0;
}