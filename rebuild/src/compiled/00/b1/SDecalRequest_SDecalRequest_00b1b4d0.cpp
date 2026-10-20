// SDecalRequest::SDecalRequest(void*, bool, long, long, float, short) -- fills a 0x24-byte request record (kind 0x1E, cleared flags, six caller values) and returns it
// Leaf: ??0SDecalRequest
// Grade: MATCH (retry 10, RTM 3077) -- the +0x08 flag byte modelled as a record with an inline zeroing constructor: the 0x1E immediate store cannot sink below a member-object constructor, so it lands right after the +0x4 zero store as in retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1B4D0, 69 bytes. No callees.
// SDecalRequest::SDecalRequest(void* source, bool enabled, long valueA, long valueB, float scale, short extra) -- a decal request record: kind 0x1E, reserved word and three flag bytes cleared, the six arguments copied into their slots
// The manifest label `CPersistContext::Init` is a propagated name and does not fit the bytes: the
// PDB CPersistContext (44 bytes) starts with PDefinitionManager/PSymbolMap pointers, while this body
// writes the constant 0x1E at +0x0, zero at +0x4 and three zero bytes at +0x8..+0xA. The body starts
// with `mov eax,ecx` and returns `this` with `ret 18h` (six dword arguments): a constructor.
// Its only callers are two calls inside NParticleEngine::CPSCDecalRenderer vtable slot 28
// (0x00B1B710), which build the record on the stack (arguments: a pointer from the renderer's
// +0x10, the flag 1 / 0, two registers, a float pushed with `fstp`, and a zero short) and hand the
// returned `this` to a virtual call at vtable +0x68 -- hence the descriptive name. The float argument
// is copied with integer moves (a plain copy) and the short is stored with a 16-bit move.
// Tried: init list (this), a base class owning +0x0/+0x4 with an inline constructor, body
// assignments in retail store order, `#pragma optimize("s",on)` (adds a frame): the lane always
// sinks the immediate store; every other instruction matches.
struct SZeroByte { unsigned char v; SZeroByte() : v(0) {} };
struct SDecalRequest {
    long          m_kind;                  // +0x00  always 0x1E here
    long          m_reserved;              // +0x04
    SZeroByte     m_flag8;                 // +0x08
    unsigned char m_flag9;                 // +0x09
    unsigned char m_flagA;                 // +0x0A
    void*         m_pSource;               // +0x0C  first argument
    short         m_extra;                 // +0x10  sixth argument
    long          m_valueA;                // +0x14  third argument
    long          m_valueB;                // +0x18  fourth argument
    float         m_scale;                 // +0x1C  fifth argument
    bool          m_enabled;               // +0x20  second argument

    SDecalRequest(void* source, bool enabled, long valueA, long valueB, float scale, short extra);
};

SDecalRequest::SDecalRequest(void* source, bool enabled, long valueA, long valueB, float scale, short extra)
    : m_kind(0x1E), m_reserved(0), m_flag9(0), m_flagA(0), m_pSource(source),
      m_extra(extra), m_valueA(valueA), m_valueB(valueB), m_scale(scale), m_enabled(enabled)
{
}