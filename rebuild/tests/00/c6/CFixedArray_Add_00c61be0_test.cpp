// behaviour test for 0x00C61BE0
#include <stdio.h>
class CFixedArray {
public:
    void* _vptr_pad;                                     // +0x00
    long* Data;                                          // +0x04
    long  Count;                                         // +0x08
    void Add(long value);
};
void CFixedArray::Add(long value)
{
    Data[Count] = value;
    ++Count;
}
int main()
{
    long buf[4] = {0}; CFixedArray a; a.Data = buf; a.Count = 0; int ok = 1;
    a.Add(5); a.Add(-2);
    ok &= a.Count == 2 && buf[0] == 5 && buf[1] == -2 && buf[2] == 0;
    printf(ok ? "00c61be0_TEST PASS\n" : "00c61be0_TEST FAIL\n");
    return ok ? 0 : 1;
}