// CThingComponents_0065662b::GetInterface_0x90(CTCBase**) -- interface lookup for type 0x90: bitset test, then the CVectorMap lower bound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065662B, oracle length 76 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push ecx ; push esi ; mov esi,ecx ; push 0x90 ; lea ecx,[esi+0x20] ; call bitset::test ; test al,al ; je F ; lea eax,[ebp-4] ; add esi,0x44 ; push eax ; mov ecx,esi ; mov dword ptr [ebp-4],0x90 ; call CVectorMap::LowerBound ; mov esi,[esi+4] ; cmp eax,esi ; je E ; cmp dword ptr [eax],0x90 ; jle G ; E: mov eax,esi ; G: mov eax,[eax+4] ; mov ecx,[ebp+8] ; mov [ecx],eax ; mov al,1 ; jmp R ; F: xor al,al ; R: pop esi ; leave ; ret 4
// Per-interface-type lookup on a thing's component table: the presence bitset at +0x20
// (test @ 0x00410DE0) is consulted first, then the sorted vector map at +0x44 {first, last}
// of {type, component} pairs is searched with LowerBound @ 0x0040F020 (key by address);
// a miss (end, or a larger key) yields the end entry, whose value is handed out regardless. The
// frame pointer and `leave` come from this TU's settings: `#pragma optimize("y",off)` +
// `("s",on)`. The constant 0x90 is the interface type. One of 25 bodies.
#pragma optimize("y", off)              // frame pointer kept (`leave`), size-optimised TU
#pragma optimize("s", on)

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                   // 0x00410DE0 (std::bitset::test)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type); // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CThingComponents_0065662b {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;            // +0x44
    bool GetInterface_0x90(CTCBase** out);
};

bool CThingComponents_0065662b::GetInterface_0x90(CTCBase** out)
{
    if (m_present.test(0x90)) {
        ETCInterfaceType type = (ETCInterfaceType)(0x90);
        CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once (`add esi`)
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || 0x90 < it->first)
            it = map->last;
        *out = it->second;
        return true;
    }
    return false;
}