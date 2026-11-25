// CTCInventory::ProcessButtonXReleased() -- toggle the +0x1cd flag (clear it if set, else set it unless +0x1cc blocks it), then the empty hook 0x005BC68F
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581AE1, length 32 bytes (no padding: slot 61 follows at 0x00581B01).
// Vtable 0x01250DE4 (CTCInventory, rtti-vtables.tsv) slot 60 (ProcessButtonXReleased, named from the landed slot 58/61
// siblings ProcessButtonAReleased / ProcessButtonYReleased, chunk 91); part of the skipped-ledger row 00581ab3 (chunk 901).
// Callee: 0x005BC68F (landed `global_E2_005bc68f`, a one-byte `ret` reached as a thiscall tail jump).
// Layout as 0x00581AB3: the two bools at +0x1cc/+0x1cd are modelled locally (retail containers are 4 bytes
// smaller than engine/CTCInventory.h).
// A size-optimised TU (#pragma optimize("s", on)): the flag address is formed once and dl holds the shared zero.
// Shape: the flag's address formed once (`lea eax,[ecx+1CDh]`), `xor dl,dl` shared by both compares and the clear
// store, `mov byte ptr [eax],1` for the set, then `jmp 0x5BC68F` with ecx still `this`.
#pragma optimize("s", on)
class CTCInventory { public:                            // vtable 0x01250DE4
    virtual void v00();
    unsigned char _pad_0x04[0x1cc - 0x04];
    bool m_suppressA;                          // +0x1cc
    bool m_suppressB;                          // +0x1cd
    void ProcessButtonXReleased();
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x005BC68F ?ProcessButtonXReleased@CTCInventoryBase@NInventory@@UAEXXZ (was ?Hook_005BC68F@CTCInventory@@QAEXXZ)
namespace NInventory { class CTCInventoryBase { public: virtual void ProcessButtonXReleased(); }; }
void CTCInventory::ProcessButtonXReleased()
{
    bool& flag = m_suppressB;
    if (flag)
        flag = false;
    else if (!m_suppressA)
        flag = true;
    reinterpret_cast<NInventory::CTCInventoryBase*>(this)->NInventory::CTCInventoryBase::ProcessButtonXReleased();
}