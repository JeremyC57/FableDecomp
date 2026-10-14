#include <cstdio>
struct CResource { virtual void v00() {} unsigned char _body[0x18]; CResource() {} };
struct SLipSyncBuffers { void* m_pPhonemes; void* m_pTimes; void* m_pWeights; void* m_pOwner; SLipSyncBuffers() : m_pPhonemes(0), m_pTimes(0), m_pWeights(0), m_pOwner(0) { } };
struct CLipSyncDataBankEntry { struct CData : CResource { SLipSyncBuffers m_buffers; void* m_pData; unsigned long m_size; virtual void v00() {} CData(void* data, unsigned long size); }; };
CLipSyncDataBankEntry::CData::CData(void* data, unsigned long size) : CResource(), m_buffers() { m_pData = data; m_size = size; }
int main()
{
    int blob[2];
    CLipSyncDataBankEntry::CData d(blob, 8);
    if (d.m_buffers.m_pPhonemes != 0 || d.m_buffers.m_pTimes != 0 || d.m_buffers.m_pWeights != 0 || d.m_buffers.m_pOwner != 0 || d.m_pData != blob || d.m_size != 8) { std::printf("00a43580_TEST FAIL\n"); return 1; }
    std::printf("00a43580_TEST PASS\n");
    return 0;
}
