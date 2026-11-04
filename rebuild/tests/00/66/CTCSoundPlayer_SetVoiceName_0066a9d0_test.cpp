// Behaviour test for CTCSoundPlayer::SetVoiceName 0x0066A9D0: the voice name is assigned to the member string. The body forwards to the member at +0x14
// (`add ecx,14h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
#include <string>
struct CCharString { std::string Text; CCharString& operator=(const CCharString& o) { Text = o.Text; return *this; } };
struct CWideString { std::wstring Text; CWideString& operator=(const CWideString& o) { Text = o.Text; return *this; } };
#pragma pack(push, 4)
struct CTCSoundPlayer {                                  // retail is 32-bit: 4-byte packing keeps the +0x14 offset on a 64-bit host
    unsigned char Before[0x14];
    CCharString VoiceName;
    void SetVoiceName(const CCharString& name) { VoiceName = name; }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CTCSoundPlayer, VoiceName) != 0x14u) { std::printf("0066a9d0_TEST FAIL\n"); return 1; }
    CTCSoundPlayer o; CCharString v; v.Text = "VOICE_GUARD";
    o.SetVoiceName(v);
    if (o.VoiceName.Text != v.Text) { std::printf("0066a9d0_TEST FAIL\n"); return 1; }
    std::printf("0066a9d0_TEST PASS\n");
    return 0;
}