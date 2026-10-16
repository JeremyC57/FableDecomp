#include <cstdio>
// C3DAnimation2(): every pointer/count zero, MemoryHandle -1, an empty circular instance list.
struct Node { Node* next; Node* prev; };
struct Anim { void* MemoryPool; long MemoryHandle; float SamplesPerSecond; unsigned int FrameCount, SequenceCount; bool Locked; Node* list;
    Anim() : MemoryPool(0), MemoryHandle(-1), SequenceCount(0), Locked(false) { Node* n = new Node; n->next = n; n->prev = n; list = n; FrameCount = 0; SamplesPerSecond = 0; } };
int main() {
    Anim a;
    if (a.MemoryHandle != -1 || a.MemoryPool || a.FrameCount || a.SamplesPerSecond != 0.0f || a.list->next != a.list || a.list->prev != a.list || a.Locked) { std::printf("00a4ee10_TEST FAIL\n"); return 1; }
    delete a.list;
    std::printf("00a4ee10_TEST PASS\n"); return 0; }
