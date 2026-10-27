#include <cstdio>
// Behaviour model: the message is posted only when both things exist and are not dead.
struct T { int dead; };
static int posted;
static void perform(T* creature, T* target)
{
    if (creature && !(creature->dead & 1) && target && !(target->dead & 1)) ++posted;
}
int main()
{
    T a = { 0 }, b = { 0 }, c = { 1 };
    perform(&a, &b); perform(&a, &c); perform(0, &b); perform(&c, &b);
    if (posted != 1) { std::printf("008442f0_TEST FAIL\n"); return 1; }
    std::printf("008442f0_TEST PASS\n");
    return 0;
}