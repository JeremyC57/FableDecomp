// Behaviour test for 0x00c33080 CVtable_012b4f60::Read: queue a read on the wrapped file: forwards all six arguments to the file object's Read (+4, slot 1)
// Self-contained: a recording file object stands in for the wrapped outside file.
#include <stdio.h>

class CUnprovenFileObject_012b4f60 {
public:
    int last; bool result; unsigned long size, count, unit; long unused; void *a, *b, *c, *d;
    CUnprovenFileObject_012b4f60() : last(-1), result(false), size(0), count(0), unit(0), unused(0), a(0), b(0), c(0), d(0) { }
    virtual bool GetSize(unsigned long* s) { last = 0; *s = size; return result; }
    virtual bool Read(unsigned long n, long u, void* pa, void* pb, void* pc, void* pd) { last = 1; count = n; unused = u; a = pa; b = pb; c = pc; d = pd; return result; }
    virtual unsigned long GetUnitSize() { last = 2; return unit; }
    virtual void Close() { last = 3; }
    virtual ~CUnprovenFileObject_012b4f60() { }
};

class CVtable_012b4f60 {
public:
    explicit CVtable_012b4f60(CUnprovenFileObject_012b4f60* file) : m_pFile(file), m_pOwner(0) { }
    virtual ~CVtable_012b4f60() { }
    virtual bool Read(unsigned long count, long unused, void* a, void* b, void* c, void* d);
    CUnprovenFileObject_012b4f60* m_pFile;
    void* m_pOwner;
};

bool CVtable_012b4f60::Read(unsigned long count, long unused, void* a, void* b, void* c, void* d)
{
    return m_pFile->Read(count, unused, a, b, c, d);
}

int main()
{
    int ok = 1;
    CUnprovenFileObject_012b4f60 file;
    CVtable_012b4f60 h(&file);
    int x, y, z, w; file.result = true;
    if (!h.Read(16, 7, &x, &y, &z, &w) || file.last != 1 || file.count != 16 || file.unused != 7 || file.a != &x || file.d != &w) ok = 0;
    file.result = false; if (h.Read(1, 0, 0, 0, 0, 0)) ok = 0;
    if (!ok) { printf("00c33080_TEST FAIL\n"); return 1; }
    printf("00c33080_TEST PASS\n");
    return 0;
}