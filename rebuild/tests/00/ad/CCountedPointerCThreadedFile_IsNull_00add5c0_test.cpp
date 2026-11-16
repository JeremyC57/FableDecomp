// Test: CCountedPointer<CThreadedFile>::IsNull -- true exactly when Data is zero (the count block is not read).
#include <cstdio>
struct CThreadedFile { int x; };
struct CCPPointerInfo { unsigned long RefCount; void* DeleteFunc; void* Data; };
template <class T> struct CCountedPointer { T* Data; CCPPointerInfo* Info; bool IsNull() const { return Data == 0; } };
int main()
{
    CThreadedFile file; CCPPointerInfo info = { 1, 0, &file };
    CCountedPointer<CThreadedFile> p; p.Data = 0; p.Info = &info;
    bool ok = sizeof(p) == 8 && p.IsNull();
    p.Data = &file; ok = ok && !p.IsNull();
    p.Info = 0; ok = ok && !p.IsNull();
    std::printf(ok ? "00add5c0_TEST PASS\n" : "00add5c0_TEST FAIL\n");
    return ok ? 0 : 1;
}