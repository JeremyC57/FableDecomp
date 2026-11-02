// test: CFileInstaller::BeginDecompressionThread -- set the current thread to normal priority, then run the installer's decompression loop
#include <stdio.h>
static int lastPriority = -99, mainRuns = 0; static void* ranOn = 0;
static int SetThreadPriority(void*, int p) { lastPriority = p; return 1; }
static void* GetCurrentThread() { return (void*)-2; }
class CFileInstaller { public:
    int dummy;
    void DecompressionThreadMain() { ++mainRuns; ranOn = this; }
    static void BeginDecompressionThread(void* installer) {
        SetThreadPriority(GetCurrentThread(), 0);
        ((CFileInstaller*)installer)->DecompressionThreadMain();
    }
};
int main() {
    CFileInstaller fi;
    CFileInstaller::BeginDecompressionThread(&fi);
    if (lastPriority != 0 || mainRuns != 1 || ranOn != &fi) { printf("009c4420_TEST FAIL\n"); return 1; }
    printf("009c4420_TEST PASS\n");
    return 0;
}