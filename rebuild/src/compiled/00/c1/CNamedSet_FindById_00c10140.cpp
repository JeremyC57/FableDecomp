// CNamedSet::FindById(unsigned long id) -- look an entry up by its number: forwards the id by reference to the set's own keyed Find (virtual slot 17) and returns the entry (or NULL)
// Leaf: ?FindById@CNamedSet@@QAEPAVCNamedEntry@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10140, 13 bytes: mov eax,[ecx] ; lea edx,[esp+4] ; push edx ;
//   call [eax+44h] ; ret 4
// Naming (sweep1, 2026-10-25): was landed as CDataOutputStream::WriteCBYTE (a propagated manifest label; the stand-in
//   called it `C::W(int)` and dropped the result); descriptive (no PDB covers libpk). It is one of the three finders of
//   the libpk named set beside the landed CNamedSet::Find(const CString&, long*) 0x00C101D0 (by name, through the name
//   index at +0x10) and FindByHash 0x00C10120 (the name index's own slot-17 Find): the landed object-map lookup
//   0x00C196A0 picks FindByName / this one (key type 0) / FindByHash (key type 1); the landed CItemAction ctor
//   0x00C1EE60 and CBankProcessor::AddNumberTriggers 0x00C2E4D0 (FINDINGS chunk 436: "registry Find(number)") use it
//   to find a trigger by number.
class CNamedEntry;

class CNamedSet {
public:
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16();
    virtual CNamedEntry* Find(const unsigned long& key);   // slot 17 (+0x44)
    CNamedEntry* FindById(unsigned long id);
};

CNamedEntry* CNamedSet::FindById(unsigned long id)
{
    return Find(id);
}
