// CTCPerceiveThing::StatGetClassTypeName() -- static: the class-type name "CTCPerceiveThing" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCPerceiveThing@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D35B9: push esi; push -1; push "CTCPerceiveThing"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123aa84 = "CTCPerceiveThing" = the class name; PDB name set ?StatGetClassTypeName@CTCPerceiveThing@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d35b9::build).
#define CTCPerceiveThing CTCPerceiveThingLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCPerceiveThing : public CTCPerceiveThingLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCPerceiveThing::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123AA84, -1);   // "CTCPerceiveThing"
}
