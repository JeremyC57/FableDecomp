// Behaviour test for CWideString's NullString atexit destructor (0x0122A550): drop the data block's owner count, free the
// text and the block on the last owner, clear the pointer; the instance count drops either way.
#include <stdio.h>
#include <stdlib.h>
struct Data { wchar_t* text; unsigned long a, b; long owners; };
static long instances; static int frees, deletes;
struct WS {
    Data* storage_;
    static long Dec(long* owners) { long count = --*owners; return count; }
    static void FreeData(Data* d) { if (d) { if (d->text) { free(d->text); ++frees; } delete d; ++deletes; } }
    void Destroy() { if (storage_) { if (Dec(&storage_->owners) <= 0) FreeData(storage_); storage_ = 0; } --instances; }
};
int main()
{
    WS s; Data* d = new Data(); d->owners = 2; d->text = (wchar_t*)malloc(4); s.storage_ = d; instances = 3;
    s.Destroy();
    if (s.storage_ != 0 || d->owners != 1 || frees || deletes || instances != 2) { printf("0122a550_TEST FAIL\n"); return 1; }
    s.storage_ = d; s.Destroy();
    if (s.storage_ != 0 || frees != 1 || deletes != 1 || instances != 1) { printf("0122a550_TEST FAIL\n"); return 1; }
    s.storage_ = 0; s.Destroy();
    if (instances != 0 || deletes != 1) { printf("0122a550_TEST FAIL\n"); return 1; }
    printf("0122a550_TEST PASS\n"); return 0;
}