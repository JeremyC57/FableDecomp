// CBankFile::GetEnumerationType(unsigned long entry) const -- the base implementation: whatever the entry, return a copy of the bank header's enumeration-type string (HeaderEnumerationType)
// Leaf: ?GetEnumerationType@CBankFile@@MBE?AVCCharString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CBD10, 25 bytes, thiscall `ret 8` (the hidden CCharString return slot and the
//   unused entry index), returns the slot. Callee: CCharString copy constructor 0x0099EC30.
// Identity: vtable slot 19 (rtti-vtables.tsv: CWADBank 0x012442DC slot 19 = 0x009CBD10, inherited from CBankFile), the
//   slot the RTTI-named sibling banks fill with their GetEnumerationType overrides (CMeshDataBank 0x00A23D80,
//   CLipSyncDataBank 0x00A427E0, CStreamingFontDataBank 0x00A5EF10); Ego_r/FableWin.pdb name set
//   ?GetEnumerationType@CBankFile@@MBE?AVCCharString@@... (protected virtual const); the overrides' rtti_port names
//   ?GetEnumerationType@CMeshDataBank@@MBE?AVCCharString@@K@Z etc. take an unsigned long and forward it here. The copied member at retail +0xF0
//   is Ego_r.pdb CBankFile::HeaderEnumerationType (+0x114 in the donor, whose three Dinkumware maps / CVectorMap /
//   CArrays are larger); BankHandle (the public non-virtual GetBankHandle) sits far lower (+0x98 donor). Was landed as
//   "GetBankHandle(int unused)" whose int parameter was in fact this unused entry index.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
};

class CBankFile {
protected:
    virtual CCharString GetEnumerationType(unsigned long entry) const;
public:
    unsigned char _pad_0x04[0xF0 - 4];
    CCharString HeaderEnumerationType;          // +0xF0
};

CCharString CBankFile::GetEnumerationType(unsigned long /*entry*/) const
{
    return HeaderEnumerationType;
}
