// CGameEvent::ReadData8(CGameEvent* event) -- reads the next eight bytes of the event's data buffer into a two-word record returned in edx:eax
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00628E80, length 25 bytes.
// Callees: the buffered reader @ 0x009F18F0 (`this` passed through; arguments: byte count 8, destination), manifest label
// "CircularBuffer_Read"; this body's own label "AddUBYTE" is propagated (it reads, it does not add).
// Shape: an 8-byte POD local filled by the reader and returned as a plain two-dword struct in edx:eax with a plain `ret`:
// VC7.1 returns a class through a hidden pointer from every non-static member, so this is a static `__fastcall` taking the
// event in ecx (and `this`-less), not a __thiscall method.
struct SEventData8 {
    long m_low;
    long m_high;
};

struct CGameEvent {
    void ReadBytes(long count, void* destination);  // 0x009F18F0
    static SEventData8 __fastcall ReadData8(CGameEvent* event);
};

SEventData8 __fastcall CGameEvent::ReadData8(CGameEvent* event)
{
    SEventData8 value;
    event->ReadBytes(8, &value);
    return value;
}
