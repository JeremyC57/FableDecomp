// CTCWeaponTrail::GetStrikeEventIndexFromEventName(const CCharString& name) -- map an animation strike-event name to a weapon-trail helper index: two fixed names give 0/1, names under the event prefix (and not the excluded prefix) are lower-cased, turned into "weapon_pos_<suffix>" and looked up in the trail's helper-name table
// Leaf: GetStrikeEventIndexFromEventName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076DF90, length 448 bytes, `ret 4`.
// Callees: Compare 0x00411570 (__fastcall; its landed body is repeated above under auto_inline(off) so VC7.1 knows it writes no
//   memory and keeps the name's string block in esi across the two inlined == tests), strncmp 0x00BFEAF8, CCharString::GetSubString
//   0x0099EC70, CCharString::ToLower 0x0099ED10, operator+(const char*, const CCharString&) 0x0099F690 (__fastcall, hidden result
//   built in the dead `name` argument slot), ~CCharString 0x0099EAE0. Data: four static CCharStrings 0x013BAC78/80/64/6C, "weapon_pos_" 0x0126A5A4.
// Shape: the prefix match wraps the lookup (`if (strncmp(...) == 0) { ... }` then one shared `return 0`): early returns for the
//   excluded prefix and the two fixed names share the in-line `return 0` block, the failed prefix match shares the final epilogue.
//   The inline c_str()/length() getters are written null-first (`PStringData == 0 ? "" : ...`) for retail's block order.
// Layout: CCharString handle on the packed CCharStringData (PData +0, StringLength +4); the trail's table count +0x0c and
//   8-byte entries (name first) at +0x54 -- local model (no engine header for CTCWeaponTrail).
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)       // 0x00411570 (landed body; same-TU so VC7.1 knows it writes no memory)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#include <string.h>                                         // strncmp 0x00BFEAF8
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    __forceinline bool operator==(const CBasicStringChar& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare(PData, other.PData) == 0;
    }
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
    CCharString GetSubString(unsigned long start, unsigned long length) const;   // 0x0099EC70
    CCharString ToLower() const;                            // 0x0099ED10
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return PStringData->Data == other.PStringData->Data;
        return false;
    }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
extern CCharString g_StrikeEventName0;                     // 0x013BAC78
extern CCharString g_StrikeEventName1;                     // 0x013BAC80
extern CCharString g_StrikeEventExcludedPrefix;            // 0x013BAC64
extern CCharString g_StrikeEventPrefix;                    // 0x013BAC6C
struct CStrikeEvent { CCharString Name; long Data; };
struct CTCWeaponTrail {
    unsigned char _pad00[0x0c];
    long NoStrikeEvents;                                    // +0x0c
    unsigned char _pad10[0x54 - 0x10];
    CStrikeEvent* StrikeEvents;                             // +0x54
    long GetStrikeEventIndexFromEventName(const CCharString& name);
};

long CTCWeaponTrail::GetStrikeEventIndexFromEventName(const CCharString& name)
{
    if (name == g_StrikeEventName0)
        return 0;
    if (name == g_StrikeEventName1)
        return 1;
    if (strncmp(name.GetString(), g_StrikeEventExcludedPrefix.GetString(), g_StrikeEventExcludedPrefix.GetLength()) == 0)
        return 0;
    if (strncmp(name.GetString(), g_StrikeEventPrefix.GetString(), g_StrikeEventPrefix.GetLength()) == 0) {
        CCharString suffix = name.GetSubString(g_StrikeEventPrefix.GetLength(), name.GetLength()).ToLower();
        CCharString eventName = "weapon_pos_" + suffix;
        for (long i = 0; i < NoStrikeEvents; ++i) {
            if (StrikeEvents[i].Name == eventName)
                return i;
        }
    }
    return 0;
}
