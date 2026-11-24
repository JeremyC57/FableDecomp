// Leaf: ?Close@CFontDataBank@@UAEXXZ
class CFontDataBank; extern void __fastcall h1(CFontDataBank* bank); extern void __fastcall h2(CFontDataBank* bank);
class CFontDataBank { public:
    virtual void Close();
};
void CFontDataBank::Close()
{
    CFontDataBank* bank = this; h1(bank); h2(bank); }
// Flatten route (flatten_pilot v4, NF324): CFontDataBank_Close -> CFontDataBank::Close, the RTTI-placed PDB member; class model: the row's local class model.
