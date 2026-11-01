// Naming wave 18: the interface-presence stand-in InterfaceBits is std::bitset<274> (STLport 4.6); its Test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
#pragma optimize("s",on)

namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}

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
    std::bitset<274> bits;
    InterfaceMap map;

    bool CTC_FindInterface_0x11(int* output);
};

bool InterfaceContainer::CTC_FindInterface_0x11(int* output)
{
    unsigned long tested = bits.test(0x11);
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