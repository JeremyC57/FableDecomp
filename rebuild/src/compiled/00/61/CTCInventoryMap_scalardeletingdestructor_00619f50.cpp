// CTCInventoryMap::`scalar deleting destructor'(unsigned int) -- run the out-of-line destructor 0x00606F80, then free the object on flag bit 0
// Leaf: ??_GCTCInventoryMap@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00619F50, thiscall, `ret 4`: push esi ; mov esi,ecx ; call ??1CTCInventoryMap@@UAE@XZ ; test byte ptr [esp+8],1 ; je ; push esi ; call operator delete ; add esp,4 ; mov eax,esi ; pop esi ; ret 4.
// Identity (naming wave 21, chunk 689 NAME_FIXES): slot 0 of vtable 0x01257A9C (RTTI CTCInventoryMap; stored by the landed ctor 0x00606D20); its body calls ??1CTCInventoryMap@@UAE@XZ 0x00606F80
//   (landed, chunk 689) then operator delete 0x00BFE9BC. Was the __fastcall CTCInventoryMap_scalar_deleting_destructor stand-in (catalog CTCInventoryMap_scalar_deleting_destructor).
// Shape: the class's destructor is user-declared and out of line (one slot-0 entry); the vtable (and so this ??_G) is emitted by the
//   construction helper below, as the landed ??_G rows 0x008C37C0 / 0x00BB6520 are. The helper is not retail code.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC
class CTCInventoryMap {                                  // FableWin.pdb / Ego_r.pdb; only the slot-0 destructor matters here
public:
    virtual ~CTCInventoryMap();                          // 0x00606F80 (??1CTCInventoryMap@@UAE@XZ)
};
void* MakeOne_00619f50()
{
    return new CTCInventoryMap;
}
