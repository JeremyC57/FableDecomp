#include <stdio.h>
struct Foo;
Foo* makeShort(void* mem);
Foo* makeFull(void* mem);
int callD(Foo* p);
int main() {
    int buf[4], buf2[4];
    Foo* p = makeShort(buf); Foo* q = makeFull(buf2);
    void** vt = *(void***)p; void** vq = *(void***)q;
    printf("vt=%p same=%d slots: %p %p %p %p\n", vt, vt == vq, vt[0], vt[1], vt[2], vt[3]);
    fflush(stdout);
    printf("callD=%d\n", callD(p));
    return 0;
}
