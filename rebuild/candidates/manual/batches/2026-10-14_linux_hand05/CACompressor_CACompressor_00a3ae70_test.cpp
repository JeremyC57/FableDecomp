#include <cstdio>
struct CBase { virtual void v00() {} CBase() {} };
struct CRetailByteVector { unsigned char* first; unsigned char* last; unsigned char* end; CRetailByteVector() : first(0), last(0), end(0) { } };
struct CACompressor : CBase { void* Workspace; unsigned long WorkspaceSize; bool CompressionOn; CRetailByteVector OwnWorkspace; virtual void v00() {} CACompressor(void* workspace, unsigned long workspaceSize); };
CACompressor::CACompressor(void* workspace, unsigned long workspaceSize) : CBase(), OwnWorkspace() { Workspace = workspace; WorkspaceSize = workspaceSize; }
int main()
{
    int buf[4];
    CACompressor c(buf, 16);
    if (c.Workspace != buf || c.WorkspaceSize != 16 || c.OwnWorkspace.first != 0 || c.OwnWorkspace.last != 0 || c.OwnWorkspace.end != 0) { std::printf("00a3ae70_TEST FAIL\n"); return 1; }
    std::printf("00a3ae70_TEST PASS\n");
    return 0;
}
