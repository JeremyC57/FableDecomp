// CStreamPlayer_00c38a40::ReceiveStatus(bool* pReceived, CStreamRange_00c38a40* pRange) -- read one message from each of the two PKCMailbox members (+0xac: received flag, range, position; +0x40: valid flag, two words); report the first's flag through pReceived; when both carried data, cache the second's words at +0x174/+0x178 and the first's position at +0x170, copy the first's range to pRange when given, and return true
// Leaf: ?ReceiveStatus@CStreamPlayer_00c38a40@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C38A40, 126 bytes (+ int3 padding), __fastcall `ret 8`, bool in al.
// Library: libpk audio (no PDB/RTTI); descriptive names. The callees 0x00C39830 / 0x00C394B0 are the mailboxes'
//   by-value Read() (EH-framed, lock + `PKCMailbox.h` trace on an empty queue, result through a hidden pointer, `ret 4`).
//   Callers 0x00C38B57 (pRange 0) and 0x00C38DC5. The manifest's "CTCActivationReceptor_GetWeatherState" is propagated.
// Shape: `if (!a.m_received || !b.m_valid) return false;` puts the shared false block at the function end.
struct CStreamRange_00c38a40 {
    long m_start;
    long m_end;
};

struct CStreamMessage_00c38a40 {                        // 16 bytes
    bool m_received;
    CStreamRange_00c38a40 m_range;                      // +4
    long m_position;                                    // +0xc
};

struct CFormatMessage_00c38a40 {                        // 12 bytes
    bool m_valid;
    long m_format;                                      // +4
    long m_rate;                                        // +8
};

struct CStreamMailbox_00c38a40 {
    CStreamMessage_00c38a40 Read();                     // 0x00C39830
};

struct CFormatMailbox_00c38a40 {
    CFormatMessage_00c38a40 Read();                     // 0x00C394B0
};

struct CStreamPlayer_00c38a40 {
    unsigned char _pad0[0x40];
    CFormatMailbox_00c38a40 m_formatBox;                // +0x40
    unsigned char _pad41[0xac - 0x41];
    CStreamMailbox_00c38a40 m_streamBox;                // +0xac
    unsigned char _padad[0x170 - 0xad];
    long m_position;                                    // +0x170
    long m_format;                                      // +0x174
    long m_rate;                                        // +0x178
    bool ReceiveStatus(bool* pReceived, CStreamRange_00c38a40* pRange);
};

bool CStreamPlayer_00c38a40::ReceiveStatus(bool* pReceived, CStreamRange_00c38a40* pRange)
{
    CStreamMessage_00c38a40 stream = m_streamBox.Read();
    CFormatMessage_00c38a40 format = m_formatBox.Read();
    *pReceived = stream.m_received;
    if (!stream.m_received || !format.m_valid)
        return false;
    m_format = format.m_format;
    m_rate = format.m_rate;
    m_position = stream.m_position;
    if (pRange)
        *pRange = stream.m_range;
    return true;
}