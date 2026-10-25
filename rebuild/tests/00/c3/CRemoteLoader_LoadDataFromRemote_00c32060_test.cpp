// Behaviour test for 00c32060: CRemoteLoader::LoadDataFromRemote -- refused request returns 0; otherwise the reply flag delivered through the mailbox
#include <cstdio>
#include <cstring>
struct SReply { bool m_ok; void* m_pMessage; };
struct CReplyHandler {
    bool m_pending; SReply m_stored;
    CReplyHandler() : m_pending(true) { m_stored.m_ok = false; m_stored.m_pMessage = 0; }
    void Handle(void* message) { *(SReply*)message = m_stored; m_pending = false; }
};
struct CMailbox {
    CReplyHandler* m_pHandler; int m_posted;
    CMailbox(CReplyHandler* handler, long) : m_pHandler(handler), m_posted(0) { }
    void Post(const SReply& reply) { m_pHandler->m_stored = reply; m_posted = 1; }
    void* Dispatch(void* message) { if (m_posted) m_pHandler->Handle(message); return message; }
};
static int g_waitDestroyed = 0;
struct CSyncWait {
    CReplyHandler m_handler; CMailbox m_mailbox;
    CSyncWait() : m_mailbox(&m_handler, 1) { }
    ~CSyncWait() { ++g_waitDestroyed; }
};
struct CAsyncFileReader;
struct IUserCallback { virtual void ReplyToUser(char flag, void* message) = 0; virtual ~IUserCallback() { } };
struct CUserMailboxCallback : IUserCallback {
    CMailbox* m_pMailbox; CAsyncFileReader* m_pReader;
    CUserMailboxCallback(CMailbox* mailbox, CAsyncFileReader* reader) : m_pMailbox(mailbox), m_pReader(reader) { }
    virtual void ReplyToUser(char flag, void* message) { SReply r; r.m_ok = flag != 0; r.m_pMessage = message; m_pMailbox->Post(r); }
};
struct CAsyncFileReader {
    int m_accept; int m_succeed; const char* m_data;
    bool ReadRemote(unsigned int offset, unsigned int size, void* dest, unsigned int, IUserCallback* callback)
    {
        if (!m_accept) return false;
        if (m_succeed) std::memcpy(dest, m_data + offset, size);
        callback->ReplyToUser(m_succeed ? 1 : 0, dest);
        return true;
    }
};
struct CRemoteLoader {
    CAsyncFileReader* m_pReader;
    bool LoadDataFromRemote(unsigned int offset, unsigned int size, void* dest)
    {
        CSyncWait wait;
        CUserMailboxCallback callback(&wait.m_mailbox, m_pReader);
        if (!m_pReader->ReadRemote(offset, size, dest, 0, &callback))
            return false;
        SReply reply;
        wait.m_mailbox.Dispatch(&reply);
        return reply.m_ok;
    }
};
int main()
{
    int ok = 1; char buf[8] = { 0 };
    CAsyncFileReader reader; reader.m_accept = 1; reader.m_succeed = 1; reader.m_data = "abcdefgh";
    CRemoteLoader loader; loader.m_pReader = &reader;
    ok = ok && loader.LoadDataFromRemote(2, 3, buf) && std::memcmp(buf, "cde", 3) == 0 && g_waitDestroyed == 1;
    reader.m_succeed = 0;
    ok = ok && !loader.LoadDataFromRemote(0, 3, buf) && g_waitDestroyed == 2;
    reader.m_accept = 0;
    ok = ok && !loader.LoadDataFromRemote(0, 3, buf) && g_waitDestroyed == 3;
    std::printf(ok ? "00c32060_TEST PASS\n" : "00c32060_TEST FAIL\n");
    return ok ? 0 : 1;
}