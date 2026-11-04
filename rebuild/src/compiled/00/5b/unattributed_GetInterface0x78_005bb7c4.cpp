// CThingComponents_005bb7c4::GetInterface_0x78(CTCBase**) -- interface lookup for type 0x78: bitset test, then the CVectorMap lower bound
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad1[0x23] removed; map pointer made const (the row grades the const LowerBound).
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BB7C4, oracle length 75 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push ecx ; push esi ; mov esi,ecx ; push 0x78 ; lea ecx,[esi+0x20] ; call bitset::test ; test al,al ; je F ; lea eax,[ebp-4] ; add esi,0x44 ; push eax ; mov ecx,esi ; mov dword ptr [ebp-4],0x78 ; call CVectorMap::LowerBound ; mov esi,[esi+4] ; cmp eax,esi ; je E ; cmp dword ptr [eax],0x78 ; jle G ; E: mov eax,esi ; G: mov eax,[eax+4] ; mov ecx,[ebp+8] ; mov [ecx],eax ; mov al,1 ; jmp R ; F: xor al,al ; R: pop esi ; leave ; ret 4
// Per-interface-type lookup on a thing's component table: the presence bitset at +0x20
// (test @ 0x00410DE0) is consulted first, then the sorted vector map at +0x44 {first, last}
// of {type, component} pairs is searched with LowerBound @ 0x004365B0 (key by address);
// a miss (end, or a larger key) yields the end entry, whose value is handed out regardless. The
// frame pointer and `leave` come from this TU's settings: `#pragma optimize("y",off)` +
// `("s",on)`. The constant 0x78 is the interface type. One of 40 bodies.
#pragma optimize("y", off)              // frame pointer kept (`leave`), size-optimised TU
#pragma optimize("s", on)

class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CThingComponents_005bb7c4 {
    unsigned char _pad0[0x20];
    std::bitset<274> m_present;               // +0x20
    CInterfaceMap m_interfaces;            // +0x44
    bool GetInterface_0x78(CTCBase** out);
};

bool CThingComponents_005bb7c4::GetInterface_0x78(CTCBase** out)
{
    if (m_present.test(0x78)) {
        ETCInterfaceType type = (ETCInterfaceType)(0x78);
        const CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once (`add esi`)
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || 0x78 < it->first)
            it = map->end();
        *out = it->second;
        return true;
    }
    return false;
}