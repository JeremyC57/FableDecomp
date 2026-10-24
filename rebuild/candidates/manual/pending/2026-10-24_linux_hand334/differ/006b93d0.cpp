// CFactionManager::GetPFaction(const CCharString& name) -- CRC the faction name and look it up in the hash-keyed faction map; null when absent
// Grade: DIFFER(ours v retail) -- 92v92: register swap only (retail keeps `this` in esi and the hash in edi, ours the reverse)
// Leaf: ?GetPFaction@CFactionManager@@QAEPAVCFaction@@ABUCCharString@@@Z
// Retail: Fable.exe @ 0x006B93D0, 92 bytes, `__thiscall`, `ret 4`. Label correction: the manifest's "Faction::FindFactionByHash"
// is Ego_r.pdb ?GetPFaction@CFactionManager@@QBEPAVCFaction@@ABVCCharString@@@Z (11 callers); CFactionManager::Factions is
// a CVectorMap<unsigned long, CCountedPointer<CFaction> > at +0 (Ego_r.pdb).
// Callees: CRC Calc(seed, data, length) 0x004014A0 (`__fastcall`), CVectorMap<unsigned long,CCountedPointer<CFaction>>::LowerBound
// 0x006B8AE0.
// Shape: the inline by-hash GetPFaction: the key copied into a local (homed in the dead `name` slot, the hash itself kept in a
// register for the `hash < it->first` test); the CRC arguments as `Data == 0 ? ... : ...` (null arm first).
// Tried: inline Find helper (92v92, more diffs), key copy + null-first ternaries, positive `it != Last` return (this file),
// a named outer hash local (no change). Next: a `const` method/LowerBound (the PDB is QBE) or a same-TU LowerBound body.
struct CCharStringData {
    const char*   Text;                     // +0x00
    unsigned long Length;                   // +0x04
};
struct CCharString {
    CCharStringData* PStringData;           // +0x00
};
unsigned long __fastcall CRCCalc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0

class CFaction;
template <class T> struct CCountedPointer {
    T* Data;
    T* Get() const { return Data; }
};
template <class A, class B> struct pair { A first; B second; };
struct CFactionMap {                        // CVectorMap<unsigned long, CCountedPointer<CFaction>, CKeyPairCompareLess<...> >
    typedef pair<unsigned long, CCountedPointer<CFaction> > value_type;
    value_type* First;                      // +0x00
    value_type* Last;                       // +0x04
    value_type* EndOfStorage;               // +0x08
    value_type* LowerBound(const unsigned long& key);   // 0x006B8AE0
    value_type* Find(const unsigned long& key)
    {
        value_type* it = LowerBound(key);
        if (it == Last || key < it->first)
            return Last;
        return it;
    }
};

class CFactionManager {
public:
    CFactionMap Factions;                   // +0x00
    CFaction* GetPFaction(unsigned long hash)
    {
        unsigned long key = hash;
        CFactionMap::value_type* it = Factions.LowerBound(key);
        if (it == Factions.Last || hash < it->first)
            it = Factions.Last;
        if (it != Factions.Last)
            return it->second.Get();
        return 0;
    }
    CFaction* GetPFaction(const CCharString& name);
};

CFaction* CFactionManager::GetPFaction(const CCharString& name)
{
    return GetPFaction(CRCCalc(0, name.PStringData == 0 ? "" : name.PStringData->Text, name.PStringData == 0 ? 0 : name.PStringData->Length));
}
