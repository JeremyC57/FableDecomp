// CObjectMap::GetScope(SScopeRecord* record) -- the named set a lookup runs in: the record's own set (at its +0xc) when the key carries a scope record, else the map owner's default set (owner +0x3c)
// Leaf: ?GetScope@CObjectMap@@QAEPAVCNamedSet@@PAUSScopeRecord@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19680, 20 bytes (`mov eax,[ecx+4] ; mov ecx,[esp+4] ; add eax,3Ch ;
//   test ecx,ecx ; je ; lea eax,[ecx+0Ch] ; ret 4`), thiscall.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 433 "0xC19680 _Cons_val = object map GetScope"): its four callers are
//   the libpk object map's lookups -- CObjectMap::Find 0x00C196A0 (call 0x00C196BE; agent433: "GetScope @ 0x00C19680
//   (same TU: key ? key + 0xc : m_4 + 0x3c)") and its siblings at 0x00C19CE8 / 0x00C19D58 / 0x00C19DC8 -- which then
//   search the returned set by name or through CNamedSet::FindById 0x00C10140 / FindByHash 0x00C10120. Descriptive
//   names (no PDB covers libpk); the map class is agent433's CObjectMap (landed address-suffixed).
// Label correction: the landed row was ConsThis::Cons over a padding struct (catalog "std::_Cons_val<allocator<pair<
//   EClothingSuitPart const,long> >, ...>", a propagated game label).
class CNamedSet;                                // a libpk named set (FindById / FindByHash)

struct SScopeRecord {                           // a scope record: the set sits at +0xc
    unsigned char _pad[0xc];
    unsigned char m_set[4];                     // +0x0c
};

struct CObjectMapOwner {
    unsigned char _pad[0x3c];
    unsigned char m_defaultSet[4];              // +0x3c
};

class CObjectMap {
public:
    long             m_0;
    CObjectMapOwner* m_pOwner;                  // +0x04
    CNamedSet* GetScope(SScopeRecord* record);
};

CNamedSet* CObjectMap::GetScope(SScopeRecord* record)
{
    CNamedSet* scope = (CNamedSet*)m_pOwner->m_defaultSet;
    if (record)
        scope = (CNamedSet*)record->m_set;
    return scope;
}
