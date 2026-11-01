// CAmbientScene::Lookup(unsigned long id) -- the sequence item of a sound id, from the item list at +0xc (a tail call)
// Leaf: ?Lookup@CAmbientScene@@QAEPAUCSequenceItem@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36C70, 7 bytes (thiscall; VC7.1 tail-calls the forward: `mov ecx,[ecx+0Ch] ; jmp rel32`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- CAmbientEvent::Trigger 0x00C61D10 passes CPKArray::GetRandom's id and copies two
//   words from the result (the position record at +4 of the item); the callee is the landed ?GetItem@CSequenceList@@QBEPAUCSequenceItem@@K@Z
//   0x00C463C0 (the model of the landed 0x00C36EA0). Was a `T::Run` forwarder over Run_00c463c0 (the catalog's STLport _Cons_val label is propagated).
#pragma pack(push,1)
struct CSequenceItem;
struct CSequenceList {
    CSequenceItem* GetItem(unsigned long index) const;   // 0x00C463C0
};
class CAmbientScene { public:
    unsigned char  _pad[0xc];
    CSequenceList* Steps;                                // +0x0c
    CSequenceItem* Lookup(unsigned long id);
};
#pragma pack(pop)
CSequenceItem* CAmbientScene::Lookup(unsigned long id) { return Steps->GetItem(id); }
