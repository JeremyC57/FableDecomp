// CGameScriptInterface::IsPlayerCarryingItemOfType(const CCharString& defName) const -- whether the hero carries a
// Grade: MATCH (retry 9) -- the empty-name test is written in the caller as two separate tests that `goto` the empty arm
// (strcmp("", "") on the unassigned block, the CBasicString compare otherwise): the inline operator==("") with two returns
// materialised its bool with `sete al` and re-tested it (the chunk 188 gap), and that also hoisted push ebx/ebp above the
// TC lookup; with the branches taken straight off each compare the pushes sit where retail has them
// thing in its primary carrying slot: any thing for an empty name, else one whose definition has that name
// Leaf: IsPlayerCarryingItemOfType
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008973A0, length 224 bytes, `ret 4`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, the TC map lower bound
// copy @ 0x004365B0 (same body as 0x0040F020), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440,
// CBasicString<char>::operator==(const char*) @ 0x004115A0, CThing::GetDefName() (CDefString by hidden pointer)
// @ 0x004C7CC0, CDefString::operator==(const CCharString&) @ 0x004B7980. "" @ 0x0122D70E.
// Layout: CGameScriptInterface PlayerManager +0x14; CThing TC bitset +0x20 (TC 0x46 CTCCarrying = +0x28 bit 6), TC
// map +0x44, dead bit +0x91 bit 0.
// Shape: the hero tests and HasTC give the 8-bit `xor al,al` exit; the carried-item part returns int-typed
// expressions (`return item != 0;`, `return item && ...;`); the CDefString result lands in the dead `defName`
// argument slot; the CCharString `== ""` is open-coded in the caller (strcmp intrinsic on the null block, else the
// CBasicString compare) with `goto empty`; GetTC(k)->F() puts the component straight into ecx.
#include <string.h>

#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};

class CDefString { public:
    long Index;
    bool operator==(const CCharString& name) const; // 0x004B7980
};

class CTCBase;
struct CInterfaceMap {                    // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0 (this TU's copy of the 0x0040F020 body)
};

class CThing;
class CTCCarrying { public:
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

class CThing { public:
    virtual void v000();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];       // +0x20 std::bitset of the interface types present
    CInterfaceMap m_interfaces;           // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_dead : 1;             // +0x91 bit 0
    unsigned char m_91_1 : 7;
    CDefString GetDefName() const;        // 0x004C7CC0
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CPlayer {
public:
    CThing* GetHeroThing();               // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();             // 0x00449970 (the non-const overload; 0x00449960 is the const one)
};

class CGameScriptInterface { public:
    virtual void v000();
    void* World;                          // +0x04
    void* Component;                      // +0x08
    void* DisplayEngine;                  // +0x0c
    void* DefinitionManager;              // +0x10
    CPlayerManager* PlayerManager;        // +0x14
    bool IsPlayerCarryingItemOfType(const CCharString& defName) const;
};

bool CGameScriptInterface::IsPlayerCarryingItemOfType(const CCharString& defName) const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetHeroThing();
    if (hero && !hero->m_dead && hero->HasTC(0x46)) {
        CThing* item = hero->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        if (defName.PStringData == 0) {
            if (strcmp("", "") == 0)
                goto empty;
        } else if (defName.PStringData->Data == "")
            goto empty;
        return item && item->GetDefName() == defName;
empty:
        return item != 0;
    }
    return false;
}