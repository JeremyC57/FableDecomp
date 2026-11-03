// CEAXListener::SetSlotProperty(unsigned long slot, const EAXFXSLOTPROPERTIES&) -- EAX 4.0 FX-slot property dispatcher: set every property of slot `slot` in turn (the lock, the guidLoadEffect, the volume, the flags), stopping at the first error
// Leaf: ?SetSlotProperty@CEAXListener@@QAEJKABU_EAXFXSLOTPROPERTIES@@@Z
// Name (naming wave 19, chunk 685): the object is the landed CEAXListener (the slot setters SetSlotLock 0x00C67470 / SetSlotLoadEffect 0x00C674B0 /
//   SetSlotVolume 0x00C67720 / SetSlotFlags 0x00C67430 share its `this`); descriptive member name (libpk, no PDB); landed as the propagated
//   CSkillTable_00c67470::SetSkillPropertiesEx (the skill-table reading of the EAX slot table).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C689B0, length 144 bytes. The chunk label "CDrawGuildSeal::SetSlotProperty"
// keeps the method name; the class is the EAX context of the landed slot setters (same `this` layout).
// Callees: CEAXListener::SetSlotLock (0x00C67470, inlined here: indices 0 and 1 are reserved -> error slot
// +0x3d4 = -1, result -3), SetSlotLoadEffect(unsigned long, _GUID) @ 0x00C674B0 (16-byte GUID by value, compared against a null _GUID with
// `repe cmpsd`), SetSlotVolume @ 0x00C67720, SetSlotFlags @ 0x00C67430 (manifest gap-scan labels
// "CCharacter_AddSkill" / "CCharacter_SetSkillDamage" / "CCharacter_SetSkillAcquired").
// Layout: 0x1c-byte slot records at +0x98 (index property first); the property block: GUID +0, Value +0x10, Property +0x14,
// Flag +0x18.
// Shape: the inlined index check keeps its own `-3` exit; the GUID argument is copied into the 16-byte argument area through edx/ecx
// (`sub esp,10h ; mov eax,esp`); the calls are nested `if (result >= 0)` blocks with one final return (early `return result` per
// step swaps the index/this registers esi/edi), so a negative result falls through to the shared exit.
struct _GUID {                                     // _GUID, 16 bytes (the landed 0x00C674B0 spelling)
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};

struct SEaxSlotRecord {
    long          m_lock;                          // +0x00
    unsigned char _rest[0x18];
};

struct _EAXFXSLOTPROPERTIES {                      // EAX 4.0 SDK tag (engine/EAX40.h)
    _GUID         guidLoadEffect;                  // +0x00
    long          lVolume;                         // +0x10
    long          lLock;                           // +0x14
    unsigned long ulFlags;                         // +0x18
};

struct CEAXListener {
    unsigned char         _pad[0x98];
    SEaxSlotRecord m_slots[1];              // +0x98 (open-ended)
    unsigned char         _padB[0x3d4 - 0x98 - 0x1c];
    long                  m_lastError;             // +0x3d4
    long SetSlotLock(unsigned long slot, long lock)   // 0x00C67470 (the landed ?SetSlotLock@CEAXListener@@QAEJKJ@Z, inlined here)
    {
        if (slot == 0 || slot == 1) {
            m_lastError = -1;
            return -3;
        }
        SEaxSlotRecord* record = &m_slots[slot];
        if (lock != record->m_lock)
            record->m_lock = lock;
        return 0;
    }
    long SetSlotLoadEffect(unsigned long slot, _GUID effect);   // 0x00C674B0 (the landed ?SetSlotLoadEffect@CEAXListener@@QAEJKU_GUID@@@Z)
    long SetSlotVolume(unsigned long slot, long volume);   // 0x00C67720 (the landed ?SetSlotVolume@CEAXListener@@QAEJKJ@Z)
    long SetSlotFlags(unsigned long slot, unsigned long flags);   // 0x00C67430 (the landed ?SetSlotFlags@CEAXListener@@QAEJKK@Z)
    long SetSlotProperty(unsigned long index, const _EAXFXSLOTPROPERTIES& properties);
};

long CEAXListener::SetSlotProperty(unsigned long index, const _EAXFXSLOTPROPERTIES& properties)
{
    long result = SetSlotLock(index, properties.lLock);
    if (result >= 0) {
        result = SetSlotLoadEffect(index, properties.guidLoadEffect);
        if (result >= 0) {
            result = SetSlotVolume(index, properties.lVolume);
            if (result >= 0)
                result = SetSlotFlags(index, properties.ulFlags);
        }
    }
    return result;
}