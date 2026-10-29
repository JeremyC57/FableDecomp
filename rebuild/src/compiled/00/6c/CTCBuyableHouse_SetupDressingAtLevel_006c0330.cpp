// CTCBuyableHouse::SetupDressingAtLevel(long level) -- record the dress level and frame, then show each live "Dress..." thing in the house whose script-name digits after the prefix include the level (hide the rest; level -1 hides all)
// Leaf: SetupDressingAtLevel
// Retail 0x006C0330, 384 bytes, `ret 4`.
// Callees: the world-frame accessor 0x0049D870, CVectorMap::LowerBound 0x0040F020 (interface 0x21 thing list),
// CCharString(const char*, long) 0x0099EBF0 ("Dress" 0x0125D780), ~CCharString 0x0099EAE0, CBaseIntelligentPointer::GetPItem
// 0x00A01B50, _strnicmp 0x00F3FB90, CThing::SetInLimbo(bool) 0x004C8CF0, CCharString::GetSubString(ulong, ulong) const
// 0x0099EC70 (by-value result); "" 0x0129AAF4.
// Layout: engine/CTCBuyableHouse.h agrees with retail (CurrentDressLevel +0x1C, FrameToNextCheckIfWeNeedToCreateAnything +0x34);
// a local model adds the CTCBase ParentThing +4; interface-0x21 component list header +0x0C
// (landed CountMoneyBags 0x006C04D0 model); CThing script name +0x78, dead flag +0x91.
// Shape: the list head is a named local (cached in ebx, spilled once) and the prefix a named CCharString local for the walk; CCharString's inline GetPString/Length (null data =
// "" / 0) feed _strnicmp and GetSubString; the digit scan is an index loop re-testing Length() each pass.
extern "C" int __cdecl _strnicmp(const char* a, const char* b, unsigned int n);   // 0x00F3FB90
class CTCBase;
class CBaseObject;
class CCharStringData { public:
    char* m_pData;                                      // +0x00
    long  m_length;                                     // +0x04
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }   // "" = 0x0129AAF4
    long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    char operator[](long i) const { return GetPString()[i]; }
    CCharString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099EC70
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct SThingNode {
    SThingNode*                 next;
    SThingNode*                 prev;
    CIntelligentPointer<CThing> thing;                  // +0x08
};
struct CTCThingList_0x21 {
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                               // +0x0c
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad_0x4c[0x78 - 0x4c];
    CCharString   ScriptName;                           // +0x78
    unsigned char _pad_0x7c[0x91 - 0x7c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0 = dead)
    void SetInLimbo(bool limbo);                        // 0x004C8CF0
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CTCBuyableHouse {
public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad08[0x1c - 0x08];
    long          CurrentDressLevel;                    // +0x1c
    unsigned char _pad20[0x34 - 0x20];
    long          FrameToNextCheckIfWeNeedToCreateAnything;   // +0x34
    long GetWorldFrame();                               // 0x0049D870
    void SetupDressingAtLevel(long level);
};

void CTCBuyableHouse::SetupDressingAtLevel(long level)
{
    CurrentDressLevel = level;
    FrameToNextCheckIfWeNeedToCreateAnything = GetWorldFrame();
    CTCThingList_0x21* list;
    if (ParentThing->GetInterface(&list, 0x21)) {
        CCharString prefix("Dress");
        SThingNode* head = list->m_head;
        for (SThingNode* node = head->next; node != head; node = node->next) {
            CThing* thing = node->thing.Get();
            if (thing && !(thing->m_stateFlags & 1)) {
                CCharString& name = thing->ScriptName;
                if (_strnicmp(name.GetPString(), prefix.GetPString(), prefix.Length()) == 0) {
                    if (level == -1) {
                        thing->SetInLimbo(false);
                    } else {
                        CCharString levels = name.GetSubString(prefix.Length(), name.Length());
                        bool found = false;
                        for (long i = 0; i < levels.Length(); ++i) {
                            if ((char)(levels[i] - '0') == level) {
                                found = true;
                                break;
                            }
                        }
                        thing->SetInLimbo(!found);
                    }
                }
            }
        }
    }
}