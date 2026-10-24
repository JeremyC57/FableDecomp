// Behaviour test for LHHttp2::SendRawData (0x0083F8B0): 3 without a socket (nothing sent, count untouched); otherwise
// the block is copied into a byte vector of exactly `size` bytes, handed to the socket, the socket's last-sent count is
// reported and 0 returned.
#include <cstdio>
#include <cstring>
#include <vector>
struct LAddress {
    unsigned long ServerIP;
    LAddress() : ServerIP(0) { }
};
struct LSocket {
    std::vector<char> Sent;
    unsigned long NrLastDataSent;
    bool Send(std::vector<char>& data, const LAddress&, const LAddress&)
    {
        Sent = data;
        NrLastDataSent = (unsigned long)data.size();
        return true;
    }
};
struct LHHttp2 {
    LSocket* Host;
    int SendRawData(char* data, unsigned long size, unsigned long* bytesSent, bool)
    {
        if (!Host)
            return 3;
        std::vector<char> buffer;
        buffer.resize(size);
        std::memcpy(&buffer[0], data, size);
        Host->Send(buffer, LAddress(), LAddress());
        *bytesSent = Host->NrLastDataSent;
        return 0;
    }
};
int main()
{
    char block[] = "GET / HTTP/1.1\r\n";
    unsigned long sent = 12345;
    LHHttp2 h;
    h.Host = 0;
    int ok = h.SendRawData(block, 16, &sent, true) == 3 && sent == 12345;
    LSocket socket;
    socket.NrLastDataSent = 0;
    h.Host = &socket;
    ok = ok && h.SendRawData(block, 16, &sent, false) == 0 && sent == 16 && socket.Sent.size() == 16u
         && std::memcmp(&socket.Sent[0], block, 16) == 0;
    if (!ok) {
        std::printf("0083f8b0_TEST FAIL\n");
        return 1;
    }
    std::printf("0083f8b0_TEST PASS\n");
    return 0;
}