// Grade: DIFFER(438v438) -- esi vs ecx load register only (twin of 00871290)
// CActionEventTriggerParticle::SetDataFromString(const CCharString& data) -- parse "effect, attach type[, attachment object][, drop]" event data: field 0 is the effect name; the attach type "<WEAPON|...>_<HELPER|DUMMY>" sets Weapon and the helper (4) or dummy (2) attachment flag over the default 8; an attached effect reads the attachment object from the next field, and a further field marks DropRatherThanAttach
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00870E00, 436 bytes (448 with padding), `ret 4`.
// Callees: GetDataFromEventString(const CCharString&, long, CCharString*) @ 0x00870C90 (__fastcall, as the landed
//   CActionEventAddObject::SetDataFromString 0x00870D70), CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0,
//   CCharString::FindCharPos(char) 0x0099E5A0, CCharString::GetSubString(unsigned long, unsigned long) const 0x0099EC70,
//   CBasicString<char>::operator==(const char*) 0x004115A0.
// Data: "" 0x0122D70E, "WEAPON" 0x01277498, "HELPER" 0x01277490, "DUMMY" 0x01277488.
// Layout: Ego_r.pdb CActionEventTriggerParticle (36 bytes): AttachmentFlags +0x14, AttachmentObject +0x18, EffectName
//   +0x1c, Weapon +0x20, DropRatherThanAttach +0x21.
// Shape: the field index is one counter (constant-propagated into the first three calls, materialised in the frame for
//   the last); the attach-type string is a named local in its own block homed in the dead `data` slot, the drop flag's
//   string reuses that slot in a second block. The prefix compare is the inline CCharString::operator==(const char*)
//   of the landed CCombatActionFallStunned ctor; the suffix's HELPER/DUMMY chain tests the string block once and runs
//   the strcmp-with-"" arms and the out-of-line compares as two separate ladders (retail's threaded form).
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    long FindCharPos(char c) const;             // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long length) const;   // 0x0099EC70
    unsigned long GetLength() const { if (PStringData == 0) return 0; return PStringData->Data.StringLength; }
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

bool __fastcall GetDataFromEventString(const CCharString& data, long index, CCharString* out);   // 0x00870C90


struct CActionEventTriggerParticle {
    void* __vftable;                            // +0x00
    unsigned char _base[0x10];                  // +0x04 CActionEventBase words
    long AttachmentFlags;                       // +0x14
    CCharString AttachmentObject;               // +0x18
    CCharString EffectName;                     // +0x1c
    bool Weapon;                                // +0x20
    bool DropRatherThanAttach;                  // +0x21
    bool SetDataFromString(const CCharString& data);
};

bool CActionEventTriggerParticle::SetDataFromString(const CCharString& data)
{
    long field = 0;
    if (!GetDataFromEventString(data, field++, &EffectName))
        return false;
    Weapon = false;
    AttachmentFlags = 8;
    {
        CCharString attachType;
        if (!GetDataFromEventString(data, field++, &attachType))
            return false;
        long underscore = attachType.FindCharPos('_');
        if (underscore > -1) {
            CCharString prefix = attachType.GetSubString(0, underscore);
            CCharString type = attachType.GetSubString(underscore + 1, attachType.GetLength());
            if (prefix == "WEAPON")
                Weapon = true;
            CCharStringData* typeData = type.PStringData;
            if (typeData == 0) {
                if (strcmp("HELPER", "") == 0)
                    goto helper;
                if (strcmp("DUMMY", "") == 0)
                    goto dummy;
            } else {
                if (typeData->Data == "HELPER")
                    goto helper;
                if (typeData->Data == "DUMMY")
                    goto dummy;
            }
            goto done;
        helper:
            AttachmentFlags |= 4;
            goto done;
        dummy:
            AttachmentFlags |= 2;
        done:
            ;
        }
        if (AttachmentFlags & 6) {
            if (!GetDataFromEventString(data, field++, &AttachmentObject))
                return false;
        }
    }
    {
        CCharString drop;
        if (GetDataFromEventString(data, field, &drop))
            DropRatherThanAttach = true;
    }
    return true;
}
