// Behaviour test for 0x00656E0C deque<pair<CCharString,long>>::push_back (model copy)
#include <cstdio>
#include <new>
struct Elem { int text; long time; };
struct Iter { Elem* cur; Elem* first; Elem* last; Elem** node; };
struct Deque {
    Iter start; Iter finish; int aux;
    void push_back_aux(const Elem&) { ++aux; }
    void push_back(const Elem& t)
    {
        if (finish.cur != finish.last - 1) { if (finish.cur) new (finish.cur) Elem(t); ++finish.cur; }
        else push_back_aux(t);
    }
};
int main()
{
    Elem block[16];
    Deque d; d.aux = 0; d.finish.first = d.finish.cur = block; d.finish.last = block + 16;
    Elem e; e.text = 3; e.time = 44;
    for (int i = 0; i < 15; ++i) d.push_back(e);
    if (d.aux != 0 || d.finish.cur != block + 15 || block[14].time != 44) { std::printf("00656e0c_TEST FAIL\n"); return 1; }
    d.push_back(e);                       // last slot of the block: aux path
    if (d.aux != 1 || d.finish.cur != block + 15) { std::printf("00656e0c_TEST FAIL\n"); return 1; }
    std::printf("00656e0c_TEST PASS\n");
    return 0;
}