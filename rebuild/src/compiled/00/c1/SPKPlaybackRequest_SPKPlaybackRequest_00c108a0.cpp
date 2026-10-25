// SPKPlaybackRequest::SPKPlaybackRequest() -- libpk (audio middleware) request record constructor: zero the leading id, construct the embedded handle, zero the two trailing words
// Leaf: ??0SPKPlaybackRequest@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C108A0, 35 bytes, thiscall, plain `ret` (returns `this`).
// Identity: libpk (0x00C08AE0..0x00C40000: no PDB names, no RTTI), so the class and member names are descriptive. The only
//   caller 0x00C12B70 builds the record in a 0x28-byte stack local and then fills its words (+0x0, +0x1c and the float
//   block) before handing it on. The catalog label `pair<unsigned_long,C3DMeshVolume>` is propagated from the byte-similar
//   0x00A327E0 and wrong.
// Callees: the embedded handle's constructor @ 0x00C20390 (zeroes three words and a byte: an out-of-line LTCG body;
//   catalogued as the propagated `CNavigationLayer::CNavigationLayer`).
// Layout: +0x00 id (0), +0x04 the 0x10-byte handle, +0x14..+0x1f left for the caller, +0x20 / +0x24 zeroed words.
// Shape: member-init list in declaration order; the id store is scheduled between the `lea ecx` and the call.
struct SPKHandle {                                  // 0x10 bytes; constructor out of line
    void*         m_pObject;                        // +0x0
    unsigned long m_id;                             // +0x4
    unsigned long m_extra;                          // +0x8
    bool          m_valid;                          // +0xc
    SPKHandle();                                    // 0x00C20390
};

struct SPKPlaybackRequest {
    unsigned long m_eventId;                        // +0x00
    SPKHandle     m_handle;                         // +0x04
    float         m_volume;                         // +0x14 (set by the caller)
    float         m_pitch;                          // +0x18 (set by the caller)
    unsigned long m_flags;                          // +0x1c (set by the caller)
    unsigned long m_userData;                       // +0x20
    unsigned long m_callback;                       // +0x24
    SPKPlaybackRequest();
};

SPKPlaybackRequest::SPKPlaybackRequest() : m_eventId(0), m_userData(0), m_callback(0)
{
}