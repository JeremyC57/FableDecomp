#pragma optimize("s",on)

struct InterfaceBits
{
    unsigned long words[9];
    unsigned long Test(unsigned long bit);
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public: typedef std::pair<ETCInterfaceType, CTCBase*> SEntry; SEntry* begin; SEntry* end; SEntry* LowerBound(const ETCInterfaceType& key); };
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > InterfaceMap;

struct InterfaceContainer
{
    unsigned char unused00[0x20];
    InterfaceBits bits;
    InterfaceMap map;

    bool CTC_FindInterface_0x11(int* output);
};

bool InterfaceContainer::CTC_FindInterface_0x11(int* output)
{
    unsigned long tested = bits.Test(0x11);
    bool result;
    if ((unsigned char)tested != 0) {
        ETCInterfaceType key = (ETCInterfaceType)(0x11);
        InterfaceMap* current_map = &(map);
        InterfaceMap::SEntry* entry = current_map->LowerBound(key);
        InterfaceMap::SEntry* end = current_map->end;
        if (entry == end || entry->first > 0x11) {
            entry = end;
        }
        *output = (int)entry->second;
        result = true;
    } else {
        result = false;
    }
    return result;
}