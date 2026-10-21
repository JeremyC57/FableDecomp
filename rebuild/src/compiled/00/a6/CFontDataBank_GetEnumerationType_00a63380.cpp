// CFontDataBank::GetEnumerationType(unsigned long) const -- defers to CBankFile's enumeration-type name
// Leaf: GetEnumerationType@CFontDataBank
// Retail: Fable.exe @ 0x00A63380, 22 bytes, `ret 8`; Ego_r.pdb ?GetEnumerationType@CFontDataBank@@MBE?AVCCharString@@K@Z (protected
// virtual, referenced only from its vtable slot at 0x0129DAD0). Calls the base CBankFile::GetEnumerationType 0x009CBD10 non-virtually
// with the hidden return slot passed straight through (RTTI: CFontDataBank : CBankFileAsync : CBankFile).
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
class CFontDataBank : public CBankFileAsync {
protected:
    virtual CCharString GetEnumerationType(unsigned long type) const;
};
CCharString CFontDataBank::GetEnumerationType(unsigned long type) const
{
    return CBankFile::GetEnumerationType(type);
}