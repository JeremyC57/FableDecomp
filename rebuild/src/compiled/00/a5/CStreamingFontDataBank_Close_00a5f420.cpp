class CStreamingFontDataBank; extern void __fastcall h1(CStreamingFontDataBank* bank); extern void __fastcall h2(CStreamingFontDataBank* bank);
class CStreamingFontDataBank { public: int x; };
void __fastcall Close(CStreamingFontDataBank* bank){ h1(bank); h2(bank); }