#include <cstdio>

struct CCountedPointer { void* p; };

struct SubList {
    int calls;
    CCountedPointer* last;
    void Insert(CCountedPointer* arg) { calls++; last = arg; }
};

struct CTCAnimation {
    SubList a;
    SubList b;
    void RemoveCombinationAnimsOfType(CCountedPointer* arg)
    {
        this->a.Insert(arg);
        this->b.Insert(arg);
    }
};

int main()
{
    CTCAnimation list;
    list.a.calls = 0; list.a.last = 0;
    list.b.calls = 0; list.b.last = 0;
    CCountedPointer cp; cp.p = (void*)0x1234;
    list.RemoveCombinationAnimsOfType(&cp);
    if (list.a.calls == 1 && list.b.calls == 1 &&
        list.a.last == &cp && list.b.last == &cp) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}