// CAnimationEntry::GetAnimateThingFlags() -- translate the flags component's bits 3/0/4 into animate-thing flags 1/2/4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC390, length 46 bytes (+ int3 padding).
// Callees: CAnimationEntry::GetFlagsComponent(T**) @ 0x005E0380 (writes the component of type 0 from the entry's
// map, or the default one, through its out pointer; the slot is re-read after the call).
// Shape: the out-pointer local is the one frame slot (`push ecx`); its component's Flags (+0x08) are read into
// cl and tested bit by bit with the result accumulated in eax (`xor eax,eax` first, then `or`).
class CAnimComponentFlags { public:
    unsigned char _pad[8];                      // vptr, ref count
    long Flags;                                 // +0x08
};


// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x005E0380 ??$GetComponent@VCAnimComponentFlags@@@CAnimationEntry@@QBEXPAPBVCAnimComponentFlags@@@Z (was ?GetFlagsComponent@CAnimationEntry@@QBEXPAPAVCAnimComponentFlags@@@Z)
class CAnimationEntry { public:
    long BankIndex;                             // +0x00
    long AnimName;                              // +0x04 CDefString
    long GroupName;                             // +0x08 CDefString
    void* Components;                           // +0x0c CIVCountedPointer<CAnimComponentMap>
    unsigned long GetAnimateThingFlags() const;
 public: template <class T1> void GetComponent(T1 const**) const; };

unsigned long CAnimationEntry::GetAnimateThingFlags() const
{
    CAnimComponentFlags* pComponent;
    reinterpret_cast<const CAnimationEntry*>(this)->GetComponent<CAnimComponentFlags>((CAnimComponentFlags const**)(&pComponent));
    long flags = pComponent->Flags;
    unsigned long result = 0;
    if (flags & 8)
        result |= 1;
    if (flags & 1)
        result |= 2;
    if (flags & 0x10)
        result |= 4;
    return result;
}