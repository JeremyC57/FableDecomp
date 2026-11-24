// Leaf: ?Close@CStreamingFontDataBank@@UAEXXZ
class CStreamingFontDataBank; extern void __fastcall h1(CStreamingFontDataBank* bank); extern void __fastcall h2(CStreamingFontDataBank* bank);
class CStreamingFontDataBank { public:
    virtual void Close();
};
void CStreamingFontDataBank::Close()
{
    CStreamingFontDataBank* bank = this; h1(bank); h2(bank); }
// Flatten route (flatten_pilot v4, NF324): CStreamingFontDataBank_Close -> CStreamingFontDataBank::Close, the RTTI-placed PDB member; class model: the row's local class model.
