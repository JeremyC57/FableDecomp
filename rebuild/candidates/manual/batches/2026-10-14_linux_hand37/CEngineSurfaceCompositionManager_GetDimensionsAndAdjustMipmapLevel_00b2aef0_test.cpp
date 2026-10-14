#include <cstdio>
#include <cstdlib>
#include <cstring>
struct STextureRecord_00b2aef0 { unsigned char _pad[6]; unsigned short m_width; unsigned short m_height; };
struct CTextureEntry_00b2aef0 { unsigned char _pad[0xc]; STextureRecord_00b2aef0* m_pRecord; };
struct CEngineSurfaceCompositionManager { unsigned char _pad[0x10c8]; long m_maxDimension;
    void GetDimensionsAndAdjustMipmapLevel(long* dims, CTextureEntry_00b2aef0* t, unsigned long* level) { dims[0] = t->m_pRecord->m_width >> *level; dims[1] = t->m_pRecord->m_height >> *level; while (dims[0] > m_maxDimension || dims[1] > m_maxDimension) { dims[0] /= 2; dims[1] /= 2; ++*level; } } };
int main() {
    CEngineSurfaceCompositionManager* m = (CEngineSurfaceCompositionManager*)new char[sizeof(CEngineSurfaceCompositionManager)]; m->m_maxDimension = 100; STextureRecord_00b2aef0 rec; rec.m_width = 512; rec.m_height = 256; CTextureEntry_00b2aef0 t; t.m_pRecord = &rec; long dims[2]; unsigned long level = 1;
    m->GetDimensionsAndAdjustMipmapLevel(dims, &t, &level); if (dims[0] != 64 || dims[1] != 32 || level != 3) { std::printf("00b2aef0_TEST FAIL\n"); return 1; }
    level = 0; m->m_maxDimension = 1000; m->GetDimensionsAndAdjustMipmapLevel(dims, &t, &level); if (dims[0] != 512 || dims[1] != 256 || level != 0) { std::printf("00b2aef0_TEST FAIL\n"); return 1; }
    delete[] (char*)m;
    std::printf("00b2aef0_TEST PASS\n"); return 0;
}
