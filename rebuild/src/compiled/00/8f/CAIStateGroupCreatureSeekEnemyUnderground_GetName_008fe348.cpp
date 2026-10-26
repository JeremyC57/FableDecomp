// CAIStateGroup_CreatureSeekEnemyUnderground::GetName() -- static: the AI state group name "SG_CREATURE_SEEK_ENEMY_UNDERGROUND" as a CCharString
// Leaf: ?GetName@CAIStateGroup_CreatureSeekEnemyUnderground@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE348: push esi; push -1; push "SG_CREATURE_SEEK_ENEMY_UNDERGROUND"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220eac registers this GetName with Alloc 0x902a8a (pushed before it); Alloc 0x902a8a -> ctor 0x90107a stores (last) the vtable of CAIStateGroup_CreatureSeekEnemyUnderground at this+0; literal "SG_CREATURE_SEEK_ENEMY_UNDERGROUND"; PDB name set ?GetName@CAIStateGroup_CreatureSeekEnemyUnderground@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_CreatureSeekEnemyUnderground@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe348::build).
class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_CreatureSeekEnemyUnderground
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_CreatureSeekEnemyUnderground::GetName()
{
    return CCharString((const char*)0x0128A66C, -1);   // "SG_CREATURE_SEEK_ENEMY_UNDERGROUND"
}
