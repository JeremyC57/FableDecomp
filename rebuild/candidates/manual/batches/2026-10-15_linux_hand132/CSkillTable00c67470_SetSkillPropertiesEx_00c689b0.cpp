// CSkillTable_00c67470::SetSkillPropertiesEx(long, const SSkillPropertiesEx&) -- set every property of skill record `index` in turn (index property, GUID, value, flag), stopping at the first error
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C689B0, length 144 bytes. The chunk label "CDrawGuildSeal::SetSkillPropertiesEx"
// keeps the method name; the class is the skill table named by address in the landed sibling 0x00C67470 (same `this` layout).
// Callees: CSkillTable::SetSkillIndexProperty(long, long) (0x00C67470, inlined here: indices 0 and 1 are reserved -> error slot
// +0x3d4 = -1, result -3), SetSkillGUID(long, GUID) @ 0x00C674B0 (16-byte GUID by value, compared against a null GUID with
// `repe cmpsd`), SetSkillValue(long, long) @ 0x00C67720, SetSkillFlag(long, long) @ 0x00C67430 (manifest gap-scan labels
// "CCharacter_AddSkill" / "CCharacter_SetSkillDamage" / "CCharacter_SetSkillAcquired").
// Layout: 0x1c-byte skill records at +0x98 (index property first); the property block: GUID +0, Value +0x10, Property +0x14,
// Flag +0x18.
// Shape: the inlined index check keeps its own `-3` exit; the GUID argument is copied into the 16-byte argument area through edx/ecx
// (`sub esp,10h ; mov eax,esp`); the calls are nested `if (result >= 0)` blocks with one final return (early `return result` per
// step swaps the index/this registers esi/edi), so a negative result falls through to the shared exit.
struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

struct SSkillRecord_00c67470 {
    long          m_property;                      // +0x00
    unsigned char _rest[0x18];
};

struct SSkillPropertiesEx {
    GUID Guid;                                     // +0x00
    long Value;                                    // +0x10
    long Property;                                 // +0x14
    long Flag;                                     // +0x18
};

struct CSkillTable_00c67470 {
    unsigned char         _pad[0x98];
    SSkillRecord_00c67470 m_skills[1];             // +0x98 (open-ended)
    unsigned char         _padB[0x3d4 - 0x98 - 0x1c];
    long                  m_lastError;             // +0x3d4
    long SetSkillIndexProperty(long index, long value)   // 0x00C67470
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
    long SetSkillGUID(long index, GUID guid);      // 0x00C674B0
    long SetSkillValue(long index, long value);    // 0x00C67720
    long SetSkillFlag(long index, long flag);      // 0x00C67430
    long SetSkillPropertiesEx(long index, const SSkillPropertiesEx& properties);
};

long CSkillTable_00c67470::SetSkillPropertiesEx(long index, const SSkillPropertiesEx& properties)
{
    long result = SetSkillIndexProperty(index, properties.Property);
    if (result >= 0) {
        result = SetSkillGUID(index, properties.Guid);
        if (result >= 0) {
            result = SetSkillValue(index, properties.Value);
            if (result >= 0)
                result = SetSkillFlag(index, properties.Flag);
        }
    }
    return result;
}
