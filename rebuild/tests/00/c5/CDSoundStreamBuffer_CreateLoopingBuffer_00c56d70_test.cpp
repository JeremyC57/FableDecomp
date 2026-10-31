// Behaviour test for CDSoundStreamBuffer::CreateLoopingBuffer (0x00C56D70): the matched control flow over mock COM objects.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __stdcall
#endif
struct GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
extern "C" const GUID IID_IDirectSound3DBuffer = { 0x279AFA86, 0x4981, 0x11CE, { 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60 } };
extern "C" const GUID IID_IKsPropertySet = { 0x31EFAC30, 0x515C, 0x11D0, { 0xA9, 0xAA, 0x00, 0xAA, 0x00, 0x61, 0xBE, 0x93 } };
const unsigned char g_eaxSourcePropertySetGuid_012beea0[16] = { 0x81, 0x68, 0xFA, 0xA8, 0x76, 0xB4, 0xD3, 0x11, 0xBD, 0xB9, 0x00, 0xC0, 0xF0, 0x2D, 0xDF, 0x87 };
struct EAXBUFFERPROPERTIES {
    long lDirect, lDirectHF, lRoom, lRoomHF; float flRoomRolloffFactor; long lObstruction; float flObstructionLFRatio;
    long lOcclusion; float flOcclusionLFRatio, flOcclusionRoomRatio; long lOutsideVolumeHF; float flAirAbsorptionFactor;
    unsigned long dwFlags; long _reserved[5];
};
static int g_fail;        // which step fails (0 = none)
static long g_playResult;
static long g_setDirect;
static unsigned long g_mode, g_playFlags;
struct IKsPropertySet {
    long Set(const GUID& set, unsigned long id, void*, unsigned long, void* data, unsigned long bytes)
    {
        if (g_fail == 5) return -1;
        if (std::memcmp(&set, g_eaxSourcePropertySetGuid_012beea0, 16) != 0 || id != 1 || bytes != sizeof(EAXBUFFERPROPERTIES)) return -2;
        g_setDirect = ((EAXBUFFERPROPERTIES*)data)->lDirect;
        return 0;
    }
};
static IKsPropertySet g_props;
static int g_qiCount;
struct IDirectSound3DBuffer {
    long QueryInterface(const GUID& iid, void** object)
    {
        ++g_qiCount;
        if (&iid != &IID_IKsPropertySet) return -2;
        if ((g_fail == 3 && g_qiCount == 1) || (g_fail == 4 && g_qiCount == 2)) return -1;
        *object = &g_props; return 0;
    }
    long SetMode(unsigned long mode, unsigned long) { if (g_fail == 6) return -1; g_mode = mode; return 0; }
};
static IDirectSound3DBuffer g_3d;
struct IDirectSoundBuffer {
    long QueryInterface(const GUID& iid, void** object)
    {
        if (g_fail == 2 || &iid != &IID_IDirectSound3DBuffer) return -1;
        *object = &g_3d; return 0;
    }
    long Play(unsigned long, unsigned long, unsigned long flags) { g_playFlags = flags; return g_playResult; }
};
static IDirectSoundBuffer g_buffer;
class CDSoundStreamBuffer {
public:
    IDirectSoundBuffer* m_pBuffer;
    unsigned long m_rate;
    bool CreateBuffer(unsigned long a, unsigned long b, bool c)
    {
        if (g_fail == 1 || a != 1 || !c) return false;
        m_rate = b; m_pBuffer = &g_buffer; return true;
    }
    bool CreateLoopingBuffer(unsigned long rate);
};
bool CDSoundStreamBuffer::CreateLoopingBuffer(unsigned long rate)
{
    if (!CreateBuffer(1, rate, true))
        return false;
    IDirectSound3DBuffer* buffer3d;
    if (m_pBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&buffer3d) < 0)
        return false;
    IKsPropertySet* properties;
    if (buffer3d->QueryInterface(IID_IKsPropertySet, (void**)&properties) < 0)
        return false;
    if (buffer3d->SetMode(2, 0) < 0)
        return false;
    if (buffer3d->QueryInterface(IID_IKsPropertySet, (void**)&properties) < 0)
        return false;
    EAXBUFFERPROPERTIES eax;
    std::memset(&eax, 0, sizeof(eax));
    eax.lDirect = -10000;
    if (properties->Set(*(const GUID*)g_eaxSourcePropertySetGuid_012beea0, 1, 0, 0, &eax, sizeof(eax)) < 0)
        return false;
    if (m_pBuffer->Play(0, 0, 1) != 0)
        return false;
    return true;
}
int main()
{
    CDSoundStreamBuffer s;
    for (int f = 1; f <= 6; ++f) {
        g_fail = f; g_qiCount = 0; g_playResult = 0;
        if (s.CreateLoopingBuffer(22050)) { std::printf("00c56d70_TEST FAIL\n"); return 1; }
    }
    g_fail = 0; g_qiCount = 0; g_playResult = 1;        // Play returns a success code other than DS_OK
    if (s.CreateLoopingBuffer(22050)) { std::printf("00c56d70_TEST FAIL\n"); return 1; }
    g_qiCount = 0; g_playResult = 0; g_setDirect = 0; g_mode = 0; g_playFlags = 0;
    if (!s.CreateLoopingBuffer(44100) || s.m_rate != 44100 || g_setDirect != -10000 || g_mode != 2 || g_playFlags != 1 || g_qiCount != 2) {
        std::printf("00c56d70_TEST FAIL\n"); return 1;
    }
    std::printf("00c56d70_TEST PASS\n");
    return 0;
}