// CUnprovenRecord_007aba60::CUnprovenRecord_007aba60() -- default constructor: construct the CCharString member, zero the bool member
// Leaf: ??0CUnprovenRecord_007aba60@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007aba60, 16 bytes, thiscall, returns this, plain `ret`.
// Identity (descriptive): no call, jump or data reference reaches it (unreferenced COMDAT); its own bytes construct a CCharString at +0 (??0CCharString@@QAE@XZ 0x0099E4B0) and store a zero byte at +4, so the record is {CCharString, bool}; Ego_r.pdb has many 8-byte {CCharString, bool} records (CCounterEntry, CUserStatVariable, CAtmosGroup, pairs) and nothing here picks one.
// Callees: CCharString::CCharString() 0x0099E4B0.
// Shape (family L029): push esi / mov esi,ecx / call the string ctor / mov byte ptr [esi+4],0 / mov eax,esi.
class CCharString {
public:
    CCharString();                                        // 0x0099E4B0
    ~CCharString();
private:
    void* PData;                                          // +0x00
};
class CUnprovenRecord_007aba60 {                                              // descriptive: the record type is unproven
public:
    CCharString Name;                                     // +0x00
    bool Value;                                          // +0x04
    CUnprovenRecord_007aba60();
};
CUnprovenRecord_007aba60::CUnprovenRecord_007aba60()
    : Value(false)
{
}