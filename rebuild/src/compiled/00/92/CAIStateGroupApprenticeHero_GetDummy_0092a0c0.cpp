// CAIStateGroup_ApprenticeHero::GetDummy() -- starting from a random dummy type, pick the first free type whose dummy can be found
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092A0C0, length 346 bytes.
// Callees: GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, the vector-map lower_bound inlined (interface 0xAA, bit 10 of
//          +0x34; the lookup's result is unused), IsDummyTypeLegal (static, inlined: the three "in use" bytes @ 0x013BAE18..1A),
//          CCharString(const char*, long) @ 0x0099EBF0 ("MeleePractice" @ 0x01291A18, "SkillTarget" @ 0x01291A28, "WillDummy"
//          @ 0x01291A34, "" @ 0x0122D70E), the dummy finder @ 0x00929FC0 (manifest "CAIStateGroupBase_SelectRandomElement"),
//          ~CCharString @ 0x0099EAE0.
// Shape: the start type is the unsigned world-seed random `% 3`, the candidate `(start + i) % 3` is signed; an inner
//          search loop breaks on a legal type, `if (i == 3)` gives up, the dummy name comes from an inline helper returning
//          CCharString by value from a switch (the four ctor calls tail-merge into one with `push -1` hoisted), and a
//          miss bumps i inside the string's block before looping back into the search.
// Layout: engine/CAIStateGroup_ApprenticeHero.h (PDummy +0x18, Action +0x1C) behind engine/CAIStateGroupBase.h.
#include "engine/CAIStateGroup_ApprenticeHero.h"

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)        // STLport lower_bound (inlined in this TU)
    {
        SEntry* it = first;
        int len = int(last - first);
        while (len > 0) {
            int half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCInformation;                          // interface 0xaa

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

extern bool g_dummyTypeInUse0;                  // 0x013BAE18
extern bool g_dummyTypeInUse1;                  // 0x013BAE19
extern bool g_dummyTypeInUse2;                  // 0x013BAE1A

inline CCharString GetDummyName(long type)
{
    switch (type) {
    case 0: return CCharString("MeleePractice");
    case 1: return CCharString("SkillTarget");
    case 2: return CCharString("WillDummy");
    }
    return CCharString("");
}

struct CAIStateGroupApprenticeHeroMethods : CAIStateGroup_ApprenticeHero {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
    static bool IsDummyTypeLegal(long dummyType)
    {
        switch (dummyType) {
        case 0: return !g_dummyTypeInUse0;
        case 1: return !g_dummyTypeInUse1;
        case 2: return !g_dummyTypeInUse2;
        }
        return false;
    }
    CThingGameObject* FindDummy(const CCharString& name);   // 0x00929FC0
    bool GetDummy();
};

bool CAIStateGroupApprenticeHeroMethods::GetDummy()
{
    long start = Random(3);
    PDummy = 0;
    long i = 0;
    while (i < 3) {
        for (; i < 3; ++i) {
            Action = (start + i) % 3;
            CTCInformation* information;
            Brain()->PCreature->GetInterface(&information, 0xaa);
            if (IsDummyTypeLegal(Action))
                break;
        }
        if (i == 3)
            break;
        CCharString name = GetDummyName(Action);
        PDummy = FindDummy(name);
        if (PDummy)
            return true;
        ++i;
    }
    return false;
}