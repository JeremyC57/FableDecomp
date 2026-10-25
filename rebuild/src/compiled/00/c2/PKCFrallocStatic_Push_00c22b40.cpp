// PKCFrallocStatic::Push(void* block) -- append a block to the fixed-capacity ring (asserting "Overflow" from PKCFrallocStatic.h line 85 when the ring is full), advance the write index modulo the capacity and count it
// Flags: /EHsc
// Leaf: Push
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C22B40, 222 bytes (`ret 4`), built under /EHsc. Descriptive member names (no
//   PDB covers the libpk audio library; the assert names the header "d:\dev\audio\ver_fable_1.5_b\libpk\
//   PKCFrallocStatic.h"): the manifest's "NScript_EventMessageSet_Push" is propagated. The function is slot 2 of the
//   vtable 0x012B3A9C stored by the constructor 0x00C23E00 (capacity +4, count +8, write index +0xC, +0x10, the
//   block array +0x14); slot 1 (0x00C24020) is the matching Pop (reads data[(write - count) wrapped], --count).
// Callees: the library log line (MFC-style CString over the shared empty buffer @ 0x0139C900): Append(const char*)
//   @ 0x00C0DEC0 and AppendNumber(long) @ 0x00C0DF00 (both `__fastcall`, returning the string), ~CString
//   @ 0x00C0BA40; the optional log sink @ 0x0143BB2C (`__fastcall`, the text); CDebugBreak @ 0x00C16750 (an empty
//   temporary homed in the dead high byte of the log-line frame word, `lea ecx,[esp+7]`).
// Shape: the assert is the library's warning block (`{ CString line; line.Append(__FILE__)->Append("(")->
//   AppendNumber(__LINE__)->Append(") : ")->Append(text)->Append("\n"); if (sink) sink(line); } CDebugBreak();`), the
//   string gets EH state 0 while it lives. The ring update reads the write index twice (store, then `(w + 1) % cap`
//   through `div`), the count increment is stored before the new write index.

typedef void (__fastcall* PFNLOGSINK)(const char* text);
extern PFNLOGSINK g_pfnLogSink;                 // 0x0143BB2C
extern char* g_pchEmptyString;                  // 0x0139C900 (the shared empty string buffer)

class CString {                                 // the library's MFC-style string (one word)
public:
    char* m_pchData;
    CString() : m_pchData(g_pchEmptyString) {}
    ~CString();                                 // 0x00C0BA40
    CString* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

struct PKCFrallocStatic {                       // vtable 0x012B3A9C
    virtual void v00();
    virtual void* Pop();                        // slot 1 = 0x00C24020
    virtual void Push(void* block);             // slot 2
    unsigned long m_capacity;                   // +0x04
    unsigned long m_count;                      // +0x08
    unsigned long m_write;                      // +0x0C
    unsigned long m_10;                         // +0x10
    void**        m_pBlocks;                    // +0x14
};

void PKCFrallocStatic::Push(void* block)
{
    if (m_count >= m_capacity) {
        {
            CString line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(85)->Append(") : ")->Append("Overflow")->Append("\n");
            if (g_pfnLogSink)
                g_pfnLogSink(line.m_pchData);
        }
        CDebugBreak();
    }
    m_pBlocks[m_write] = block;
    m_write = (m_write + 1) % m_capacity;
    ++m_count;
}