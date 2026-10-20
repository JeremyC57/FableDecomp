// CTCHeroOnlineScoreboard::UpdateTransaction() -- step the HTTP transaction state machine (send request, read and check the response headers, receive the document) and report success, still-busy or failure
// Leaf: UpdateTransaction
//
// Retail 0x0055FB80, 272 bytes incl. the 20-byte jump table (plain `ret`). Ego_r.pdb
// `?UpdateTransaction@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@XZ`.
// Callees: the HTTP response record's ctor @ 0x00569C50 / dtor @ 0x00569D30 (labels "Transaction_InitializeVectors" /
// "~CPatchTesselationEdgeStrip" are propagated), LHNetworkLib::LHHttp2::SendRequest @ 0x008405F0, ::ReceiveResponseHeaders
// @ 0x00840740, ::ParseResponseLine @ 0x00840B60, ::ReceiveDocumentAsync @ 0x0083EFE0.
// Layout: Ego_r.pdb CTCHeroOnlineScoreboard with retail 12-byte maps/vectors and 4-byte lists: PHttp +0x0c (scoped_ptr),
// TaskState +0x54 (ETaskState 100..104); the response record (0x48 bytes) starts with the HTTP status code.
// Shape: a dense switch over 100..104 (case 100 explicitly returns the default's "busy" value so the table starts at 100;
// 101 takes the default); cases 102 and 103 each hold a block-scoped response record and fall through into the next state
// after advancing TaskState (the send-request success written first, `if (status == 2) ... else`, so it falls through inline);
// results 1000 success, 1001 busy/retry, 1002 failure.
namespace LHNetworkLib {
struct CHttpResponse {                                  // 0x48 bytes
    long          StatusCode;                           // +0x00
    unsigned char _body[0x44];
    CHttpResponse();                                    // 0x00569C50
    ~CHttpResponse();                                   // 0x00569D30
};
struct LHHttp2 {
    long SendRequest();                                 // 0x008405F0 (LH_HTTP_STATUS)
    long ReceiveResponseHeaders();                      // 0x00840740
    long ParseResponseLine(CHttpResponse* response);    // 0x00840B60
    long ReceiveDocumentAsync();                        // 0x0083EFE0
};
}

struct CTCHeroOnlineScoreboard {
    enum ENetworkSuccess { NETWORK_SUCCESS = 1000, NETWORK_BUSY = 1001, NETWORK_FAILED = 1002 };
    enum ETaskState { TASK_IDLE = 100, TASK_BEGIN = 101, TASK_SEND_REQUEST = 102, TASK_RECEIVE_HEADERS = 103, TASK_RECEIVE_DOCUMENT = 104 };
    void*                  __vftable;                   // +0x00
    void*                  ParentThing;                 // +0x04 (CTCBase)
    unsigned char          _pad08[0x0c - 0x08];
    LHNetworkLib::LHHttp2* PHttp;                       // +0x0c scoped_ptr<LHNetworkLib::LHHttp2>
    unsigned char          _pad10[0x54 - 0x10];
    long                   TaskState;                   // +0x54 ETaskState
    ENetworkSuccess UpdateTransaction();
};

CTCHeroOnlineScoreboard::ENetworkSuccess CTCHeroOnlineScoreboard::UpdateTransaction()
{
    long status;
    switch (TaskState) {
    case TASK_IDLE:
        return NETWORK_BUSY;
    case TASK_SEND_REQUEST: {
        LHNetworkLib::CHttpResponse response;
        status = PHttp->SendRequest();
        if (status == 2) {
            TaskState = TASK_RECEIVE_HEADERS;
        } else {
            if (status == -0x68 || status == -0x64)
                return NETWORK_BUSY;
            return NETWORK_FAILED;
        }
    }
    case TASK_RECEIVE_HEADERS: {
        LHNetworkLib::CHttpResponse response;
        status = PHttp->ReceiveResponseHeaders();
        if (status == -0x65 || status == -0x66)
            return NETWORK_BUSY;
        if (status != 5)
            return NETWORK_FAILED;
        PHttp->ParseResponseLine(&response);
        if (response.StatusCode != 200)
            return NETWORK_BUSY;
        TaskState = TASK_RECEIVE_DOCUMENT;
    }
    case TASK_RECEIVE_DOCUMENT:
        status = PHttp->ReceiveDocumentAsync();
        if (status == 7) {
            TaskState = TASK_IDLE;
            return NETWORK_SUCCESS;
        }
        if (status == -0x65 || status == -0x66)
            return NETWORK_BUSY;
        return NETWORK_FAILED;
    }
    return NETWORK_BUSY;
}