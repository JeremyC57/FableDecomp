// CPKDecompressedStream::Read(const C16BitSamplePtr& destination, long frames) -- decompress up to `frames` 16-bit frames into the destination, rewinding the decompressor while loops remain (m_userData: -1 forever, n > 0 counts down, 0 marks the stream finished); returns the frames read, 0 once finished, -1 on a decoder error or without a decompressor
// Leaf: ?Read@CPKDecompressedStream@@
// Grade: RELOCATION_MATCH -- `total += read` ahead of `m_position += read` gives retail's read-modify-write `add dword
//   ptr [esi+8],eax` (the member update first made VC7.1 load/add/store it)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62790, 264 bytes, thiscall, `ret 8`; 8-aligned EH frame (`and esp,-8`).
// Caller: 0x00C48053 (the stream reader: builds the C16BitSamplePtr with 0x00C62A50 and passes it with the frame count).
// Callees: the decompressor's vtable slot 3 (+0x0c, decode into a sample pointer) and slot 2 (+0x08, Seek(0, true));
//   C16BitSamplePtr::Advance 0x00C62A30 (pointer += frames * channels shorts) and the out-of-line empty destructor
//   ~C16BitSamplePtr 0x00C62A20 (`ret`, defined in .\C16BitSamplePtr.cpp, the trace file of the constructor 0x00C62A50).
// Name: the class name C16BitSamplePtr is proven by the ".\C16BitSamplePtr.cpp" trace string (0x012B9374) of its
//   constructor; CPKDecompressedStream and its members keep the landed Seek 0x00C626A0 / destructor 0x00C62750 names;
//   Read, Decode and Advance are descriptive (libpk has no PDB or RTTI).
// Shape: the destination is copied into a local cursor (implicit copy constructor, its destructor is the EH state 0);
//   a decode error sets both the result and m_position to -1; the rewind result goes straight into m_position.
class C16BitSamplePtr {
public:
    short* m_pData;                                    // +0x00
    long   m_channels;                                 // +0x04 shorts per frame
    ~C16BitSamplePtr();                                // 0x00C62A20 (empty, out of line)
    void Advance(long frames);                         // 0x00C62A30
};
class CDecompressorInterface {
public:
    virtual ~CDecompressorInterface();
    virtual long GetUnitSize();                        // slot 1 (+0x04)
    virtual long Seek(long position, bool relative);   // slot 2 (+0x08)
    virtual long Decode(C16BitSamplePtr& destination, long frames);   // slot 3 (+0x0c)
    virtual long GetRate();                            // slot 4 (+0x10)
};
class CPKSourceObject;
class CPKDecompressedStream {
public:
    CPKSourceObject*        m_pSource;                 // +0x00
    CDecompressorInterface* m_pDecompressor;           // +0x04
    long                    m_position;                // +0x08
    long                    m_userData;                // +0x0c loops left: -1 forever, 0 last pass
    bool                    m_bFlag;                   // +0x10 finished
    long Read(const C16BitSamplePtr& destination, long frames);
};

long CPKDecompressedStream::Read(const C16BitSamplePtr& destination, long frames)
{
    long total = 0;
    if (!m_pDecompressor)
        return -1;
    if (m_position < 0)
        return -1;
    if (m_bFlag)
        return 0;
    C16BitSamplePtr cursor(destination);
    while (frames) {
        long read = m_pDecompressor->Decode(cursor, frames);
        if (read < 0) {
            total = -1;
            m_position = total;
            break;
        }
        if (read > 0) {
            total += read;
            m_position += read;
            frames -= read;
            cursor.Advance(read);
        }
        if (!frames)
            break;
        if (m_userData) {
            m_position = m_pDecompressor->Seek(0, true);
            if (m_position < 0) {
                total = -1;
                break;
            }
            if (m_userData > 0)
                --m_userData;
        } else {
            m_bFlag = true;
            break;
        }
    }
    return total;
}