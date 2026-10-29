// NCreatureAI::IsSpeaking(const CThingCreatureBase&) -- true while the creature's talk component is talking, or an active non-replaceable speech emote icon is showing
// Leaf: ?IsSpeaking@
// Retail 0x008644E0, 0x9B bytes (+ int3 padding), __fastcall (creature in ecx). PDB (Ego_r):
// ?IsSpeaking@NCreatureAI@@YI_NABVCThingCreatureBase@@@Z. Ten callers (0x007B36BF, 0x0094BCFE, 0x00980CE5, ...).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (twice), CTCTalk::IsTalking() const @ 0x006E28A0.
// Interface types (GetInterfaceType, vtable slot 21): 0x45 = CTCEmoteIcon (bitset word 2 bit 5, `test al,20h` on +0x28),
// 0x0F = CTCTalk (word 0 bit 15, `mov eax,[edi+20h] ; test ah,ah ; jns`). CTCEmoteIcon (Ego_r): ActiveTimer +0x0C,
// EmoteType +0x1C, Replaceable +0x25.
// Shape: the emote test sets a bool local (`xor bl,bl` ... `mov bl,1`); the emote lookup is the __forceinline GetInterface
// out-pointer template (component kept in eax for the member reads), the talk lookup the HasTC + FindEntry form (component
// loaded straight into ecx for the call). `return talking || emoting;` is an int-valued `||`: retail returns
// `mov eax,1` / `xor eax,eax`, not a byte.
// CThing is modelled locally: rebuild/include/engine has no CThing.h and CThingCreatureBase.h pads the CThing bytes.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCEmoteIcon                                   // interface type 0x45
{
public:
    unsigned char _pad0[0x0c];
    long          ActiveTimer;                       // +0x0C
    unsigned char _pad10[0x1c - 0x10];
    long          EmoteType;                         // +0x1C EEmoteType
    unsigned char _pad20[0x25 - 0x20];
    bool          Replaceable;                       // +0x25
};

class CTCTalk                                        // interface type 0x0F
{
public:
    bool IsTalking() const;                          // 0x006E28A0
};

class CThing
{
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 UsingComponents bitset<274>
    CInterfaceMap m_interfaces;                      // +0x44 Components

    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CInterfaceMap::SEntry* FindEntry(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        const CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->first <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
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
    bool __fastcall IsSpeaking(const CThingCreatureBase& creature);
}

bool __fastcall NCreatureAI::IsSpeaking(const CThingCreatureBase& creature)
{
    bool emoting = false;
    const CTCEmoteIcon* emote;
    if (creature.GetInterface(0x45, &emote))
    {
        if (!emote->Replaceable && emote->ActiveTimer != 0 && emote->EmoteType == 0)
            emoting = true;
    }
    return (creature.HasTC(0x0F) && ((const CTCTalk*)creature.FindEntry(0x0F)->second)->IsTalking()) || emoting;
}