// CUnprovenStringRecord_00526f50::CUnprovenStringRecord_00526f50() -- default-construct the record's two strings (a CCharString at +0, a CWideString at +4)
// Leaf: ??0CUnprovenStringRecord_00526f50@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00526F50, 20 bytes, thiscall, plain `ret`, returns this.
// Identity (descriptive): no caller, jump or address reference anywhere in the image; the bytes say only "a record whose first
//   member is a CCharString and second a CWideString, both default-constructed" (the catalog's QuestData label is unsupported).
// Callees: CCharString::CCharString() 0x0099E4B0, CWideString::CWideString() 0x0099AED0.
// Shape (family K110, representative 0x004AEBC0): an out-of-line constructor whose two member default constructors are calls.
class CCharStringData;
class CCharString {                                      // 4 bytes: PStringData
public:
    CCharString();                                       // 0x0099E4B0
    CCharStringData* PStringData;
};
class CWideStringData;
class CWideString {                                      // 4 bytes: PStringData
public:
    CWideString();                                       // 0x0099AED0
    CWideStringData* PStringData;
};
class CUnprovenStringRecord_00526f50 {                                    // placeholder for the unnamed record (8 bytes)
public:
    CUnprovenStringRecord_00526f50();
    CCharString First;                                   // +0x00
    CWideString Second;                                       // +0x04
};
CUnprovenStringRecord_00526f50::CUnprovenStringRecord_00526f50()
{
}