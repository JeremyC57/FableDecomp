// CAIStateGroup_TestTestTest::GetName() -- static: the AI state group name "SG_CREATURE_TEST_TEST_TEST" as a CCharString
// Leaf: ?GetName@CAIStateGroup_TestTestTest@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE288: push esi; push -1; push "SG_CREATURE_TEST_TEST_TEST"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220e34 registers this GetName with Alloc 0x900eac (pushed before it); Alloc 0x900eac stores the vtable of CAIStateGroup_TestTestTest into the new object; literal "SG_CREATURE_TEST_TEST_TEST"; PDB name set ?GetName@CAIStateGroup_TestTestTest@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_TestTestTest@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe288::build).
#define CAIStateGroup_TestTestTest CAIStateGroup_TestTestTestLayout
#include "engine/CAIStateGroup_TestTestTest.h"
#undef CAIStateGroup_TestTestTest

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_TestTestTest : public CAIStateGroup_TestTestTestLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_TestTestTest::GetName()
{
    return CCharString((const char*)0x0128A4BC, -1);   // "SG_CREATURE_TEST_TEST_TEST"
}
