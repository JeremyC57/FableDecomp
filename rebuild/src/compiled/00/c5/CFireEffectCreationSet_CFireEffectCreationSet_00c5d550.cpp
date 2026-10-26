// CFireEffectCreationSet::CFireEffectCreationSet() -- this class's vtable, then the critical section at +4 initialised
// Leaf: ??0CFireEffectCreationSet@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D550, length 32 bytes.
// Callees: KERNEL32 InitializeCriticalSection (IAT 0x0143FEE4). Vtable 0x012B8FC8 (not in rtti-vtables.tsv; the chunk label names the class). Shape: the section
// address pushed before the vtable store, `this` returned.
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // IAT 0x0143FEE4

class CFireEffectCreationSet { public:                    // vtable 0x012B8FC8
    virtual void v00();
    unsigned char m_section[0x18];                 // +0x04: CRITICAL_SECTION
    CFireEffectCreationSet();
};

CFireEffectCreationSet::CFireEffectCreationSet()
{
    InitializeCriticalSection(m_section);
}