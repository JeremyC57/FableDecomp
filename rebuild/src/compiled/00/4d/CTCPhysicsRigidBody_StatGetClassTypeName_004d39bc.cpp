// CTCPhysicsRigidBody::StatGetClassTypeName() -- static: the class-type name "CTCPhysicsRigidBody" as a CCharString
// Leaf: ?StatGetClassTypeName@CTCPhysicsRigidBody@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D39BC: push esi; push -1; push "CTCPhysicsRigidBody"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): literal 0x123ae9c = "CTCPhysicsRigidBody" = the class name; PDB name set ?StatGetClassTypeName@CTCPhysicsRigidBody@@SI?AVCCharString@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_004d39bc::build).
#define CTCPhysicsRigidBody CTCPhysicsRigidBodyLayout
#include "engine/CTCPhysicsRigidBody.h"
#undef CTCPhysicsRigidBody

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCPhysicsRigidBody : public CTCPhysicsRigidBodyLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

CCharString __fastcall CTCPhysicsRigidBody::StatGetClassTypeName()
{
    return CCharString((const char*)0x0123AE9C, -1);   // "CTCPhysicsRigidBody"
}
