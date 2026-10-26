// CScriptManager::FindScriptBinding(const CCharString& name) -- nonzero when the script-binding map holds an entry whose name equals `name`
// Leaf: FindScriptBinding
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7D60, length 61 bytes.
// Callees: CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, CCompareString<..>>::LowerBound
// @ 0x00CBA1F0 (on the map at +0x18, returns the element pointer in eax), CCharString::FastCompare
// @ 0x009E6530 (key.FastCompare(element name): the map's "less" -- true when the key sorts first).
// Shape: the map's inline Find is `it = LowerBound(key); last = end(); return (it == last ||
// key < *it) ? last : it;` -- `end()` read ONCE into a local right after the call (`mov esi,[esi+4]`,
// reusing the map-address register; reading end() at each use compares against memory and forks the
// tails); the caller then compares with a fresh `end()` (`mov ecx,[ebx+1Ch]`). `xor eax,eax ; setne al`
// = a long result (`return find(name) != end();`).
// No engine header exists for CScriptManager; the map is modelled with its retail layout (12-byte
// vector at +0x18, element = SScriptBindingPair {CCharString name; CCountedPointer binding}, 12 bytes).
class CCPPointerInfo;
class CEntityScriptBindingBase;

class CCharString { public:
    char* m_pData;
    bool FastCompare(const CCharString& other) const;     // 0x009E6530
};

struct SScriptBindingPair {
    CCharString               first;
    CEntityScriptBindingBase* second_Data;
    CCPPointerInfo*           second_Info;
};

struct CScriptBindingMap {
    SScriptBindingPair* m_first;
    SScriptBindingPair* m_last;
    SScriptBindingPair* m_end;
    unsigned char       Compare;
    bool                Dirty;
    const SScriptBindingPair* LowerBound(const CCharString& key) const;   // 0x00CBA1F0
    const SScriptBindingPair* end() const { return m_last; }
    const SScriptBindingPair* Find(const CCharString& key) const
    {
        const SScriptBindingPair* it = LowerBound(key);
        const SScriptBindingPair* last = end();
        return (it == last || key.FastCompare(it->first)) ? last : it;
    }
};

struct CScriptManager {
    unsigned char     _pad00[0x18];
    CScriptBindingMap m_bindings;                    // +0x18
    long FindScriptBinding(const CCharString& name) const;
};

long CScriptManager::FindScriptBinding(const CCharString& name) const
{
    return m_bindings.Find(name) != m_bindings.end();
}