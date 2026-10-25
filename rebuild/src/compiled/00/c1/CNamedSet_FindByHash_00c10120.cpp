// CNamedSet::FindByHash(unsigned long hash) -- look an entry up by a precomputed name hash: forwards it by reference to the name index's keyed Find (virtual slot 17 of the index at +0x10) and returns the entry (or NULL)
// Leaf: ?FindByHash@CNamedSet@@QAEPAVCNamedEntry@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10120, 17 bytes: mov eax,[ecx+10h] ; add ecx,10h ; lea edx,[esp+4] ;
//   push edx ; call [eax+44h] ; ret 4
// Naming (sweep1, 2026-10-25): was landed as `NScript::CIsThingAlive::operator()` (a propagated manifest label; the
//   stand-in called it `C::W(int)` and dropped the result); descriptive (no PDB covers libpk). The landed
//   CNamedSet::Find(const CString&, long*) 0x00C101D0 looks names up by calling this same index (+0x10, slot 17) with
//   name.GetHash() (CString::GetHash 0x00C0DBB0, the Jenkins hash), so this finder takes the hash directly; its only
//   caller, the landed object-map lookup 0x00C196A0 (0x00C19726), uses it for key type 1 beside FindById 0x00C10140
//   (type 0) and Find by name 0x00C101D0.
class CNamedEntry;

struct CNamedIndex {                            // the name-hash index (as the landed CNamedSet::Find models it)
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16();
    virtual CNamedEntry* Find(const unsigned long& key);   // slot 17 (+0x44)
    void* m_pImpl;                              // +4
    unsigned char m_impl[8];                    // +8
};

class CNamedSet {
public:
    unsigned char _pad00[0x10];
    CNamedIndex m_index;                        // +0x10
    CNamedEntry* FindByHash(unsigned long hash);
};

CNamedEntry* CNamedSet::FindByHash(unsigned long hash)
{
    return m_index.Find(hash);
}
