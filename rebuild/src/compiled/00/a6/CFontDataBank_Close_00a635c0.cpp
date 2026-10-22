struct CFontDataBank; extern void __fastcall h1(CFontDataBank* bank); extern void __fastcall h2(CFontDataBank* bank);
struct CFontDataBank { int x; };
void __fastcall Close(CFontDataBank* bank){ h1(bank); h2(bank); }