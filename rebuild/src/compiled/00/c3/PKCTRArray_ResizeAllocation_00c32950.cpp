// PKCTRArray::ResizeAllocation(unsigned long growBy) -- grow the array's pointer block by growBy entries: allocate the larger block, copy the old entries across, free the old block and record the new block and capacity (asserting a positive growth, a successful allocation and an existing old block)
// Leaf: ?ResizeAllocation@PKCTRArray@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C32950, 559 bytes (+ int3 padding), `ret 4`. Built under /EHsc (one EH state
//   per assertion's log line: 0, 1, 2; the -1 restore value is held in edi). Callers: 0x00C32B91 (a tail jump) and
//   0x00C32E26 in the PKCTRArray code.
// Callees: scalar operator new 0x00BFEA1A / operator delete 0x00BFE9BC; the libpk assertion line as the landed
//   CResourceArray destructor 0x00C32BF0 (CString @ 0x0139C900 / Append 0x00C0DEC0 / AppendNumber 0x00C0DF00 /
//   ~CString 0x00C0BA40, sink 0x0143BB2C, CTrace 0x00C16750). Literals ".\PKCTRArray.cpp" lines 29 / 31 / 34 with
//   "Error: PK", "EMEM" and "NULL" (retail .rdata 0x012B4E74 / 0x012B1B58 / 0x012393E4).
// Layout (descriptive, libpk has no PDB; as the landed 0x00C32BF0): vtable +0, entry block +0x04, capacity +0x08
//   (the item count at +0x0C is untouched here).
// Shape: the copy is the inlined memcpy (`rep movsd` then the `rep movsb` remainder); the capacity sum is formed
//   after the old block is freed, so both tails re-read +0x08 and add the growth.
#include <string.h>

void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void  __cdecl operator delete(void* block);             // 0x00BFE9BC

extern char* g_pchNil_0139c900;                         // 0x0139C900
typedef void (__fastcall* PFNLOGSINK)(const char* text);
extern PFNLOGSINK g_pfnLogSink_0143bb2c;                // 0x0143BB2C

struct CString {                                        // MFC-style string: one word
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                                         // 0x00C0BA40
    CString* __fastcall Append(const char* text);       // 0x00C0DEC0
    CString* __fastcall AppendNumber(long value);       // 0x00C0DF00
};

struct CTrace {                                         // empty debug-break temporary
    CTrace();                                           // 0x00C16750
};

#define PK_ASSERT_MSG(cond, line_no, message)                                                                  \
    if (!(cond)) {                                                                                               \
        {                                                                                                        \
            CString line;                                                                                        \
            line.Append(".\\PKCTRArray.cpp")->Append("(")->AppendNumber(line_no)->Append(") : ")->Append(message)->Append("\n"); \
            if (g_pfnLogSink_0143bb2c)                                                                           \
                g_pfnLogSink_0143bb2c(line.m_pchData);                                                           \
        }                                                                                                        \
        CTrace();                                                                                                \
    }

class PKCTRArray {
public:
    void*         __vftable;
    void**        m_pEntries;                           // +0x04
    unsigned long m_capacity;                           // +0x08
    unsigned long m_count;                              // +0x0C
    void ResizeAllocation(unsigned long growBy);
};

void PKCTRArray::ResizeAllocation(unsigned long growBy)
{
    PK_ASSERT_MSG(growBy > 0, 29, "Error: PK")
    void** entries = (void**)operator new((m_capacity + growBy) * sizeof(void*));
    PK_ASSERT_MSG(entries != 0, 31, "EMEM")
    if (m_capacity) {
        PK_ASSERT_MSG(m_pEntries != 0, 34, "NULL")
        memcpy(entries, m_pEntries, m_capacity * sizeof(void*));
        operator delete(m_pEntries);
    }
    m_pEntries = entries;
    m_capacity += growBy;
}