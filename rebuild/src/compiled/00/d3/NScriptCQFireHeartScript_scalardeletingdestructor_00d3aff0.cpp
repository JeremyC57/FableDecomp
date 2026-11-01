// NScript::CQ_FireHeartScript::`scalar deleting destructor'(unsigned int) -- run the out-of-line destructor 0x00D3B010, then free the object on flag bit 0
// Leaf: ??_GCQ_FireHeartScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3AFF0, thiscall, `ret 4`: push esi ; mov esi,ecx ; call ??1CQ_FireHeartScript@NScript@@UAE@XZ ; test byte ptr [esp+8],1 ; je ; push esi ; call operator delete ; add esp,4 ; mov eax,esi ; pop esi ; ret 4.
// Identity (naming wave 21, chunk 689 NAME_FIXES): slot 0 of vtable 0x012CCB90 (??_7CQ_FireHeartScript@NScript@@6B@, stored only by the landed ctor 0x00D34470); its body calls ??1CQ_FireHeartScript@NScript@@UAE@XZ 0x00D3B010
//   (landed, chunk 689) then operator delete 0x00BFE9BC. Was a CLandscapeBackgroundPatch `vector_deleting_destructor' __fastcall stand-in.
// Shape: the class's destructor is user-declared and out of line (one slot-0 entry); the vtable (and so this ??_G) is emitted by the
//   construction helper below, as the landed ??_G rows 0x008C37C0 / 0x00BB6520 are. The helper is not retail code.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC
namespace NScript {
class CQ_FireHeartScript {                               // Ego_r.pdb; only the slot-0 destructor matters here
public:
    virtual ~CQ_FireHeartScript();                       // 0x00D3B010 (??1CQ_FireHeartScript@NScript@@UAE@XZ)
};
}
void* MakeOne_00d3aff0()
{
    return new NScript::CQ_FireHeartScript;
}
