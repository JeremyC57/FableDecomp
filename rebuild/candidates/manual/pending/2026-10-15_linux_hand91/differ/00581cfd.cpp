// Grade: DIFFER(43v45) -- addressing only: ours folds the element address once (`lea eax,[eax+ecx*4] ; cmp [eax],0 ; mov edx,[eax]`), retail scales the index alone (`shl ecx,2 ; cmp [ecx+eax],0 ; mov edx,[ecx+eax]`)
#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::GetContextSensitiveItemGraphicIndex(long index) -- graphic bank index of the context-sensitive item in the given slot, 0 for an empty slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581cfd, length 45 bytes; size-optimised TU with a frame pointer
// (`optimize("y",off)` + `("s",on)`, `leave`).
// Callees: GetEngineGraphic(long) 0x005D96A0 (__fastcall by-value CEngineGraphic: result slot in ecx, item type in edx;
//          returns the slot).
// Ego_r PDB: ?GetContextSensitiveItemGraphicIndex@CTCInventory@@QAEJJ@Z.
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (ContextSensitiveItems at PDB +0x178, 16-byte vectors);
// the shipped object keeps ContextSensitiveItems as a 12-byte {first,last,end} vector<long> at 0x154, so `this` is modelled locally.
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
    long& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventory {
    unsigned char _pad_0x000[0x154];
    CLongVector   ContextSensitiveItems;                  // 0x154 (PDB +0x178)
    long GetContextSensitiveItemGraphicIndex(long index);
};

long CTCInventory::GetContextSensitiveItemGraphicIndex(long index)
{
    if (ContextSensitiveItems[index] > 0)
        return GetEngineGraphic(ContextSensitiveItems[index]).BankIndex;
    return 0;
}
