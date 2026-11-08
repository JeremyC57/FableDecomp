// PKCBufferedReader::CopyOutOfBuffers(void* buffer, unsigned long size) -- copy up to size bytes from the resident read-ahead blocks at the current position into buffer: return the completed reads first, then walk the blocks from the current one (each block's slot is (first slot + block) modulo the slot count; a slot still owned by an outstanding read is waited for, and the stall listener is told when that wait fails), copying the rest of each block, tracing "alignment out" / "Expecting to be processing SOME bytes!" errors; refill the read-ahead and return the bytes copied
// Leaf: ?CopyOutOfBuffers@PKCBufferedReader@@QAEKPAXK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4B640, 580 bytes, thiscall `ret 8`, EH-framed (two CString trace messages, states 0 / 1).
// Identity: libpk PKCBufferedReader (vtable 0x012B8458); both traces name `.\PKCBufferedReader.cpp` lines 292 / 300 (strings
//   0x012B8200, 0x012B84E4, 0x012B84B4); the landed spelling ?CopyOutOfBuffers@PKCBufferedReader@@QAEKPAXK@Z (caller ReadBlock
//   0x00C4B890, landed, which loops on the returned count).
// Callees: ReturnCompletedBuffers 0x00C4B060 (landed), WaitForSlot 0x00C4AEB0 (true when the slot came back), Refill 0x00C4A3B0
//   (re-issue the read-ahead; landed callers bind ?Refill@CBufferedReader_00c4aa40@@ -- same object), the stall listener's
//   slot 0 (+0x6c); the libpk trace macro: CString builders operator<<(const char*) 0x00C0DEC0 / (long) 0x00C0DF00, ~CString
//   0x00C0BA40, the trace hook g_pfnAssertHook 0x0143BB2C, CDebugBreak 0x00C16750; memcpy intrinsic.
// Layout (retail code): +4 slot owners (one byte per buffer slot, 0xFF = resident), +8 block size, +0x14 slot count, +0x1c buffer
//   memory (slot * block size), +0x28 first slot, +0x2c block count, +0x30 position, +0x6c the stall listener.
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include <string.h>
#include "engine/CString.h"

// libpk's trace macro: "<file>(<line>) : <text>\n" through the assertion hook, then the debugger break.
#define PKC_TRACE_ERROR(line, text) \
    { \
        { \
            CString message; \
            message << ".\\PKCBufferedReader.cpp" << "(" << (long)(line) << ") : " << text << "\n"; \
            if (g_pfnAssertHook) \
                g_pfnAssertHook(message.m_pchData); \
        } \
        CDebugBreak(); \
    }

class CUnprovenReaderListener_00c4b640 {
public:
    virtual void OnSlotStalled(unsigned long slot);     // slot 0
};

class PKCBufferedReader {                               // vtable 0x012B8458, 0x70 bytes
public:
    virtual void v00();                                 // slot 0: 0x00C4AB90
    unsigned char*      m_slotOwners;                   // +0x04 (0xFF = resident)
    unsigned long       m_blockSize;                    // +0x08
    unsigned long       m_0c;                           // +0x0c
    unsigned long       m_lookBehind;                   // +0x10
    unsigned long       m_slotCount;                    // +0x14
    void*               m_pSource;                      // +0x18
    unsigned char*      m_pBufferMemory;                // +0x1c
    unsigned long       m_size;                         // +0x20
    unsigned long       m_firstBlock;                   // +0x24
    unsigned long       m_firstSlot;                    // +0x28
    unsigned long       m_blockCount;                   // +0x2c
    unsigned long       m_position;                     // +0x30
    unsigned char       _body34[0x6c - 0x34];
    CUnprovenReaderListener_00c4b640* m_pListener;      // +0x6c
    void Refill();                                      // 0x00C4A3B0
    bool WaitForSlot(unsigned long slot);               // 0x00C4AEB0
    void ReturnCompletedBuffers();                      // 0x00C4B060
    unsigned long CopyOutOfBuffers(void* buffer, unsigned long size);
};

unsigned long PKCBufferedReader::CopyOutOfBuffers(void* buffer, unsigned long size)
{
    unsigned long block = m_position / m_blockSize;
    unsigned long offset = m_position - block * m_blockSize;
    unsigned long copied = 0;
    ReturnCompletedBuffers();
    while (size != 0 && block < m_blockCount) {
        unsigned long count = m_blockSize;
        if (offset >= count)
            PKC_TRACE_ERROR(292, "Error: alignment out")
        if (offset != 0)
            count -= offset;
        if (count > size)
            count = size;
        if (count == 0)
            PKC_TRACE_ERROR(300, "Error: Expecting to be processing SOME bytes!")
        unsigned long slot = (m_firstSlot + block) % m_slotCount;
        if (m_slotOwners[slot] != 0xFF && !WaitForSlot(slot) && m_pListener)
            m_pListener->OnSlotStalled(slot);
        memcpy(buffer, m_pBufferMemory + slot * m_blockSize + offset, count);
        size -= count;
        offset = 0;
        ++block;
        buffer = (char*)buffer + count;
        m_position += count;
        copied += count;
    }
    Refill();
    return copied;
}