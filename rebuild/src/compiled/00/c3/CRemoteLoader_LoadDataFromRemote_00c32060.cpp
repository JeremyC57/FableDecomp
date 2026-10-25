// CRemoteLoader::LoadDataFromRemote(unsigned long offset, unsigned long size, void* dest) -- synchronous read over the async file reader: posts the request with a user-mailbox reply callback, then waits on the local mailbox for the completion flag
// Leaf: LoadDataFromRemote
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C32060, 206 bytes, __fastcall `ret 0Ch`, al = the reply flag (0 when the request
//   is refused). Descriptive name (no PDB covers this library; the catalog's `CResourceContainer_LoadDataFromRemote`
//   is kept). Caller: CMemoryStream::CMemoryStream 0x00C31F60 (this chunk).
// Callees (landed): the reply handler's constructor 0x00C1DD50 (vtable 0x012B332C, filed as InitStateGroup), the mailbox
//   constructor 0x00C1D9D0 (handler at +0x18, event at +0x1C), CMailbox::Dispatch 0x00C1DA30 (waits, then hands the message
//   to the handler's slot 0), the wait object's destructor 0x00C1DCE0 (EH state 0); the user-mailbox callback's slot 0
//   is ReplyToUser 0x00C1DB60 (PKCASyncFileReader.h); the request is CAsyncFileReader::ReadRemote 0x00C3BF30 (`ret 14h`).
// Shape: the wait object is one 0x30-byte local (handler + mailbox) whose inline constructor makes both calls before
//   state 0; the 12-byte callback {vtable 0x012B32CC, mailbox, reader} is built inline with no destructor; the reply
//   is the 8-byte {flag, message} record the handler's slot 0 copies out (a lone bool local packs into 4 bytes).
struct SReply {                            // the 8-byte record the one-shot queue hands back (PKCOneShotQueue.h)
    bool m_ok;                              // +0x00
    void* m_pMessage;                       // +0x04
};

struct CReplyHandler {                      // vtable 0x012B332C, 0x10 bytes
    virtual void Handle(void* message);     // slot 0  0x00C1DD70
    bool m_pending;                         // +0x04
    unsigned long m_field08;                // +0x08
    unsigned long m_field0C;                // +0x0C
    CReplyHandler();                        // 0x00C1DD50
};

struct CMailbox {                           // 0x20 bytes
    unsigned char _body[0x18];
    CReplyHandler* m_pHandler;              // +0x18
    void* m_hEvent;                         // +0x1C
    CMailbox(CReplyHandler* handler, long flag);   // 0x00C1D9D0
    void* Dispatch(void* message);          // 0x00C1DA30
};

struct CSyncWait {                          // 0x30 bytes
    CReplyHandler m_handler;                // +0x00
    CMailbox m_mailbox;                     // +0x10
    CSyncWait() : m_mailbox(&m_handler, 1) { }
    ~CSyncWait();                           // 0x00C1DCE0
};

struct CAsyncFileReader;

struct IUserCallback {
    virtual void ReplyToUser(char flag, void* message) = 0;
};

struct CUserMailboxCallback : IUserCallback {   // vtable 0x012B32CC
    CMailbox* m_pMailbox;                   // +0x04
    CAsyncFileReader* m_pReader;            // +0x08
    CUserMailboxCallback(CMailbox* mailbox, CAsyncFileReader* reader) : m_pMailbox(mailbox), m_pReader(reader) { }
    virtual void ReplyToUser(char flag, void* message);   // 0x00C1DB60
};

struct CAsyncFileReader {
    bool ReadRemote(unsigned long offset, unsigned long size, void* dest, unsigned long flags,
                    IUserCallback* callback);                                     // 0x00C3BF30
};

struct CRemoteLoader {
    CAsyncFileReader* m_pReader;
    bool LoadDataFromRemote(unsigned long offset, unsigned long size, void* dest);
};

bool CRemoteLoader::LoadDataFromRemote(unsigned long offset, unsigned long size, void* dest)
{
    CSyncWait wait;
    CUserMailboxCallback callback(&wait.m_mailbox, m_pReader);
    if (!m_pReader->ReadRemote(offset, size, dest, 0, &callback))
        return false;
    SReply reply;
    wait.m_mailbox.Dispatch(&reply);
    return reply.m_ok;
}