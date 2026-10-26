// CAIStateGroup_ThrowLogsOnFire::GetName() -- static: the AI state group name "SG_THROW_LOGS_ON_FIRE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_ThrowLogsOnFire@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDADA: push esi; push -1; push "SG_THROW_LOGS_ON_FIRE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220434 registers this GetName with Alloc 0x8ffe58 (pushed before it); Alloc 0x8ffe58 stores the vtable of CAIStateGroup_ThrowLogsOnFire into the new object; literal "SG_THROW_LOGS_ON_FIRE"; PDB name set ?GetName@CAIStateGroup_ThrowLogsOnFire@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_ThrowLogsOnFire@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdada::build).
#define CAIStateGroup_ThrowLogsOnFire CAIStateGroup_ThrowLogsOnFireLayout
#include "engine/CAIStateGroup_ThrowLogsOnFire.h"
#undef CAIStateGroup_ThrowLogsOnFire

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_ThrowLogsOnFire : public CAIStateGroup_ThrowLogsOnFireLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_ThrowLogsOnFire::GetName()
{
    return CCharString((const char*)0x012899A8, -1);   // "SG_THROW_LOGS_ON_FIRE"
}
