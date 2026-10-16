#include <cstdio>
struct CBeam { long state; void* player; void Deactivate(void* p) { state = 5; player = p; } };
struct Node { Node* next; CBeam* beam; };
struct CTCWillLightning {
    float time, timeBase; Node head; long state; int player;
    void Deactivate() {
        if (state == 3 || state == 2 || state == 1) {
            timeBase = time; state = 5;
            for (Node* it = head.next; it != &head; it = it->next) it->beam->Deactivate(&player);
        }
    }
};
int main()
{
    CBeam b1 = { 2, 0 }, b2 = { 2, 0 };
    CTCWillLightning w; w.time = 4.0f; w.timeBase = 1.0f; w.state = 2; w.player = 0;
    Node n1, n2; w.head.next = &n1; n1.next = &n2; n2.next = &w.head; n1.beam = &b1; n2.beam = &b2;
    w.Deactivate();
    if (w.timeBase != 4.0f || w.state != 5 || b1.state != 5 || b2.player != &w.player) { std::printf("007a0310_TEST FAIL\n"); return 1; }
    w.state = 4; w.time = 9.0f; w.Deactivate();
    if (w.timeBase != 4.0f || w.state != 4) { std::printf("007a0310_TEST FAIL\n"); return 1; }
    std::printf("007a0310_TEST PASS\n");
    return 0;
}
