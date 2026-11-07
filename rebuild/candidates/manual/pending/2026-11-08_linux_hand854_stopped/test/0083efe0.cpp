// Behaviour test for LHNetworkLib::LHHttp2::ReceiveDocumentAsync 0x0083EFE0: the plain body appends each received block as a
// document part and finishes on an empty receive; the chunked body reads the hex chunk-size line, allocates size+2 bytes,
// fills the chunk across receives, stores it without its CRLF and finishes on a zero-size chunk; silence past TimeOutValue
// seconds frees the pending chunk and reports the timeout.
#include <cstdio>
#include <cstring>
#include <vector>

enum Status { RECEIVING = 0, RECEIVED = 1, TIMEOUT = 2, RECV_ERROR = 3, LINE_PENDING = 4 };
enum Encoding { PLAIN = 0, CHUNKED = 1 };

struct Part { char* Buffer; unsigned long BufferLength; unsigned long CurrBufPos; };

struct Http {
    // simulated socket: a queue of incoming blocks (an empty block = nothing readable)
    std::vector<std::vector<char> > Incoming;
    unsigned long Now;
    unsigned long TimeOutValue, ActionStartTime;
    bool ActionStarted;
    unsigned long CurrDownSize;
    std::vector<Part*> Parts;
    Encoding DocumentEncoding;
    char Line[64];
    bool LineReady;
    bool InChunkMode;
    Part* Chunk;
    bool ResetCalled;

    Status Receive()
    {
        Status returnstatus = RECEIVING;
        if (DocumentEncoding == CHUNKED) {
            if (!InChunkMode) {
                if (!LineReady) return RECEIVING;
                unsigned long chunksize = 0;
                InChunkMode = true;
                std::sscanf(Line, "%lx", &chunksize);
                if (chunksize > 0) {
                    chunksize += 2;
                    Chunk = new Part;
                    Chunk->Buffer = new char[chunksize];
                    std::memset(Chunk->Buffer, 0, chunksize);
                    Chunk->BufferLength = chunksize;
                    Chunk->CurrBufPos = 0;
                } else {
                    ActionStarted = false;
                    returnstatus = RECEIVED;
                }
                LineReady = false;
            } else if (Chunk) {
                if (Chunk->CurrBufPos < Chunk->BufferLength) {
                    if (!Incoming.empty() && !Incoming.front().empty()) {
                        std::vector<char> block = Incoming.front();
                        Incoming.erase(Incoming.begin());
                        unsigned long room = Chunk->BufferLength - Chunk->CurrBufPos;
                        unsigned long n = block.size() < room ? (unsigned long)block.size() : room;
                        std::memcpy(Chunk->Buffer + Chunk->CurrBufPos, &block[0], n);
                        Chunk->CurrBufPos += n;
                        CurrDownSize += n;
                        ActionStartTime = Now;
                    } else if (!ActionStarted) {
                        ActionStartTime = Now;
                        ActionStarted = true;
                    } else if (Now > ActionStartTime + TimeOutValue) {
                        delete[] Chunk->Buffer;
                        delete Chunk;
                        Chunk = 0;
                        ResetCalled = true;
                        returnstatus = TIMEOUT;
                    }
                } else {
                    Chunk->BufferLength -= 2;
                    Parts.push_back(Chunk);
                    InChunkMode = false;
                }
            } else {
                returnstatus = RECEIVED;
            }
            return returnstatus;
        }
        if (!Incoming.empty()) {
            std::vector<char> block = Incoming.front();
            Incoming.erase(Incoming.begin());
            unsigned long n = (unsigned long)block.size();
            if (n == 0) {
                ActionStarted = false;
                returnstatus = RECEIVED;
            } else {
                Part* p = new Part;
                p->Buffer = new char[n];
                std::memcpy(p->Buffer, &block[0], n);
                p->BufferLength = n;
                p->CurrBufPos = 0;
                CurrDownSize += n;
                Parts.push_back(p);
                ActionStartTime = Now;
            }
        } else if (!ActionStarted) {
            ActionStartTime = Now;
            ActionStarted = true;
        } else if (Now > ActionStartTime + TimeOutValue) {
            ResetCalled = true;
            returnstatus = TIMEOUT;
        }
        return returnstatus;
    }
    void Reset(Encoding e)
    {
        Incoming.clear(); Now = 100; TimeOutValue = 5; ActionStartTime = 0; ActionStarted = false; CurrDownSize = 0;
        Parts.clear(); DocumentEncoding = e; LineReady = false; InChunkMode = false; Chunk = 0; ResetCalled = false;
    }
};

static std::vector<char> Block(const char* s) { return std::vector<char>(s, s + std::strlen(s)); }

int main()
{
    Http h;
    // plain: two blocks then the empty receive that ends the document
    h.Reset(PLAIN);
    h.Incoming.push_back(Block("abc"));
    h.Incoming.push_back(Block("de"));
    h.Incoming.push_back(std::vector<char>());
    if (h.Receive() != RECEIVING || h.Receive() != RECEIVING || h.Receive() != RECEIVED
        || h.Parts.size() != 2 || h.CurrDownSize != 5 || h.Parts[1]->BufferLength != 2) {
        std::printf("0083efe0_TEST FAIL\n"); return 1;
    }
    // plain timeout: first silent call starts the clock, a call past the limit times out
    h.Reset(PLAIN);
    if (h.Receive() != RECEIVING || !h.ActionStarted) { std::printf("0083efe0_TEST FAIL\n"); return 1; }
    h.Now = 105;
    if (h.Receive() != RECEIVING) { std::printf("0083efe0_TEST FAIL\n"); return 1; }
    h.Now = 106;
    if (h.Receive() != TIMEOUT || !h.ResetCalled) { std::printf("0083efe0_TEST FAIL\n"); return 1; }
    // chunked: "5" line -> 7-byte chunk filled by two receives, stored as 5 bytes; then "0" ends the document
    h.Reset(CHUNKED);
    std::strcpy(h.Line, "5"); h.LineReady = true;
    if (h.Receive() != RECEIVING || !h.InChunkMode || !h.Chunk || h.Chunk->BufferLength != 7) {
        std::printf("0083efe0_TEST FAIL\n"); return 1;
    }
    h.Incoming.push_back(Block("hel"));
    h.Incoming.push_back(Block("lo\r\n"));
    h.Receive(); h.Receive();
    if (h.Chunk->CurrBufPos != 7 || h.Receive() != RECEIVING || h.InChunkMode || h.Parts.size() != 1
        || h.Parts[0]->BufferLength != 5 || std::memcmp(h.Parts[0]->Buffer, "hello", 5) != 0) {
        std::printf("0083efe0_TEST FAIL\n"); return 1;
    }
    std::strcpy(h.Line, "0"); h.LineReady = true;
    if (h.Receive() != RECEIVED || h.ActionStarted) { std::printf("0083efe0_TEST FAIL\n"); return 1; }
    // chunked timeout frees the pending chunk
    h.Reset(CHUNKED);
    std::strcpy(h.Line, "a"); h.LineReady = true;
    h.Receive();
    if (h.Chunk->BufferLength != 12) { std::printf("0083efe0_TEST FAIL\n"); return 1; }
    h.Receive();
    h.Now = 200;
    if (h.Receive() != TIMEOUT || h.Chunk != 0 || !h.ResetCalled) { std::printf("0083efe0_TEST FAIL\n"); return 1; }
    std::printf("0083efe0_TEST PASS\n");
    return 0;
}
