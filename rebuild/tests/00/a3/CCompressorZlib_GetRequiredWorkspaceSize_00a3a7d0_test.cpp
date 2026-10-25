#include <stdio.h>
// Behaviour test for 0x00A3A7D0 CCompressorZlib::GetRequiredWorkspaceSize: links against the source object.
class CCompressorZlib {
public:
    static unsigned long __fastcall GetRequiredWorkspaceSize();
};
int main()
{
    if (CCompressorZlib::GetRequiredWorkspaceSize() != 0) { printf("00a3a7d0_TEST FAIL\n"); return 1; }
    printf("00a3a7d0_TEST PASS\n");
    return 0;
}
