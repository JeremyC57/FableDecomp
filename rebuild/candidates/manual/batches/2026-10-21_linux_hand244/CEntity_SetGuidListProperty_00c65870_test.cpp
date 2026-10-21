#include <cstdio>
#include <cstring>
// behaviour test: CEntity::SetGuidListProperty validates the GUID list and stores one or two entries with the dirty bit
struct SGuid { unsigned int Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
static const SGuid g_A = { 0xC4D79F1E, 0xF1AC, 0x436B, { 0xA8, 0x1D, 0xA7, 0x38, 0xE7, 0x04, 0x54, 0x69 } };
static const SGuid g_B = { 0x08C00E96, 0x74BE, 0x4491, { 0x93, 0xAA, 0xE8, 0xAD, 0x35, 0xA4, 0x91, 0x17 } };
static const SGuid g_C = { 0x1D433B88, 0xF0F6, 0x4637, { 0x91, 0x9F, 0x60, 0xE7, 0xE0, 0x6B, 0x5E, 0xDD } };
static const SGuid g_D = { 0xEFFF08EA, 0xC7D8, 0x44AB, { 0x93, 0xAD, 0x6D, 0xBD, 0x5F, 0x91, 0x00, 0x64 } };
static const SGuid g_E = { 0xF317866D, 0x924C, 0x450C, { 0x86, 0x1B, 0xE6, 0xDA, 0xA2, 0x5E, 0x7C, 0x20 } };
static const SGuid g_Null = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
struct CEntityOwner { long LastError; };
struct CEntity {
    unsigned long m_dirtyFlags;
    CEntityOwner* m_pOwner;
    SGuid m_guids[2];
    long SetGuidListProperty(const SGuid* guids, unsigned long size);
};
long CEntity::SetGuidListProperty(const SGuid* guids, unsigned long size)
{
    unsigned long count = size >> 4;
    if (count == 0) { m_pOwner->LastError = -2; return -3; }
    for (unsigned long i = 0; i < count; ++i) {
        if (std::memcmp(&guids[i], &g_A, 16) != 0 && std::memcmp(&guids[i], &g_B, 16) != 0
            && std::memcmp(&guids[i], &g_C, 16) != 0 && std::memcmp(&guids[i], &g_D, 16) != 0
            && std::memcmp(&guids[i], &g_E, 16) != 0 && std::memcmp(&guids[i], &g_Null, 16) != 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
    }
    if (count == 1) {
        if (std::memcmp(&m_guids[0], &guids[0], 16) != 0) { m_guids[0] = guids[0]; m_dirtyFlags |= 0x400000; }
    }
    else if (count == 2) {
        if (std::memcmp(&m_guids[0], &guids[0], 16) != 0 || std::memcmp(&m_guids[1], &guids[1], 16) != 0) {
            m_guids[0] = guids[0]; m_guids[1] = guids[1]; m_dirtyFlags |= 0x400000;
        }
    }
    return 0;
}
int main()
{
    CEntityOwner owner; owner.LastError = 0;
    CEntity e; e.m_dirtyFlags = 0; e.m_pOwner = &owner; e.m_guids[0] = g_Null; e.m_guids[1] = g_Null;
    bool ok = e.SetGuidListProperty(&g_A, 8) == -3 && owner.LastError == -2;
    SGuid bad = g_A; bad.Data1 ^= 1;
    owner.LastError = 0;
    ok = ok && e.SetGuidListProperty(&bad, 16) == -3 && owner.LastError == -2 && e.m_dirtyFlags == 0;
    ok = ok && e.SetGuidListProperty(&g_Null, 16) == 0 && e.m_dirtyFlags == 0;           // unchanged: no dirty bit
    ok = ok && e.SetGuidListProperty(&g_C, 16) == 0 && e.m_dirtyFlags == 0x400000 && std::memcmp(&e.m_guids[0], &g_C, 16) == 0;
    SGuid two[2] = { g_C, g_E };
    e.m_dirtyFlags = 0;
    ok = ok && e.SetGuidListProperty(two, 32) == 0 && e.m_dirtyFlags == 0x400000 && std::memcmp(&e.m_guids[1], &g_E, 16) == 0;
    e.m_dirtyFlags = 0;
    SGuid three[3] = { g_A, g_B, g_D };
    ok = ok && e.SetGuidListProperty(three, 48) == 0 && e.m_dirtyFlags == 0;   // three entries are validated but not stored
    if (!ok) { std::printf("00c65870_TEST FAIL\n"); return 1; }
    std::printf("00c65870_TEST PASS\n");
    return 0;
}
