class CFontDataBank; extern void __fastcall h1(CFontDataBank* bank); extern void __fastcall h2(CFontDataBank* bank);
class CFontDataBank { public: int x; };
void __fastcall Close(CFontDataBank* bank){ h1(bank); h2(bank); }