#include <cstdio>
struct CTCDoor {
    bool CurrentOpen; bool Opening; bool Closing;
    bool IsClosed() const { return !Opening && !Closing && !CurrentOpen; }
};
int main()
{
    CTCDoor d; int closed = 0;
    for (int i = 0; i < 8; ++i) {
        d.CurrentOpen = (i & 1) != 0; d.Opening = (i & 2) != 0; d.Closing = (i & 4) != 0;
        if (d.IsClosed()) { ++closed; if (i != 0) { std::printf("0082cf40_TEST FAIL\n"); return 1; } }
    }
    if (closed != 1) { std::printf("0082cf40_TEST FAIL\n"); return 1; }
    std::printf("0082cf40_TEST PASS\n");
    return 0;
}