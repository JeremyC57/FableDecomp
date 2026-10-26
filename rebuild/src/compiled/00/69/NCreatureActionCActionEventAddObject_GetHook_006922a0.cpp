// NCreatureAction::CActionEventAddObject::GetHook() const -- the action-event hook name "CARRYTHINGADD" as a CCharString
// Leaf: ?GetHook@CActionEventAddObject@NCreatureAction@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006922A0, 25 bytes: push esi; mov esi, [esp+8] (hidden result); push -1;
//   push "CARRYTHINGADD" 0x0125C18C; mov ecx, esi; call CCharString::CCharString(const char*, long) 0x0099EBF0;
//   mov eax, esi; pop esi; ret 4 -- a virtual const member returning CCharString by value (`this` unused).
// Identity (namepass 2026-10-26): RTTI vtable 0x0125C178 (.?AVCActionEventAddObject@NCreatureAction@@) slot 2; the
//   PDB name sets give every NCreatureAction::CActionEvent* class exactly one CCharString-returning virtual,
//   ?GetHook@<class>@NCreatureAction@@UBE?AVCCharString@@XZ (CActionEventAddObject's included). The catalog's
//   "vfunc_2" and the source's old "CActionDoCreatureAction::GetActionName" were placeholders.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_006922a0::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NCreatureAction {
class CActionEventAddObject
{
public:
    virtual CCharString GetHook() const;                    // vtable 0x0125C178 slot 2
};
}

CCharString NCreatureAction::CActionEventAddObject::GetHook() const
{
    return CCharString((const char*)0x0125C18C, -1);   // "CARRYTHINGADD"
}
