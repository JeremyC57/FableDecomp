// CPKArray::GetCount() const -- the number of entries in the libpk array (+0x08)
// Leaf: ?GetCount@CPKArray@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61C60, 4 bytes (thiscall, `mov eax,[ecx+8] ; ret`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- the array CPKArray::GetRandom 0x00C61C00 indexes; CAmbientEvent::Trigger
//   0x00C61D10 tests it before picking a sound. Was the propagated DrawGetEnvironment (a __fastcall free stub over a byte pointer).
class CPKArray { public:
    unsigned long m_capacity;                     // +0x00 (GetCapacity 0x00C61BC0)
    unsigned long _pad04;                         // +0x04
    unsigned long m_count;                        // +0x08
    unsigned long GetCount() const;
};
unsigned long CPKArray::GetCount() const { return m_count; }
