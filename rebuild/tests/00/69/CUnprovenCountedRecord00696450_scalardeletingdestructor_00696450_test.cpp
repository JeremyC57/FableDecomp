// behaviour test for 0x00696450 CUnprovenCountedRecord_00696450::`scalar deleting destructor': the counted pointer is released (the pointee deleter and the count block free run only when the count reaches zero), both words are cleared, the record is freed only when bit 0 of the flags is set, and this is returned
#include <cstdio>
#include <cstdlib>
static int gDeleted = 0, gInfoFreed = 0, gRecordFreed = 0;
struct Info { int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeletePointee(void* p) { ++gDeleted; std::free(p); }
struct Record {
    unsigned int Head[1]; void* Data; Info* PInfo;
    Record* DeletingDtor(unsigned int flags)            // the function under test
    {
        if (PInfo) {
            --PInfo->RefCount;
            if (PInfo->RefCount == 0) { PInfo->DeleteFunc(PInfo->Data); ++gInfoFreed; std::free(PInfo); }
        }
        Data = 0; PInfo = 0;
        if (flags & 1) { ++gRecordFreed; std::free(this); }
        return this;
    }
};
int main()
{
    int fail = 0;
    Info* shared = (Info*)std::malloc(sizeof(Info));
    shared->RefCount = 2; shared->DeleteFunc = DeletePointee; shared->Data = std::malloc(4);
    Record a; a.Data = shared->Data; a.PInfo = shared;
    if (a.DeletingDtor(0) != &a || a.Data != 0 || a.PInfo != 0) fail |= 1;
    if (shared->RefCount != 1 || gDeleted != 0 || gInfoFreed != 0 || gRecordFreed != 0) fail |= 2;
    Record* b = (Record*)std::malloc(sizeof(Record)); b->Data = shared->Data; b->PInfo = shared;
    b->DeletingDtor(1);
    if (gDeleted != 1 || gInfoFreed != 1 || gRecordFreed != 1) fail |= 4;
    Record c; c.Data = 0; c.PInfo = 0;
    c.DeletingDtor(0);
    if (gDeleted != 1 || gInfoFreed != 1) fail |= 8;
    if (fail) { std::printf("00696450_TEST FAIL %d\n", fail); return 1; }
    std::printf("00696450_TEST PASS\n");
    return 0;
}