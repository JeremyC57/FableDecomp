// CPKArray::GetCapacity() const -- the capacity word of the libpk array (+0x00)
// Leaf: ?GetCapacity@CPKArray@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61BC0, 3 bytes (thiscall, `mov eax,[ecx] ; ret`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- the array CPKArray::GetRandom 0x00C61C00 indexes; CAmbientEvent::Trigger
//   0x00C61D10 multiplies it by rand() for the empty-slot probability test. Was the propagated CInputEvent::GetKey (a `T::Get` stub).
class CPKArray { public:
    unsigned long m_capacity;                     // +0x00
    unsigned long _pad04;                         // +0x04
    unsigned long m_count;                        // +0x08 (GetCount 0x00C61C60)
    unsigned long GetCapacity() const;
};
unsigned long CPKArray::GetCapacity() const { return m_capacity; }
