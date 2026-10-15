// func_00ca5510(void* block) -- call operation 2 of the owner stored two words before `block`, passing the owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA5510, length 16 bytes (16 with padding; the next body follows at
// 0x00CA5520). Reached by tail jumps from 0x00CA6195 and 0x00CA61C8 and stored in the function table at 0x012BF3C8
// (not an RTTI vtable). The catalogue row 00ca5510 (75 bytes) held this body and three members at 0x00CA5520,
// 0x00CA5530, 0x00CA5540 (skipped ledger, chunk 901).
// Shape: `mov eax,[esp+4] ; mov eax,[eax-8] ; mov ecx,[eax] ; mov [esp+4],eax ; jmp [ecx+8]` -- the owner pointer
// read from the block's header (-8), its operation table's third entry tail-called with the owner replacing the
// argument in place (a __cdecl callee, so ecx is free to hold the table).
struct SBlockOwner_Retail;

struct SBlockOps_Retail {                        // the owner's operation table
    void (__cdecl* op0)(SBlockOwner_Retail*);
    void (__cdecl* op1)(SBlockOwner_Retail*);
    void (__cdecl* op2)(SBlockOwner_Retail*);
};

struct SBlockOwner_Retail {
    SBlockOps_Retail* ops;
};

void __cdecl func_00ca5510(void* block)
{
    SBlockOwner_Retail* owner = reinterpret_cast<SBlockOwner_Retail**>(block)[-2];
    owner->ops->op2(owner);
}