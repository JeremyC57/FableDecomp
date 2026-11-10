// CASoundManager::GetBank(long id) -- look a sound bank up by id in the registry's sorted id map; null when absent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00991840, 53 bytes, `ret 4`.
// Leaf: ?GetBank@CASoundManager@@QAEPAVCASoundBank@@J@Z
// Identity: a CASoundManager member (RTTI vtable 0x0129A574; the base the live CSoundManagerLH 0x0129D2C4 derives from, whose
//        AddSound 0x00A3F250 calls it on its unadjusted `this`). Its 12 callers (e.g. the landed CSubtitledMessage ctors
//        0x004CDB70/0x004CDBD0) call it on PSoundManager 0x013B8394 with a bank id and use the result as a CASoundBank*. The sound
//        module has no PDB: the method name is descriptive (engine/CASoundManager.h). The catalog's "MessageMap::FindMessageByID"
//        and the earlier CSoundBankRegistry / CSoundSystem class spellings were wrong (Codex pass 15 name fix).
// Callees: the map's LowerBound(const long&) const @ 0x00992290 (a CVectorMap<long, T> instantiation; manifest names the
//          CIntelligentPointer<CThing> one, a byte twin).
// Layout: the id map (sorted {id, bank} pairs, a 12-byte STLport vector) at +0x30.
// Shape: the inline CVectorMap find (lower bound, then `it == end || id < it->first` -> end) and `if (it != end()) return it->second;` (found arm first) that
//        re-reads end (`cmp eax,[edi+34h]`); the key is the argument's own slot (`lea eax,[esp+0Ch]`).
class CASoundBank;
template <class K, class V> class CVectorMap { public:
    struct SEntry { K first; V second; };
    SEntry* _M_start;
    SEntry* _M_finish;
    SEntry* _M_end_of_storage;
    const SEntry* LowerBound(const K& key) const;    // 0x00992290
    const SEntry* end() const { return _M_finish; }
    const SEntry* find(const K& key) const
    {
        const SEntry* it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
};
class CASoundManager { public:
    unsigned char                    _pad00[0x30];
    CVectorMap<long, CASoundBank*>   Banks;          // +0x30
    CASoundBank* GetBank(long id);
};
CASoundBank* CASoundManager::GetBank(long id)
{
    const CVectorMap<long, CASoundBank*>::SEntry* it = Banks.find(id);
    if (it != Banks.end())
        return it->second;
    return 0;
}