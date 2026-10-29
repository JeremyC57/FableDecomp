// NCreatureAI::StopSpeaking(CThingCreatureBase&) -- clears the creature's speech emote: SetEmoteText(priority 7, L"", 0, 0) on its emote icon component
// Leaf: ?StopSpeaking@
// Retail 0x00864580, 0x5C bytes (+ int3 padding), __fastcall (creature in ecx). PDB (Ego_r):
// ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z. Callers 0x00919A06 (tail jmp), 0x0094BB13, 0x0098013D, ...
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the non-const twin of 0x004365B0: the
// creature is a non-const reference), CWideString(const wchar_t*) @ 0x0099B6B0 on L"" @ 0x0122D70C,
// CTCEmoteIcon::SetEmoteText(EEmotePriority, const CWideString&, float, float) @ 0x0071AC70, ~CWideString @ 0x0099B510.
// Interface type 0x45 = CTCEmoteIcon (bitset word 2 bit 5: `test byte ptr [ecx+28h],20h`).
// Shape: the __forceinline GetInterface(type, &out) template puts the component in esi; the empty string is a NAMED local
// built before the argument pushes (a bare temporary argument is constructed between the pushes and passed as the
// constructor's returned eax) and it reuses the key's `push ecx` slot; the two float zeros are pushed as immediates.
// CThing is modelled locally: rebuild/include/engine has no CThing.h and CThingCreatureBase.h pads the CThing bytes.
typedef unsigned short wchar_t;                      // VC7.1 without /Zc:wchar_t

class CTCBase;

class CWideString
{
public:
    void* PStringData;                               // +0x00
    CWideString(const wchar_t* text);                // 0x0099B6B0
    ~CWideString();                                  // 0x0099B510
};

enum EEmotePriority { EMOTE_PRIORITY_7 = 7 };

class CTCEmoteIcon                                   // interface type 0x45
{
public:
    void SetEmoteText(EEmotePriority priority, const CWideString& text, float scale, float time);   // 0x0071AC70
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing
{
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 UsingComponents bitset<274>
    CInterfaceMap m_interfaces;                      // +0x44 Components

    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing { };

namespace NCreatureAI
{
    void __fastcall StopSpeaking(CThingCreatureBase& creature);
}

void __fastcall NCreatureAI::StopSpeaking(CThingCreatureBase& creature)
{
    CTCEmoteIcon* emote;
    if (creature.GetInterface(0x45, &emote))
    {
        CWideString text(L"");
        emote->SetEmoteText(EMOTE_PRIORITY_7, text, 0.0f, 0.0f);
    }
}