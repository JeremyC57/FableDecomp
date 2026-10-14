// CSkillTable_00c67470::SetSkillIndexProperty(long index, long value) -- store the value into the 0x1c-byte skill record `index` (first word) when it changed; indices 0 and 1 are reserved (error -3, the error slot at +0x3d4 set to -1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C67470, length 50 bytes.
// Callees: none. Shape: the two reserved indices share the error tail; the record address is formed once (`imul eax,eax,1Ch ; lea eax,[eax+ecx+98h]`) after the
// compare operand was loaded through the same addressing; the store is skipped when the value is unchanged. The chunk label "CDrawGuildSeal::...
// SetSkillIndexProperty" keeps the method name; the class is named by address.
struct SSkillRecord_00c67470 {
    long          m_property;                      // +0x00
    unsigned char _rest[0x18];
};

struct CSkillTable_00c67470 {
    unsigned char         _pad[0x98];
    SSkillRecord_00c67470 m_skills[1];             // +0x98 (open-ended)
    unsigned char         _padB[0x3d4 - 0x98 - 0x1c];
    long                  m_lastError;             // +0x3d4
    long SetSkillIndexProperty(long index, long value);
};

long CSkillTable_00c67470::SetSkillIndexProperty(long index, long value)
{
    if (index == 0 || index == 1) {
        m_lastError = -1;
        return -3;
    }
    SSkillRecord_00c67470* record = &m_skills[index];
    if (value != record->m_property)
        record->m_property = value;
    return 0;
}