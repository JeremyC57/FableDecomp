// CFactionManager::GetPFaction(unsigned long hash) const -- look a faction up by its name hash in the faction vector map; null when absent
// Leaf: ?GetPFaction@CFactionManager@@QBEPAVCFaction@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B93A0, 45 bytes (+ int3 padding), `ret 4` (thiscall).
// Callers: one site, 0x006DF156.
// Callees: CVectorMap<unsigned long, CCountedPointer<CFaction> >::LowerBound(const unsigned long&) const @ 0x006B8AE0.
// Identity: Ego_r.pdb ?GetPFaction@CFactionManager@@QBEPAVCFaction@@K@Z (the by-hash overload; the by-name overload is the
//   landed 0x006B93D0, which inlines this one). CFactionManager::Factions is a CVectorMap<unsigned long,
//   CCountedPointer<CFaction> > at +0 (Ego_r.pdb; retail 12-byte STLport vector: First +0, Last +4).
// Shape: the inline CVectorMap::Find `it = LowerBound(key); if (it == Last || key < it->first) it = Last;` (the `Last`
//   load kept in esi, the key re-read from its argument slot), then `it != Last ? it->second.Data : 0`.
// Label correction: the catalog `CVectorMap_Lookup` is descriptive.
class CFaction;

template <class T> struct CCountedPointer {
    T*    Data;                                         // +0x00
    void* Info;                                         // +0x04
    T* Get() const { return Data; }
};

template <class A, class B> struct pair { A first; B second; };

struct CFactionMap {                                    // CVectorMap<unsigned long, CCountedPointer<CFaction>, CKeyPairCompareLess<..> >
    typedef pair<unsigned long, CCountedPointer<CFaction> > value_type;
    value_type* First;                                  // +0x00
    value_type* Last;                                   // +0x04
    value_type* EndOfStorage;                           // +0x08
    const value_type* LowerBound(const unsigned long& key) const;   // 0x006B8AE0
    const value_type* Find(const unsigned long& key) const
    {
        const value_type* it = LowerBound(key);
        if (it == Last || key < it->first)
            return Last;
        return it;
    }
};

class CFactionManager {
public:
    CFactionMap Factions;                               // +0x00
    CFaction* GetPFaction(unsigned long hash) const;
};

CFaction* CFactionManager::GetPFaction(unsigned long hash) const
{
    const CFactionMap::value_type* it = Factions.Find(hash);
    if (it != Factions.Last)
        return it->second.Get();
    return 0;
}