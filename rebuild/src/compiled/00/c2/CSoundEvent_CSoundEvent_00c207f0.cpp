// CSoundEvent::~CSoundEvent() -- the libpk sound event's (non-virtual) destructor: own vtable, the state check with 0 (idle), then the +0x18 key member's destructor
// Leaf: ??1CSoundEvent@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C207F0, 21 bytes, tail `jmp` to the member destructor 0x00C25CA0.
// Identity: it stores vtable 0x012B3674 (the 0xB4-byte libpk sound event; no RTTI, no PDB: chunk 433 / sweep1 name the class
//   CSoundEvent from its describer 0x00C21040's strings, as every landed row of the family does). Two independent proofs that this is
//   that class's destructor: the eh vector constructor iterator call at 0x00C23A10 passes it with the constructor 0x00C21BA0 and
//   size 0xB4 (`new CSoundEvent[n]`), and the vector deleting destructor 0x00C22C30 passes it to the eh vector destructor iterator
//   (size 0xB4) and calls it directly for the scalar case. Vtable 0x012B3674 has no destructor slot, so the destructor is
//   non-virtual (QAE). The catalog's "Object_Destruct_c207f0" is descriptive; the landed ctor/dtor bindings
//   ??1SEntry_00c207f0@@QAE@XZ / ??1SEntry_00c239b0@@QAE@XZ (rows 0x00C23BA0 / 0x00C239B0) are placeholders (NAME_FIXES.tsv).
// Callees: CSoundEvent::CheckState 0x00C20720 (?CheckState@CSoundEvent@@QAEXJ@Z, the landed family spelling) with 0, the +0x18
//   member's destructor 0x00C25CA0 (an out-of-line empty body). The member is the event's sample reference, CSampleRef in the
//   landed CSoundEvent rows (GetSample 0x00C25C60, Reset/Clear 0x00C25CB0): its Set 0x00C25C70 AddRefs the CSoundSample it stores
//   (0x00C1EA30), Clear releases it (0x00C1EA40), and Play 0x00C21780 dereferences GetSample's result as that sample. Spelled
//   ??1CSampleRef@@QAE@XZ here; the bound ??1CKey_00c119d0@@QAE@XZ is a placeholder (NAME_FIXES.tsv).
// Shape: user destructor `CheckState(0);`; the members after +0x18 (sound parameters +0x38, the embedded virtual player +0x78)
//   are trivially destructible; `this` stays in ecx across CheckState because its body (same-TU stand-in under auto_inline(off),
//   as in the landed WaitForData 0x00C20850) is known not to clobber it.
struct CSoundEvent;

struct CSoundSample;

struct CSampleRef {                         // +0x18, 16 bytes (ctor 0x00C25C50 zeroes four words)
    CSoundSample* m_pSample;
    long m_4;
    long m_8;
    long m_c;
    ~CSampleRef();                          // 0x00C25CA0
};

struct CVirtualPlayer {                     // embedded at +0x78, vtable 0x012B37DC
    virtual void v00();
    unsigned char _body[0x28 - 4];
};

struct CSoundEvent {                        // 0xB4 bytes, vtable 0x012B3674 (no RTTI)
    virtual void v00();
    unsigned char _pad04[0x18 - 0x04];
    CSampleRef m_sample;                    // +0x18
    unsigned char _pad28[0x78 - 0x28];
    CVirtualPlayer m_virtualPlayer;         // +0x78
    unsigned char _padA0[0xAC - 0xA0];
    unsigned short m_state;                 // +0xAC
    unsigned char _padAE[0xB4 - 0xAE];
    void CheckState(long expected);         // 0x00C20720
    ~CSoundEvent();
};

// Same-TU body of CheckState 0x00C20720 (retail compiles it above its callers; the landed WaitForData stand-in): only the
// destructor is graded from this file.
#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
#pragma auto_inline(on)

CSoundEvent::~CSoundEvent()
{
    CheckState(0);
}