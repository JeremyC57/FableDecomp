// CMainGameComponent::CheckSync(const CEventPackage& package) -- network sync check: take the world checksum, then for every sync packet (type 1) in the package copy its payload into a read buffer and read the three 4-byte sync words back (the comparison is compiled out in the retail build)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004165E8, length 136 bytes.
// Grade: DIFFER(138v136) -- retail keeps the loop index in esi and re-reads the package argument from [ebp+8] for every call (no `this`/package register); ours caches the package in esi and the index in edi (+2 bytes for the extra push/pop) | tried: const reference, pointer, a one-word handle class by value
// Flags: /GS
// Callees: CMainGameComponent::PeekWorldChecksum @ 0x00416392 (result unused), CEventPackage::GetNoPackets @ 0x009F16E0,
//          CEventPackage::GetPacket(ulong) @ 0x009F16D0, CEventPacket::CopyToBuffer(CReadBuffer*) @ 0x009F17A0,
//          CReadBuffer::Read(ulong, void*) @ 0x009F18F0, __security_check_cookie @ 0x00BFE9F9.
// Shape: /GS (cookie at [ebp-4] next to the buffer's 0x20-byte array); size TU with a frame pointer
// (`optimize("s", on)` + `("y", off)`); the 0x2c-byte read buffer's inline constructor zeroes its length, read
// position and spare word with the shared `xor esi,esi`; the packet count is re-read every iteration.
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CReadBuffer {                               // 0x2c bytes
    unsigned char m_data[0x20];
    unsigned long m_length;                        // +0x20
    unsigned long m_readPos;                       // +0x24
    unsigned long m_spare;                         // +0x28
    CReadBuffer() : m_length(0), m_readPos(0), m_spare(0) { }
    void Read(unsigned long size, void* out);      // 0x009F18F0
};
struct CEventPacket {                              // 40 bytes
    long          m_type;                          // +0x00
    unsigned char _body[0x24];
    long CopyToBuffer(CReadBuffer* buffer) const;  // 0x009F17A0
};
struct CEventPackage {
    unsigned long GetNoPackets() const;            // 0x009F16E0
    const CEventPacket* GetPacket(unsigned long index) const;   // 0x009F16D0
};
struct CMainGameComponent {
    unsigned long PeekWorldChecksum();             // 0x00416392
    void CheckSync(const CEventPackage& package);
};

void CMainGameComponent::CheckSync(const CEventPackage& package)
{
    CReadBuffer buffer;
    PeekWorldChecksum();
    for (unsigned long i = 0; i < package.GetNoPackets(); ++i)
    {
        const CEventPacket* packet = package.GetPacket(i);
        if (packet->m_type == 1)
        {
            unsigned long worldChecksum, frame, random;
            packet->CopyToBuffer(&buffer);
            buffer.Read(4, &worldChecksum);
            buffer.Read(4, &frame);
            buffer.Read(4, &random);
        }
    }
}