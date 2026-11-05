// Leaf: ?CShaderDataBank_Close@@YIXPAVCShaderDataBank@@@Z
class CShaderDataBank; extern void __fastcall h1(CShaderDataBank* bank); extern void __fastcall h2(CShaderDataBank* bank);
class CShaderDataBank { public: int x; };
void __fastcall CShaderDataBank_Close(CShaderDataBank* bank){ h1(bank); h2(bank); }