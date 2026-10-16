// LSocket::~LSocket() -- closes the socket, unregisters it from the global socket list and frees its buffers
// Leaf: ??1LSocket
// Retail 0x008F9780, 0x93 bytes. NOT CWound::Kill (propagated label): the body sits between LSocket::GetClientInformation
// (0x008F96E0) and LSocket::Open (0x008F9820), calls LSocket::Close 0x008F8C70 when Socket (retail +0x38, PDB +0x58)
// is not INVALID_SOCKET, erases the first node holding `this` from the global std::list<LSocket*> whose sentinel
// pointer lives at 0x013BB000 (erase(find(...)), node next +0 / prev +4 / value +8, node freed with free 0x00BFEA14),
// calls the MSVCR71 import operator delete (0x00BFE9BC, `jmp [0x01440164]` = ??3@YAXPAX@Z) with the word at +0x98
// only when that word is null, then runs the member destructors in reverse order: InternalPacketBuffer +0x70 and
// SendOutputQueue +0x64 (STLport vector<char>, 12 bytes), LocalAddress +0x1C and RemoteAddress +0x00 (retail
// LAddress is 0x1C bytes, PDB 44), each freeing its buffer with free when non-null. No vtable store: LSocket is
// not polymorphic. Retail offsets are 0x20/0x40 below the PDB ones because of the smaller LAddress/vector models.
extern "C" void free(void* p);                               // 0x00BFEA14 (MSVCR71 import thunk)
void operator delete(void* p);                               // 0x00BFE9BC (MSVCR71 import thunk)

struct CMallocBuffer
{
    void* Data;
    ~CMallocBuffer() { if (Data) free(Data); }
};

struct LAddress                                              // retail 0x1C bytes
{
    CMallocBuffer Name;                                      // +0x00
    unsigned long Rest[6];                                   // +0x04
};

struct CCharVector                                           // STLport vector<char>, 12 bytes
{
    CMallocBuffer Start;                                     // +0x00
    char*         Finish;                                    // +0x04
    char*         EndOfStorage;                              // +0x08
};

class LSocket;

struct CSocketListNode
{
    CSocketListNode* Next;                                   // +0x00
    CSocketListNode* Prev;                                   // +0x04
    LSocket*         Value;                                  // +0x08
};

struct CSocketList
{
    CSocketListNode* Node;                                   // sentinel
    void erase(CSocketListNode* pos)
    {
        CSocketListNode* next = pos->Next;
        CSocketListNode* prev = pos->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(pos);
    }
};

extern CSocketList g_Sockets;                                // 0x013BB000

class LSocket
{
public:
    LAddress      RemoteAddress;                             // +0x00
    LAddress      LocalAddress;                              // +0x1C
    unsigned int  Socket;                                    // +0x38
    unsigned char SocketInformation[0x10];                   // +0x3C sockaddr_in
    unsigned char SocketInterfaceInformation[0x10];          // +0x4C sockaddr_in
    bool          ListeningSocket;                           // +0x5C
    long          ConnectionMode;                            // +0x60
    CCharVector   SendOutputQueue;                           // +0x64
    CCharVector   InternalPacketBuffer;                      // +0x70
    bool          NoListeningBinding;                        // +0x7C
    void*         ServerObject;                              // +0x80
    bool          ActivityPending;                           // +0x84
    unsigned long NrTotalDataSent;                           // +0x88
    unsigned long NrTotalDataReceived;                       // +0x8C
    unsigned long NrLastDataSent;                            // +0x90
    unsigned long NrLastDataReceived;                        // +0x94
    void*         DebugFile;                                 // +0x98 _DEBUGFP

    bool Close();                                            // 0x008F8C70
    ~LSocket();
};

LSocket::~LSocket()
{
    if (Socket != (unsigned int)-1)
        Close();
    CSocketListNode* head = g_Sockets.Node;
    CSocketListNode* it = head->Next;
    while (it != head && it->Value != this)
        it = it->Next;
    g_Sockets.erase(it);
    if (!DebugFile)
        operator delete(DebugFile);
}
