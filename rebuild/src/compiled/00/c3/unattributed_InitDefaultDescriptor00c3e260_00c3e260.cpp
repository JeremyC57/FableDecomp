// _unattributed::InitDefaultDescriptor_00c3e260(SDescriptor_00c3e260*) -- fill a 28-byte descriptor with the module's three default words and four zero words
// Leaf: InitDefaultDescriptor_00c3e260
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3E260, length 45 bytes.
// Callees: none. Globals 0x0139C9B8, 0x0139C9D0, 0x0139C9F4 (.data words of the 0x00C3xxxx module).
// Identity: the manifest label CQ_GuildTrainingSkillScript::Init is propagated. A free `__cdecl` function
// (plain `ret`, the record pointer at [esp+4]); its callers 0x00C33470 and 0x00C3FD20 use it when no
// descriptor is supplied (the other branch copies seven words with `rep movsd`). Landed as _unattributed.
struct SDescriptor_00c3e260 {                               // 7 dwords
    unsigned long m_00;
    unsigned long m_04;
    unsigned long m_08;
    unsigned long m_0c;
    unsigned long m_10;
    unsigned long m_14;
    unsigned long m_18;
};

extern unsigned long g_defaultWord_0139c9b8;                // 0x0139C9B8
extern unsigned long g_defaultWord_0139c9d0;                // 0x0139C9D0
extern unsigned long g_defaultWord_0139c9f4;                // 0x0139C9F4

void __cdecl InitDefaultDescriptor_00c3e260(SDescriptor_00c3e260* descriptor)
{
    descriptor->m_00 = g_defaultWord_0139c9b8;
    descriptor->m_04 = g_defaultWord_0139c9d0;
    descriptor->m_08 = g_defaultWord_0139c9f4;
    descriptor->m_0c = 0;
    descriptor->m_10 = 0;
    descriptor->m_14 = 0;
    descriptor->m_18 = 0;
}