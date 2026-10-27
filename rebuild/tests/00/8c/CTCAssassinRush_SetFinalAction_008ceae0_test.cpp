#include <cstdio>
// Behaviour model: the member takes a reference; the by-value parameter's reference is dropped on return.
struct Info { long rc; };
int main()
{
    Info oldI = { 1 }, newI = { 2 };            // newI: one owner plus the by-value parameter copy
    Info* member = &oldI;
    if (member != &newI) { --member->rc; member = &newI; ++member->rc; }
    --newI.rc;                                   // the parameter's destructor
    if (oldI.rc != 0 || newI.rc != 2 || member != &newI) { std::printf("008ceae0_TEST FAIL\n"); return 1; }
    std::printf("008ceae0_TEST PASS\n");
    return 0;
}