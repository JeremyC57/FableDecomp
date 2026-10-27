// CUnprovenRecord_006a4b30::CUnprovenRecord_006a4b30() -- default constructor: construct the CCharString member, then clear the two flags at +0x10/+0x11
// Leaf: ??0CUnprovenRecord_006a4b30@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006a4b30, 20 bytes, thiscall, returns this, plain `ret`.
// Identity (descriptive): its only caller CThingPhysical::DrawPreparePrimitivesForRendering 0x006A64D0 constructs it as a stack local; its own bytes construct a CCharString at +0 and clear the bytes at +0x10 and +0x11 (two bools), leaving +4..+0x0F to trivially constructed members; the record type is unproven (the catalog's CRegister is 12 bytes).
// Callee: CCharString::CCharString() 0x0099E4B0.
// Shape (family L038): push esi / mov esi,ecx / call the string ctor / one zeroed register stored twice / mov eax,esi.
class CCharString {
public:
    CCharString();                                        // 0x0099E4B0
    ~CCharString();
private:
    void* PStringData;                                    // +0x00
};
class CUnprovenRecord_006a4b30 {                                              // descriptive: the record type is unproven
public:
    CCharString Name;                                     // +0x00
    float Values[3];                                      // +0x04 (trivially constructed)
    bool Flag0;                                           // +0x10
    bool Flag1;                                           // +0x11
    CUnprovenRecord_006a4b30();
};
CUnprovenRecord_006a4b30::CUnprovenRecord_006a4b30()
    : Flag0(false), Flag1(false)
{
}