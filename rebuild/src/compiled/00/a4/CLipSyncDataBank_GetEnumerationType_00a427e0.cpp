// CLipSyncDataBank::GetEnumerationType(unsigned long) const -- defers to CBankFile's enumeration-type name
// Leaf: GetEnumerationType@CLipSyncDataBank
// Retail: Fable.exe @ 0x00A427E0, 22 bytes, `ret 8`; Ego_r.pdb ?GetEnumerationType@CLipSyncDataBank@@MBE?AVCCharString@@K@Z (protected
// virtual, referenced only from its vtable slot at 0x0129D420). Calls the base CBankFile::GetEnumerationType 0x009CBD10 non-virtually
// with the hidden return slot passed straight through (RTTI: CLipSyncDataBank : CBankFileAsync : CBankFile).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);
    ~CCharString();
};
class CBankFile {
protected:
    virtual CCharString GetEnumerationType(unsigned long type) const;   // 0x009CBD10
};
class CBankFileAsync : public CBankFile { };
class CLipSyncDataBank : public CBankFileAsync {
protected:
    virtual CCharString GetEnumerationType(unsigned long type) const;
};
CCharString CLipSyncDataBank::GetEnumerationType(unsigned long type) const
{
    return CBankFile::GetEnumerationType(type);
}