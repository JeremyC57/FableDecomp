// SEventRecord::SEventRecord() -- zeroes the 16-byte sound-event record: three words and the flag byte at +0xc
// Leaf: ??0SEventRecord@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C20390, oracle length 16 bytes.
// Naming (sweep1, 2026-10-25): was landed as CNavigationLayer::CNavigationLayer (a propagated manifest label);
//   descriptive (no PDB covers this library). The landed CSoundEvent rows (GetActivePlayer 0x00C20750,
//   IsWaitingOrPlayingPhysical 0x00C207C0, WaitForData 0x00C20850, StopWaitingForData 0x00C20920, ...) declare it as
//   `SEventRecord(); // 0x00C20390`: the record their requests and GetRecord 0x00C34600 carry. It is the shared
//   16-byte record of the libpk audio code: 13 call sites (0x00C108AC ... 0x00C62586), including the parameter-block
//   constructor 0x00C34320 (head record at +0) and the physical-play request 0x00C20A4F.
//
// Retail bytes:  mov eax, ecx ; xor ecx, ecx ; mov dword ptr [eax], ecx ; mov dword ptr [eax + 4], ecx ; mov dword ptr [eax + 8], ecx ; mov byte ptr [eax + 0xc], cl ; ret 
// A default constructor over three dword members and one byte flag (the dword members are initialised in
// declaration order from a zeroed ecx; `this` is returned). Member names are descriptive.
struct SEventRecord {                      // 16 bytes: three words and a flag byte
    void* m_a;                             // +0
    void* m_b;                             // +4
    void* m_c;                             // +8
    unsigned char m_flag;                  // +0xc
    SEventRecord();
};

SEventRecord::SEventRecord()
    : m_a(0), m_b(0), m_c(0), m_flag(0)
{
}
