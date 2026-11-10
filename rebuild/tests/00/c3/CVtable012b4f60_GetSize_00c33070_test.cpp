// Behaviour test for 0x00c33070 CVtable_012b4f60::GetSize: the size of the wrapped file: forwards to the file object's GetSize (+4, slot 0)
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
    virtual bool GetSize(unsigned long* size);
    CUnprovenFileObject_012b4f60* m_pFile;
    void* m_pOwner;
};

bool CVtable_012b4f60::GetSize(unsigned long* size)
{
    return m_pFile->GetSize(size);
}

int main()
{
    int ok = 1;
    CUnprovenFileObject_012b4f60 file;
    CVtable_012b4f60 h(&file);
    unsigned long n = 0; file.result = true; file.size = 1234;
    if (!h.GetSize(&n) || n != 1234 || file.last != 0) ok = 0;
    file.result = false; if (h.GetSize(&n)) ok = 0;
    if (!ok) { printf("00c33070_TEST FAIL\n"); return 1; }
    printf("00c33070_TEST PASS\n");
    return 0;
}