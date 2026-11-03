// CEAXBuffer::SetOcclusionSendParameters(const _EAXSOURCEOCCLUSIONSENDPROPERTIES* levels, unsigned long size) -- validate a list of per-component sound settings (each component GUID must be one of the four known ids, the level within -10000..0, the ratio within 0..1 and both scales within 0..10; an empty or bad list sets the owner's last error -2 and returns -3), then store each entry in its component's sound record and mark dirty bit 0x400000 when the owner's current component changed; returns 0
// Leaf: ?SetOcclusionSendParameters@CEAXBuffer@@QAEJPBU_EAXSOURCEOCCLUSIONSENDPROPERTIES@@K@Z
// Name (owner decision 2026-11-03 D3): a CEAXBuffer (pass 11 CEaxSource) method; the EAX 4.0 source dispatcher 0x00C68510
//   calls it under this name and SDK parameter type (queue cx11). The CEntity label was a propagated game name.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C65450, length 576 bytes (+ int3 padding), `ret 8`.
// Identity: the manifest label is the propagated autoname; the body is the audio-library property setter family of the
//   landed CEAXBuffer::SetExclusionSendParameters 0x00C65690 / SetSendParameters 0x00C64FB0 (owner +0x14 with LastError
//   +0x3d4, -3 failure, GUIDs compared with the memcmp intrinsic). The property's real name is unknown.
// Callees: none (memcmp is the `repe cmpsd` intrinsic). Data: the component GUIDs @ 0x012B9468, 0x012B9478, 0x012B9488,
//   0x012B9498 (component indices 0..3); float pool 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, 10.0f @ 0x01237F0C.
// Layout: local model (no PDB class) shared with the landed siblings: dirty flags +0x04, owner +0x14 (current component
//   index at owner +4), mesh records +0x70 (0x18 each), sound records +0xd0 (0x20 each), VFX records +0x150 (0x18 each).
// Shape: the landed SetExclusionSendParameters with 32-byte entries (`size >> 5`) and four stored fields.
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)

struct _GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
extern "C" const _GUID EAXPROPERTYID_EAX40_FXSlot0;          // 0x012B9468
extern "C" const _GUID EAXPROPERTYID_EAX40_FXSlot1;          // 0x012B9478
extern "C" const _GUID EAXPROPERTYID_EAX40_FXSlot2;          // 0x012B9488
extern "C" const _GUID EAXPROPERTYID_EAX40_FXSlot3;          // 0x012B9498

struct CEAXListener {
    unsigned char _pad0[4];
    long          CurrentComponent;                // +0x04
    unsigned char _pad8[0x3d4 - 8];
    long          LastError;                       // +0x3d4
};

struct SMeshRecord {                               // 0x18 bytes
    long          Level;                           // +0x00
    long          LevelHF;                         // +0x04
    unsigned char _pad8[0x10];
};
struct SSoundRecord {                              // 0x20 bytes
    long          Level;                           // +0x00
    float         Ratio;                           // +0x04
    float         Scale1;                          // +0x08
    float         Scale2;                          // +0x0c
    unsigned char _pad10[0x10];
};
struct SVFXRecord {                                // 0x18 bytes
    long          Level;                           // +0x00
    float         Ratio;                           // +0x04
    unsigned char _pad8[0x10];
};

struct _EAXSOURCEOCCLUSIONSENDPROPERTIES {                     // 0x20 bytes
    _GUID Component;                               // +0x00
    long  Level;                                   // +0x10
    float Ratio;                                   // +0x14
    float Scale1;                                  // +0x18
    float Scale2;                                  // +0x1c
};

struct CEAXBuffer {
    unsigned char    _pad0[0x4];
    unsigned long    m_dirtyFlags;                 // +0x04
    unsigned char    _pad8[0x14 - 0x8];
    CEAXListener*    m_pOwner;                     // +0x14
    unsigned char    _pad18[0x70 - 0x18];
    SMeshRecord      m_mesh[4];                    // +0x70
    SSoundRecord     m_sound[4];                   // +0xd0
    SVFXRecord       m_vfx[4];                     // +0x150
    long SetOcclusionSendParameters(const _EAXSOURCEOCCLUSIONSENDPROPERTIES* levels, unsigned long size);
};

long CEAXBuffer::SetOcclusionSendParameters(const _EAXSOURCEOCCLUSIONSENDPROPERTIES* levels, unsigned long size)
{
    unsigned long count = size / sizeof(_EAXSOURCEOCCLUSIONSENDPROPERTIES);
    if (count == 0) {
        m_pOwner->LastError = -2;
        return -3;
    }
    unsigned long i;
    for (i = 0; i < count; ++i) {
        if (memcmp(&levels[i].Component, &EAXPROPERTYID_EAX40_FXSlot0, sizeof(_GUID)) != 0
            && memcmp(&levels[i].Component, &EAXPROPERTYID_EAX40_FXSlot1, sizeof(_GUID)) != 0
            && memcmp(&levels[i].Component, &EAXPROPERTYID_EAX40_FXSlot2, sizeof(_GUID)) != 0
            && memcmp(&levels[i].Component, &EAXPROPERTYID_EAX40_FXSlot3, sizeof(_GUID)) != 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].Level < -10000 || levels[i].Level > 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].Ratio < 0.0f || levels[i].Ratio > 1.0f) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].Scale1 < 0.0f || levels[i].Scale1 > 10.0f) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].Scale2 < 0.0f || levels[i].Scale2 > 10.0f) {
            m_pOwner->LastError = -2;
            return -3;
        }
    }
    for (i = 0; i < count; ++i) {
        long index;
        if (memcmp(&levels[i].Component, &EAXPROPERTYID_EAX40_FXSlot0, sizeof(_GUID)) == 0)
            index = 0;
        else if (memcmp(&levels[i].Component, &EAXPROPERTYID_EAX40_FXSlot1, sizeof(_GUID)) == 0)
            index = 1;
        else if (memcmp(&levels[i].Component, &EAXPROPERTYID_EAX40_FXSlot2, sizeof(_GUID)) == 0)
            index = 2;
        else if (memcmp(&levels[i].Component, &EAXPROPERTYID_EAX40_FXSlot3, sizeof(_GUID)) == 0)
            index = 3;
        else {
            m_pOwner->LastError = -2;
            return -3;
        }
        m_sound[index].Level = levels[i].Level;
        m_sound[index].Ratio = levels[i].Ratio;
        m_sound[index].Scale1 = levels[i].Scale1;
        m_sound[index].Scale2 = levels[i].Scale2;
        if (index == m_pOwner->CurrentComponent)
            m_dirtyFlags |= 0x400000;
    }
    return 0;
}