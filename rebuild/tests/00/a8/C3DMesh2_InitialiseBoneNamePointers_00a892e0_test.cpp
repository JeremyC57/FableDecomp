#include <cstdio>
static unsigned short g_names[8];
class CHandleBasedMemoryPool { public: void* GetPointerFromHandle(long h) { return h == 77 ? (void*)g_names : 0; } };
class C3DMesh2 { public: void* m_vptr; CHandleBasedMemoryPool* MemoryPool; unsigned char _pad_0x08[0x7c]; long BoneNameMemoryHandle; unsigned char _pad_0x88[0x10]; unsigned long BoneCount; unsigned char _pad_0x9c[0x8]; unsigned short* BoneNameIndices; char* BoneNameData;
    void InitialiseBoneNamePointers(long has) { if (has) { unsigned short* i = (unsigned short*)MemoryPool->GetPointerFromHandle(BoneNameMemoryHandle); BoneNameIndices = i; BoneNameData = (char*)(i + BoneCount); } } };
int main()
{
    CHandleBasedMemoryPool pool; C3DMesh2 m; m.MemoryPool = &pool; m.BoneNameMemoryHandle = 77; m.BoneCount = 3; m.BoneNameIndices = 0; m.BoneNameData = 0;
    if ((char*)&m.BoneNameIndices != (char*)&m + 0xa4 || (char*)&m.BoneCount != (char*)&m + 0x98) { std::printf("00a892e0_TEST FAIL\n"); return 1; }
    m.InitialiseBoneNamePointers(0); if (m.BoneNameIndices != 0) { std::printf("00a892e0_TEST FAIL\n"); return 1; }
    m.InitialiseBoneNamePointers(1);
    if (m.BoneNameIndices != g_names || m.BoneNameData != (char*)(g_names + 3)) { std::printf("00a892e0_TEST FAIL\n"); return 1; }
    std::printf("00a892e0_TEST PASS\n"); return 0;
}