// CTCHeroCentreDoorMarker::StatGetClassTypeName() -- static: the class-type name "CTCHeroCentreDoorMarker" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCHeroCentreDoorMarker@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D465D: push esi; push -1; push "CTCHeroCentreDoorMarker"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123beb8 = "CTCHeroCentreDoorMarker" = the class name; PDB name set ?StatGetClassTypeName@CTCHeroCentreDoorMarker@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d465d::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCHeroCentreDoorMarker
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCHeroCentreDoorMarker::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123BEB8, -1);   // "CTCHeroCentreDoorMarker"
}
