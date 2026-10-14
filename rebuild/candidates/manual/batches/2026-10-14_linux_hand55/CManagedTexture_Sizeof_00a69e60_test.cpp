#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CTexture { void* PD3DTexture; unsigned long ByteLength : 28; unsigned long AllocationSource : 4; };
struct CManagedTextureData { unsigned char _pad[0x20]; CTexture* Texture_Data; unsigned char _pad24[0x1c]; unsigned long AllocSize; };
struct CManagedTexture { void* __vftable; CManagedTextureData* m_pData;
    long Sizeof() { long size = 8; /* sizeof(CManagedTexture) in retail (32-bit) */ if (m_pData) size = m_pData->AllocSize + (m_pData->Texture_Data ? m_pData->Texture_Data->ByteLength + 0xc : 8 /* sizeof(CTexture) */) + 0x54; return size; } };

int main()
{
    CManagedTexture t; t.m_pData = 0; if (t.Sizeof() != 8) { std::printf("00a69e60_TEST FAIL\n"); return 1; } CManagedTextureData d; d.Texture_Data = 0; d.AllocSize = 100; t.m_pData = &d; if (t.Sizeof() != 100 + 8 + 0x54) { std::printf("00a69e60_TEST FAIL\n"); return 1; }
    CTexture tex; tex.ByteLength = 0x1234; tex.AllocationSource = 0xf; d.Texture_Data = &tex; if (t.Sizeof() != 100 + 0x1234 + 0xc + 0x54) { std::printf("00a69e60_TEST FAIL\n"); return 1; }
    std::printf("00a69e60_TEST PASS\n");
    return 0;
}
