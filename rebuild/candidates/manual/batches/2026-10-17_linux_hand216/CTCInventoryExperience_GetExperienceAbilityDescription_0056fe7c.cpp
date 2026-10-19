// CTCInventoryExperience::GetExperienceAbilityDescription(long ability, long level) -- resolves the ability description text id
// Retail 0x0056FE7C, 193 bytes. The hero ability component is fetched through interface 0x6f; its def pointer is
// reference counted, and the max-level arm uses the def's terminal description.
#pragma optimize("y", off)
#pragma optimize("s", on)
typedef unsigned short wchar_t;
struct CWideString { void* data; CWideString(const CWideString&); ~CWideString(); };
extern CWideString g_EmptyWideString; // 0x013BCA24
struct CDefPointeeBase { virtual void v00(); virtual void Destroy(); long count; };
struct CHeroAbilityDef {
    unsigned char pad[0x60];
    long* descriptions;                         // +0x60
    unsigned char pad2[8];
    long maxedOutDescription;                   // +0x6c
};
struct CTCInventoryAbilities {
    CHeroAbilityDef* GetAbilityPDef(CHeroAbilityDef** out, long ability); // 0x005EDFD9
    long GetAbilityMaximumLevel(long ability);                            // 0x005EDEA3
};
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry* first; Entry* last; Entry* LowerBound(const long*); };
struct CPresenceBits { bool test(long kind); }; // 0x00410DE0
struct CThing {
    unsigned char pad[0x20]; CPresenceBits present; unsigned char pad2[0x23]; CInterfaceMap interfaces;
    template<class T> __forceinline bool GetInterface(T** out, long kind) {
        if (present.test(kind)) {
            long key=kind; CInterfaceMap* map=&interfaces; CInterfaceMap::Entry* it=map->LowerBound(&key);
            if (it==map->last || kind < it->type) it=map->last;
            *out=(T*)it->component; return true;
        }
        return false;
    }
};
struct CTCInventoryExperience {
    void* vtable; CThing* ParentThing;
    CWideString GetInventoryText(long textId); // 0x005BD273
    CWideString GetExperienceAbilityDescription(long ability, long level);
};
template<class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer() { if (Object) { CDefPointeeBase* p=(CDefPointeeBase*)Object; if (--p->count == 0) p->Destroy(); } }
};
CWideString CTCInventoryExperience::GetExperienceAbilityDescription(long ability, long level)
{
    CTCInventoryAbilities* abilities;
    if (!ParentThing->GetInterface(&abilities, 0x6f))
        return g_EmptyWideString;
    CDefPointer<CHeroAbilityDef> def;
    abilities->GetAbilityPDef(&def.Object, ability);
    long textId;
    if (level == abilities->GetAbilityMaximumLevel(ability) + 1)
        textId = def.Object->maxedOutDescription;
    else
        textId = def.Object->descriptions[level];
    if (textId)
        return GetInventoryText(textId);
    return g_EmptyWideString;
}
