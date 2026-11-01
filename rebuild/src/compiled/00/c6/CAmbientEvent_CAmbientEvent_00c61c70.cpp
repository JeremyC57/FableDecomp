// CAmbientEvent::CAmbientEvent(void* def, bool flag) -- constructor of the 16-byte ambient event record: the event definition (+0), a cleared due time (+4), the flag argument (+8), an index of -1 (+0xC)
// Leaf: ??0CAmbientEvent@@QAE@PAX_N@Z
// Naming (naming wave 23, DECISIONS_W22_RESOLVED item 1): the record 0x00C44C50 allocates (`operator new(0x10)`, (entry, false)) and hands to the bound ?Reschedule@CAmbientEvent@@QAEXPAVCAmbientEmitter@@@Z 0x00C61CA0 is the CAmbientEvent; was the junk CThingFilter_SolidToThing label (the PDB CThingFilter_SolidToThing is 8 bytes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61C70, length 32 bytes.
// Callees: none.
// Library: libpk ambient sound (no PDB/RTTI): descriptive member names (+4 is the due time CAmbientEmitter::ProcessEvents 0x00C44BD0 compares;
//   Reschedule reads the definition through [this]); the definition type is not named, hence void*. Retail loads the flag as a byte (`mov dl,[esp+8]`).
// Shape: plain init-list stores in member order; `this` returned in eax.
class CAmbientEvent { public:
    void*         m_pDef;                           // +0x00
    unsigned long m_time;                           // +0x04
    bool          m_flag;                           // +0x08
    long          m_index;                          // +0x0c
    CAmbientEvent(void* def, bool flag);
};

CAmbientEvent::CAmbientEvent(void* def, bool flag)
    : m_pDef(def)
    , m_time(0)
    , m_flag(flag)
    , m_index(-1)
{
}