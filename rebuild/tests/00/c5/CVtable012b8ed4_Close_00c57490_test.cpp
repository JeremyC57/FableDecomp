// Behaviour test for CVtable_012b8ed4::Close (0x00C57490): closing the stream runs the (virtual) ReleaseBuffer once.
#include <stdio.h>
struct CVtable_012b8ed4 {
    int released;
    CVtable_012b8ed4() : released(0) { }
    virtual ~CVtable_012b8ed4() { }
    virtual void Close();
    virtual void ReleaseBuffer() { ++released; }
};
void CVtable_012b8ed4::Close()
{
    ReleaseBuffer();
}
struct Derived : CVtable_012b8ed4 {
    int derivedReleased;
    Derived() : derivedReleased(0) { }
    virtual void ReleaseBuffer() { ++derivedReleased; }
};
int main(void)
{
    CVtable_012b8ed4 b; b.Close();
    Derived d; d.Close(); d.Close();
    if (b.released != 1 || d.derivedReleased != 2 || d.released != 0) { printf("00c57490_TEST FAIL\n"); return 1; }
    printf("00c57490_TEST PASS\n");
    return 0;
}