// PKCBufferedReader::Refill() -- re-centre the read-ahead window on the current position (keep m_lookBehind blocks behind it: pull resident slots back in front of the window when moving backwards, drop blocks off the front when moving forwards), claim every free slot up to the end of the file for the blocks after the window, then issue one asynchronous remote read per run of consecutive claimed slots
// Leaf: ?Refill@PKCBufferedReader@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4A3B0, 1643 bytes (one function, `ret` at 0x00C4AA1A, 0xCC pad to Tell 0x00C4AA20),
// thiscall, EH-framed (seven CString trace messages, states 0..6).
// Identity: libpk PKCBufferedReader (vtable 0x012B8458); every trace names `.\PKCBufferedReader.cpp` (0x012B8200) with
// lines 121 / 145 / 170 / 196 / 229 / 256 / 271 ("Error: Cannot move the cache by more than the cache size - this should
// have been caught earlier", "Error: moving backwards but already at the beginning of the file!", "Error: moving the cache
// to a region outside the byte offset!", "Reading past EOF", "Error: Could not prime the queue..." x3). Name: the landed
// callers Seek 0x00C4AA40 and CopyOutOfBuffers (chunk 871, 0x00C4B640) call it Refill (reloc_bindings also carries the
// stale ?Refill@CBufferedReader_00c4aa40@@ and ?helperB@@YIXPAX@Z for this address). The catalog's "PrimeBuffer" is a guess.
// Slot owners (+4): 0xFF idle (no read pending), 0xFE claimed and waiting for a read, else the index of the first slot of the run whose read
// will fill it (the cookie handed to ReadRemote is that byte's address; the completion handler returns the slots).
// Callees: GetAvailableReadSize 0x00C4A1A0 (landed), CAsyncFileReader::ReadRemote 0x00C3BF30 (landed; the reader is the
// IUserCallback), the libpk trace (CString builders 0x00C0DEC0 / 0x00C0DF00, ~CString 0x00C0BA40, the assert hook
// 0x0143BB2C, CDebugBreak 0x00C16750) through engine/CString.h.
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include <stdlib.h>
#include "engine/CString.h"

// libpk's trace macro: "<file>(<line>) : <text>\n" through the assertion hook, then the debugger break (as chunk 871's
// CopyOutOfBuffers 0x00C4B640).
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

struct IUserCallback;
class CAsyncFileReader {
public:
    bool ReadRemote(unsigned long offset, unsigned long size, void* dest, unsigned long flags, IUserCallback* callback);   // 0x00C3BF30
};

struct IUserCallback {
    virtual void v00();
};

class PKCBufferedReader : public IUserCallback {        // vtable 0x012B8458, 0x70 bytes
public:
    unsigned char*      m_slotOwners;                   // +0x04 (0xFF idle, 0xFE claimed, else the run's first slot)
    unsigned long       m_blockSize;                    // +0x08
    unsigned long       m_0c;                           // +0x0c
    unsigned long       m_lookBehind;                   // +0x10
    unsigned long       m_slotCount;                    // +0x14
    CAsyncFileReader*   m_pSource;                      // +0x18
    unsigned char*      m_pBufferMemory;                // +0x1c
    unsigned long       m_size;                         // +0x20
    unsigned long       m_firstBlock;                   // +0x24
    unsigned long       m_firstSlot;                    // +0x28
    unsigned long       m_blockCount;                   // +0x2c
    unsigned long       m_position;                     // +0x30
    unsigned char       _body34[0x68 - 0x34];
    unsigned long       m_readsIssued;                  // +0x68
    long GetAvailableReadSize(unsigned long position, long size);   // 0x00C4A1A0
    void Refill();
};

void PKCBufferedReader::Refill()
{
    // Move the window so the current block sits m_lookBehind blocks into it.
    long current = (long)(m_position / m_blockSize);
    long move = current - (long)m_lookBehind;
    if (abs(move) > (long)m_slotCount)
        PKC_TRACE_ERROR(121, "Error: Cannot move the cache by more than the cache size - this should have been caught earlier")
    if (move < 0) {
        // Backwards: pull free slots in front of the first one, as far as the start of the file allows.
        if (move < -(long)m_firstBlock)
            move = -(long)m_firstBlock;
        for (; move != 0; ++move) {
            unsigned long slot = (m_slotCount + m_firstSlot - 1) % m_slotCount;
            if (m_slotOwners[slot] != 0xFF)
                break;
            m_slotOwners[slot] = 0xFE;
            m_firstSlot = slot;
            ++m_blockCount;
            m_position += m_blockSize;
            if (m_firstBlock == 0)
                PKC_TRACE_ERROR(145, "Error: moving backwards but already at the beginning of the file!")
            --m_firstBlock;
        }
    } else if (move > 0) {
        // Forwards: drop the blocks that fell behind the window.
        unsigned long count = m_blockCount - move;
        if (m_blockCount > count)
            m_blockCount = count;
        m_firstSlot = (m_firstSlot + move) % m_slotCount;
        if (m_position < m_blockSize * move)
            PKC_TRACE_ERROR(170, "Error: moving the cache to a region outside the byte offset!")
        m_position -= m_blockSize * move;
        m_firstBlock += move;
    }

    // Claim every free slot after the window, up to the last block of the file.
    unsigned long claim = m_slotCount - m_blockCount;
    unsigned long fileBlocks = (m_size + m_blockSize - 1) / m_blockSize;
    unsigned long end = m_firstBlock + m_blockCount;
    unsigned long remaining = fileBlocks - end;
    if (claim > remaining)
        claim = remaining;
    for (; claim != 0; --claim) {
        unsigned long slot = (m_firstSlot + m_blockCount) % m_slotCount;
        if (m_slotOwners[slot] != 0xFF)
            break;
        unsigned long total = (m_size + m_blockSize - 1) / m_blockSize;
        unsigned long block = m_firstBlock + m_blockCount;
        if (block >= total)
            PKC_TRACE_ERROR(196, "Reading past EOF")
        m_slotOwners[slot] = 0xFE;
        ++m_blockCount;
    }

    // Issue one remote read per run of consecutive claimed slots (a run never wraps past slot 0); every slot of a run
    // records the run's first slot, whose address is the read's cookie.
    unsigned char* run = 0;
    unsigned long offset;
    unsigned long bytes;
    for (unsigned long i = 0; i < m_blockCount; ++i) {
        unsigned long slot = (m_firstSlot + i) % m_slotCount;
        if (m_slotOwners[slot] == 0xFE) {
            if (run) {
                if (slot == 0) {
                    unsigned char* dest = m_pBufferMemory + (run - m_slotOwners) * m_blockSize;
                    bytes = GetAvailableReadSize(offset, bytes);
                    bool ok = m_pSource->ReadRemote(offset, bytes, dest, (unsigned long)run, this);
                    ++m_readsIssued;
                    if (!ok)
                        PKC_TRACE_ERROR(229, "Error: Could not prime the queue...")
                } else {
                    bytes += m_blockSize;
                    m_slotOwners[slot] = *run;
                    continue;
                }
            }
            run = m_slotOwners + slot;
            *run = (unsigned char)slot;
            offset = (m_firstBlock + i) * m_blockSize;
            bytes = m_blockSize;
        } else if (run) {
            unsigned char* dest = m_pBufferMemory + (run - m_slotOwners) * m_blockSize;
            bytes = GetAvailableReadSize(offset, bytes);
            bool ok = m_pSource->ReadRemote(offset, bytes, dest, (unsigned long)run, this);
            ++m_readsIssued;
            if (!ok)
                PKC_TRACE_ERROR(256, "Error: Could not prime the queue...")
            run = 0;
        }
    }
    if (run) {
        unsigned char* dest = m_pBufferMemory + (run - m_slotOwners) * m_blockSize;
        bytes = GetAvailableReadSize(offset, bytes);
        bool ok = m_pSource->ReadRemote(offset, bytes, dest, (unsigned long)run, this);
        ++m_readsIssued;
        if (!ok)
            PKC_TRACE_ERROR(271, "Error: Could not prime the queue...")
    }
}