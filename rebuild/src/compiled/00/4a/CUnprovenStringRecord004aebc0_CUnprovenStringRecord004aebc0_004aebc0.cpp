// CUnprovenStringRecord_004aebc0::CUnprovenStringRecord_004aebc0() -- default-construct the record's two strings (a CCharString at +0, a CCharString at +4)
// Leaf: ??0CUnprovenStringRecord_004aebc0@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AEBC0, 20 bytes, thiscall, plain `ret`, returns this.
// Identity (descriptive): no caller, jump or address reference anywhere in the image; the bytes say only "a record whose first
//   member is a CCharString and second a CCharString, both default-constructed" (the catalog's QuestData label is unsupported).
// Callees: CCharString::CCharString() 0x0099E4B0, CCharString::CCharString() 0x0099E4B0.
// Shape (family K110, representative 0x004AEBC0): an out-of-line constructor whose two member default constructors are calls.
class CCharStringData;
class CCharString {                                      // 4 bytes: PStringData
public:
    CCharString();                                       // 0x0099E4B0
    CCharStringData* PStringData;
};
class CUnprovenStringRecord_004aebc0 {                                    // placeholder for the unnamed record (8 bytes)
public:
    CUnprovenStringRecord_004aebc0();
    CCharString First;                                   // +0x00
    CCharString Second;                                       // +0x04
};
CUnprovenStringRecord_004aebc0::CUnprovenStringRecord_004aebc0()
{
}