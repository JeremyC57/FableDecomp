// Behaviour test for CActionOverheadTalkToThing::CActionOverheadTalkToThing (0x00717BC0).
#include <cstdio>
#include <cstring>

static int g_destroyed = 0;

struct CSubtitledMessage { int id; ~CSubtitledMessage() { ++g_destroyed; } };

struct CSubtitledMessageArray {
    CSubtitledMessage* m_pFirst; CSubtitledMessage* m_pLast; CSubtitledMessage* m_pEnd;
    CSubtitledMessageArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
    ~CSubtitledMessageArray()
    {
        CSubtitledMessage* last = m_pLast;
        for (CSubtitledMessage* p = m_pFirst; p != last; ++p)
            p->~CSubtitledMessage();
        if (m_pFirst != 0)
            operator delete(m_pFirst);
    }
    CSubtitledMessageArray& operator=(const CSubtitledMessageArray& o)
    {
        long n = (long)(o.m_pLast - o.m_pFirst);
        m_pFirst = (CSubtitledMessage*)operator new(sizeof(CSubtitledMessage) * (n ? n : 1));
        for (long i = 0; i < n; ++i) m_pFirst[i].id = o.m_pFirst[i].id;
        m_pLast = m_pEnd = m_pFirst + n;
        return *this;
    }
};

static void GetMessagesFromTextName(CSubtitledMessageArray* messages, const char* textName, const char*, long count)
{
    messages->m_pFirst = (CSubtitledMessage*)operator new(sizeof(CSubtitledMessage) * count);
    for (long i = 0; i < count; ++i) messages->m_pFirst[i].id = (int)std::strlen(textName) + (int)i;
    messages->m_pLast = messages->m_pEnd = messages->m_pFirst + count;
}

struct CThing {};

struct CActionTalkBase {
    CThing* target; CSubtitledMessageArray Messages;
    CActionTalkBase(CThing* t, bool, bool, bool) : target(t) {}
};

struct CActionOverheadTalkToThing : CActionTalkBase {
    bool ScrollText; bool FinishedScroll;
    CActionOverheadTalkToThing(CThing* target, const char* textName, long method)
        : CActionTalkBase(target, false, false, false), ScrollText(false), FinishedScroll(false)
    {
        CSubtitledMessageArray messages;
        GetMessagesFromTextName(&messages, textName, 0, method);
        Messages = messages;
    }
};

int main()
{
    CThing t;
    CActionOverheadTalkToThing a(&t, "HELLO", 3);
    if (a.target != &t || a.ScrollText || a.FinishedScroll || a.Messages.m_pLast - a.Messages.m_pFirst != 3
        || a.Messages.m_pFirst[0].id != 5 || a.Messages.m_pFirst[2].id != 7 || g_destroyed != 3) {
        std::printf("00717bc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00717bc0_TEST PASS\n");
    return 0;
}
