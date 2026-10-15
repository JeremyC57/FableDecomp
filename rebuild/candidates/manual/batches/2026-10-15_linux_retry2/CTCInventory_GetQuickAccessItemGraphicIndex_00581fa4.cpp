#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::GetQuickAccessItemGraphicIndex(long index) -- graphic bank index of the quick-access item in the given slot, 0 for an empty slot
// Retry (Fable landing session): matched once the vector subscript is the STLport form `*(begin() + n)` -- retail scales the index
// on its own (`shl ecx,2 ; [ecx+eax]`) / re-reads the element for the call; a bare `first[n]` folds the address into one `lea`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581FA4, length 45 bytes; size-optimised TU with a frame pointer
// (`optimize("y",off)` + `("s",on)`, `leave`).
// Callees: GetEngineGraphic(long) 0x005D96A0 (__fastcall by-value CEngineGraphic: result slot in ecx, item type in edx;
//          returns the slot).
// Ego_r PDB: ?GetQuickAccessItemGraphicIndex@CTCInventory@@QAEJJ@Z.
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (QuickAccessItems at PDB +0x168, 16-byte vectors);
// the shipped object keeps QuickAccessItems as a 12-byte {first,last,end} vector<long> at 0x148, so `this` is modelled locally.
// CEngineGraphic is the 14-byte PDB record (BankIndex, AnimStep, RenderSizeX, AdditiveAlpha, Type; sizeof 14,
// 2-byte packed), so the by-value temporary sits at ebp-0Eh in a 16-byte frame.
// Shape: CEngineGraphic needs a user copy constructor (non-POD): only then does VC7.1 pass a __fastcall
// hidden result pointer in ecx (argument in edx) and place the temporary at ebp-0Eh; a POD return pushes
// the pointer and copies the result. The element is tested (`cmp [ecx+eax],0 ; jle`) and re-read for the call; only BankIndex of the
// returned temporary is used (`mov eax,[eax]`).
#pragma pack(push, 2)
struct CEngineGraphic {
    unsigned long BankIndex;                         // +0x00
    float         AnimStep;                          // +0x04
    float         RenderSizeX;                       // +0x08
    bool          AdditiveAlpha;                     // +0x0c
    unsigned char Type;                              // +0x0d
    CEngineGraphic(const CEngineGraphic& other);     // user copy constructor: non-POD return
};
#pragma pack(pop)

CEngineGraphic __fastcall GetEngineGraphic(long itemType);   // 0x005D96A0

struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long* begin() const { return first; }
    long& operator[](unsigned long n) { return *(begin() + n); }
};

struct CTCInventory {
    unsigned char _pad_0x000[0x148];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)
    long GetQuickAccessItemGraphicIndex(long index);
};

long CTCInventory::GetQuickAccessItemGraphicIndex(long index)
{
    if (QuickAccessItems[index] > 0)
        return GetEngineGraphic(QuickAccessItems[index]).BankIndex;
    return 0;
}
