// CAIStateGroup_StayNearFoodHall::GetName() -- static: the AI state group name "SG_STAY_NEAR_FOOD_HALL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_StayNearFoodHall@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDCA4: push esi; push -1; push "SG_STAY_NEAR_FOOD_HALL"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12205c4 registers this GetName with Alloc 0x90256c (pushed before it); Alloc 0x90256c -> ctor 0x900129 stores (last) the vtable of CAIStateGroup_StayNearFoodHall at this+0; literal "SG_STAY_NEAR_FOOD_HALL"; PDB name set ?GetName@CAIStateGroup_StayNearFoodHall@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_StayNearFoodHall@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdca4::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_StayNearFoodHall
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_StayNearFoodHall::GetName()
{
    return CCharString((const char*)0x01289C0C, -1);   // "SG_STAY_NEAR_FOOD_HALL"
}
