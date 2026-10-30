// Behaviour test for 0x00C48E00 PKCStaticQueue<SDecoderMessage>::Pop(): an empty queue reports "PKCStaticQueue.h(67) :
// Nothing in this queue - you should have checked first!" and breaks; then the oldest record (WriteIndex - Used, wrapped by
// the capacity) is returned by value and Used drops by one.
#include <cstdio>
#include <cstring>

struct SDecoderMessage { long m_type; long m_body[15]; };          // 0x40-byte record stand-in
static char g_report[256];
static int g_breaks;
static void Report(const char* s) { std::strcpy(g_report, s); }

struct Queue {
    unsigned int Capacity; SDecoderMessage* Entries; unsigned int Used; unsigned int WriteIndex;
    int OldestIndex() { int index = (int)(WriteIndex - Used); if (index < 0) index += Capacity; return index; }
    void Push(const SDecoderMessage& m) { Entries[WriteIndex] = m; if (++WriteIndex == Capacity) WriteIndex = 0; ++Used; }
    SDecoderMessage Pop()
    {
        if (Used == 0) {
            char text[256];
            std::sprintf(text, "%s(%ld) : %s\n", "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCStaticQueue.h", 67L,
                         "Nothing in this queue - you should have checked first!");
            Report(text);
            ++g_breaks;
        }
        int index = OldestIndex();
        --Used;
        return Entries[index];
    }
};

static int fail() { std::printf("00c48e00_TEST FAIL\n"); return 1; }

int main()
{
    SDecoderMessage store[3];
    std::memset(store, 0, sizeof(store));
    Queue q; q.Capacity = 3; q.Entries = store; q.Used = 0; q.WriteIndex = 0;
    SDecoderMessage m; std::memset(&m, 0, sizeof(m));
    for (int i = 1; i <= 3; ++i) { m.m_type = i; m.m_body[14] = i * 10; q.Push(m); }
    if (q.Used != 3 || q.WriteIndex != 0) return fail();
    SDecoderMessage r = q.Pop();                                        // oldest: 0 - 3 + 3 = slot 0
    if (r.m_type != 1 || r.m_body[14] != 10 || q.Used != 2) return fail();
    m.m_type = 4; q.Push(m);                                            // wraps into slot 0
    r = q.Pop(); if (r.m_type != 2) return fail();
    r = q.Pop(); if (r.m_type != 3) return fail();
    r = q.Pop(); if (r.m_type != 4 || q.Used != 0) return fail();
    if (g_breaks != 0) return fail();
    r = q.Pop();                                                        // empty: report + break
    if (g_breaks != 1 || std::strstr(g_report, "PKCStaticQueue.h(67) : Nothing in this queue") == 0) return fail();
    std::printf("00c48e00_TEST PASS\n");
    return 0;
}