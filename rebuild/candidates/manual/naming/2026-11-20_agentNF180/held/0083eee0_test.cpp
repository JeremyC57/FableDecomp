// Behaviour test for LHNetworkLib::LHHttp2::GetUntilCRLF 0x0083EEE0: with no host or nothing readable it reports RECEIVING_DOCUMENT
// (6) without reading; otherwise it appends one received byte per pass at buffer[*offset], and on LF after a CR terminates the line at
// the CR and reports DOCUMENT_RECEIVED (7); a failed receive reports TIMEOUT_ERROR (-102); after 1024 bytes it stops with 6.
#include <stdio.h>
#include <string.h>

struct FakeHost {
    bool readable;
    const char* input;
    int at;
    int failAt;
    bool Receive(char& out) { if (at == failAt) return false; out = input[at++]; return true; }
};

static int GetUntilCRLF(FakeHost* host, char* buffer, unsigned long* offset)
{
    bool foundCR = false;
    if (host && host->readable) {
        unsigned short count = 0;
        do {
            char byte = 0;
            if (!host->Receive(byte))
                return -102;
            buffer[*offset] = byte;
            if (buffer[*offset] == '\r')
                foundCR = true;
            if (buffer[*offset] == '\n' && foundCR) {
                buffer[*offset - 1] = 0;
                return 7;
            }
            ++*offset;
        } while (++count < 1024);
    }
    return 6;
}

int main()
{
    char buffer[1100];
    unsigned long offset = 0;
    bool ok = GetUntilCRLF(0, buffer, &offset) == 6 && offset == 0;
    FakeHost host = { false, "", 0, -1 };
    ok = ok && GetUntilCRLF(&host, buffer, &offset) == 6;
    host.readable = true; host.input = "hello\r\n"; host.at = 0;
    ok = ok && GetUntilCRLF(&host, buffer, &offset) == 7 && strcmp(buffer, "hello") == 0 && offset == 6;
    host.at = 0; host.failAt = 0; offset = 0;
    ok = ok && GetUntilCRLF(&host, buffer, &offset) == -102 && offset == 0;
    char longInput[1025]; memset(longInput, 'a', 1024); longInput[1024] = 0;
    host.input = longInput; host.at = 0; host.failAt = -1; offset = 0;
    ok = ok && GetUntilCRLF(&host, buffer, &offset) == 6 && offset == 1024;
    puts(ok ? "0083eee0_TEST PASS" : "0083eee0_TEST FAIL");
    return ok ? 0 : 1;
}
