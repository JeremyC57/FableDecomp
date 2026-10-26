class CMeshDataBank; extern void __fastcall h1(CMeshDataBank* bank); extern void __fastcall h2(CMeshDataBank* bank);
class CMeshDataBank { public: int x; };
void __fastcall Close(CMeshDataBank* bank){ h1(bank); h2(bank); }