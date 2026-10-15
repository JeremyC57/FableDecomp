// Behaviour test for CThing::SetAsGameStatePersisted @ 0x0078DDB0: writes bit 5 only, leaving the other bits.
#include <cstdio>
struct CThing {
    unsigned char b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char b6 : 1, b7 : 1;
    void SetAsGameStatePersisted(bool persisted) { GameStatePersisted = persisted; }
};
int main()
{
    CThing t; t.b0 = 1; t.b1 = 0; t.b2 = 1; t.b3 = 0; t.b4 = 1; t.GameStatePersisted = 0; t.b6 = 1; t.b7 = 1;
    t.SetAsGameStatePersisted(true);
    if (!t.GameStatePersisted || !t.b0 || t.b1 || !t.b2 || !t.b4 || !t.b6 || !t.b7) { std::printf("0078ddb0_TEST FAIL\n"); return 1; }
    t.SetAsGameStatePersisted(false);
    if (t.GameStatePersisted || !t.b7) { std::printf("0078ddb0_TEST FAIL\n"); return 1; }
    std::printf("0078ddb0_TEST PASS\n");
    return 0;
}
