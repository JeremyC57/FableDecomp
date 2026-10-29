// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CTCInventoryExperience::GetExperienceAbilityDescription(long ability, long level) -- resolve the ability's description text
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056FE7C, 193 bytes.
// Callees: bitset<>::test 0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the inlined
//   CThing::GetInterface lookup for interface 0x6f = the hero abilities component), CTCInventoryAbilities::GetAbilityPDef
//   0x005EDFD9 (returns a counted CDefPointer by value), CTCInventoryAbilities::GetAbilityMaximumLevel 0x005EDEA3,
//   CTCInventoryBase::GetInventoryText 0x005BD273 (inherited), CWideString copy ctor 0x0099B720 from the empty-string
//   static 0x013BCA24. The CDefPointer destructor releases the def through virtual slot 1 when its count reaches 0.
// Layout: CHeroAbilityDef description table pointer +0x60 and maxed-out description +0x6c (retail offsets);
//   CThing interface bitset +0x20 and interface map +0x44 as in the landed GetInterface users. Local model: the
//   engine/ headers for these classes do not declare the methods called here.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString { public: void* PStringData; CWideString(const CWideString&); ~CWideString(); };
extern CWideString g_EmptyWideString;                   // 0x013BCA24
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
class CDefPointeeBase { public: virtual void VFunc00(); virtual void Release(); long RefCount; };
class CHeroAbilityDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x58];
    long* Descriptions;                                 // +0x60
    unsigned char _pad64[8];
    long MaxedOutDescription;                           // +0x6c
};
template<class T> class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { CDefPointeeBase* p = (CDefPointeeBase*)Object; if (--p->RefCount == 0) p->Release(); } }
};
class CTCBase;
class CTCInventoryAbilities { public:
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;   // 0x005EDFD9
    long GetAbilityMaximumLevel(EHeroAbility ability) const;                         // 0x005EDEA3
};
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType,CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> Entry;
    Entry* first; Entry* last;
    Entry* LowerBound(const ETCInterfaceType& key);                     // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CPresenceBits { bool test(long kind); };         // bitset<>::test 0x00410DE0
class CThing { public:
    unsigned char _pad00[0x20]; CPresenceBits present; unsigned char _pad21[0x23]; CInterfaceMap interfaces;
    template<class T> __forceinline bool GetInterface(T** out, long kind) {
        if (present.test(kind)) {
            ETCInterfaceType key = (ETCInterfaceType)(kind); CInterfaceMap* map = &interfaces; CInterfaceMap::Entry* it = map->LowerBound(key);
            if (it == map->last || kind < it->first) it = map->last;
            *out = (T*)((void*)it->second); return true;
        }
        return false;
    }
};
struct CTCInventoryBase {
    void* __vftable; CThing* ParentThing;
    CWideString GetInventoryText(long text_id);         // 0x005BD273
};
class CTCInventoryExperience : public CTCInventoryBase { public:
    CWideString GetExperienceAbilityDescription(long ability, long level);
};
CWideString CTCInventoryExperience::GetExperienceAbilityDescription(long ability, long level)
{
    CTCInventoryAbilities* abilities;
    if (!ParentThing->GetInterface(&abilities, 0x6f))
        return g_EmptyWideString;
    CDefPointer<const CHeroAbilityDef> def = abilities->GetAbilityPDef((EHeroAbility)ability);
    long textId;
    if (level == abilities->GetAbilityMaximumLevel((EHeroAbility)ability) + 1)
        textId = def.Object->MaxedOutDescription;
    else
        textId = def.Object->Descriptions[level];
    if (textId)
        return GetInventoryText(textId);
    return g_EmptyWideString;
}