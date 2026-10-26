// CFactionManager::GetPFaction(const CCharString& name) -- CRC the faction name and look it up in the hash-keyed faction map; null when absent
// Grade: RELOCATION_MATCH (lane re-grade 2026-10-24; an earlier draft differed only by an esi/edi swap, notes below kept for history)
// Leaf: ?GetPFaction@CFactionManager@@QAEPAVCFaction@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x006B93D0, 92 bytes, `__fastcall`, `ret 4`. Label correction: the manifest's "Faction::FindFactionByHash"
// is Ego_r.pdb ?GetPFaction@CFactionManager@@QBEPAVCFaction@@ABVCCharString@@@Z (11 callers); CFactionManager::Factions is
// a CVectorMap<unsigned long, CCountedPointer<CFaction> > at +0 (Ego_r.pdb).
// Callees: CRC Calc(seed, data, length) 0x004014A0 (`__fastcall`), CVectorMap<unsigned long,CCountedPointer<CFaction>>::LowerBound
// 0x006B8AE0.
// Shape: the inline by-hash GetPFaction: the key copied into a local (homed in the dead `name` slot, the hash itself kept in a
// register for the `hash < it->first` test); the CRC arguments as `Data == 0 ? ... : ...` (null arm first).
// Tried: inline Find helper (92v92, more diffs), key copy + null-first ternaries, positive `it != Last` return (this file),
// a named outer hash local (no change). Next: a `const` method/LowerBound (the PDB is QBE) or a same-TU LowerBound body.
class CCharStringData { public:
    const char*   Text;                     // +0x00
    unsigned long Length;                   // +0x04
};
class CCharString { public:
    CCharStringData* PStringData;           // +0x00
};
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0

class CFaction;
template <class T> class CCountedPointer { public:
    T* Data;
    T* Get() const { return Data; }
};
template <class A, class B> struct pair { A first; B second; };
struct CFactionMap {                        // CVectorMap<unsigned long, CCountedPointer<CFaction>, CKeyPairCompareLess<...> >
    typedef pair<unsigned long, CCountedPointer<CFaction> > value_type;
    value_type* First;                      // +0x00
    value_type* Last;                       // +0x04
    value_type* EndOfStorage;               // +0x08
    __declspec(noinline) value_type* LowerBound(const unsigned long& key);   // 0x006B8AE0 (same-TU stand-in)
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
        CFactionMap::value_type* it = Factions.Find(hash);
        if (it != Factions.Last)
            return it->second.Get();
        return 0;
    }
    CFaction* GetPFaction(const CCharString& name);
};

CFaction* CFactionManager::GetPFaction(const CCharString& name)
{
    return GetPFaction(CCRC::Calc(0, name.PStringData == 0 ? "" : name.PStringData->Text, name.PStringData == 0 ? 0 : name.PStringData->Length));
}

CFactionMap::value_type* CFactionMap::LowerBound(const unsigned long& key)
{
    value_type* first = First;
    long count = Last - First;
    while (count > 0) {
        long half = count >> 1;
        if (first[half].first < key) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}