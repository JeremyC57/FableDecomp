// CMeshDataBank::GetEnumerationType(unsigned long type) -- the animation enumeration type name for the animation entry types (6..10) when one is set, else the bank file's default
// Leaf: ?GetEnumerationType@CMeshDataBank@@MBE?AVCCharString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A23D80, 90 bytes, hidden CCharString result, `ret 8`. Name: Ego_r/FableWin PDB
//   ?GetEnumerationType@CMeshDataBank@@MBE?AVCCharString@@K@Z (protected virtual; catalog row agrees).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, CBankFile::GetEnumerationType(ulong) const @
//   0x009CBD10 (the qualified base call; `this` stays in ecx, result pointer pushed last).
// Layout: AnimationEnumType (CCharString) at +0x3BC (engine/CMeshDataBank.h); CCharStringData = {CBasicString<char>
//   Data (PData +0, StringLength +4), NoRefs}. Local polymorphic model: the generated header flattens the bank-file bases
//   into a vptr word, so the virtual and its base cannot be declared on it (NOTES.md "Header debt").
// Shape: an inline IsEmpty (`PStringData == 0 || length == 0`) then the type tests in retail's compare order 6, 10, 7, 8, 9.
class CBasicStringChar {
public:
    char*         PData;           // +0x00
    unsigned long StringLength;    // +0x04
};
class CCharStringData {
public:
    CBasicStringChar Data;         // +0x00
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);   // 0x0099EC30
    ~CCharString();
    bool IsEmpty() const { return PStringData == 0 || PStringData->Data.StringLength == 0; }
};

class CBankFile {
public:
    virtual ~CBankFile();
protected:
    virtual CCharString GetEnumerationType(unsigned long type) const;   // 0x009CBD10
    unsigned char _body[0x110 - 4];
};
class CBankFileAsync : public CBankFile {
protected:
    unsigned char _async[0x164 - 0x110];
};

class CMeshDataBank : public CBankFileAsync {
protected:
    unsigned char _pad164[0x3bc - 0x164];
    CCharString   AnimationEnumType;   // +0x3bc
    virtual CCharString GetEnumerationType(unsigned long type) const;
};

CCharString CMeshDataBank::GetEnumerationType(unsigned long type) const
{
    if (!AnimationEnumType.IsEmpty()) {
        if (type == 6 || type == 10 || type == 7 || type == 8 || type == 9)
            return AnimationEnumType;
    }
    return CBankFileAsync::GetEnumerationType(type);
}