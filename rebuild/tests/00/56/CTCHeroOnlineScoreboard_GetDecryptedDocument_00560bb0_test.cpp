// Behaviour test for CTCHeroOnlineScoreboard::GetDecryptedDocument (0x00560BB0): links the row object. Its same-TU TU-mates (EncipherMorsel,
// DecipherMorsel, CryptoMatic, Explode) and the STLport template bodies run for real; the HTTP connection's GetDocumentSize / GetDocument
// are /alternatename stand-ins that log every call. Each document is the test's own mode-0 encryption of a known text (the reference
// XTEA / CryptoMatic encoder below, written from the TU's algorithm), so the row must decrypt with the key it is given and split the
// result at every CRLF. Checked: the return value (-1 with no document, else the vector's total size), the lines appended (the tail after
// the last CRLF dropped, earlier entries kept), the stand-ins' `this` (the PHttp pointer at +0x0C), the call counts, and that the
// GetDocument buffer arrives zero-filled in its own malloc block one byte longer than the document (_msize).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
extern "C" int __fastcall RowGetDecryptedDocument(void* self, int unused, void* lines, const void* key);
#pragma comment(linker, "/alternatename:@RowGetDecryptedDocument@16=?GetDecryptedDocument@CTCHeroOnlineScoreboard@@AAEHAAV?$vector@V?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@ABVCCharString@@@Z")

static unsigned char g_http[16];
static const void* g_size_this; static const void* g_doc_this; static int g_size_calls, g_doc_calls, g_buffer_ok;
static unsigned char g_doc[256]; static unsigned long g_doc_size;
extern "C" unsigned long __fastcall StubSize(const void* s, int) { g_size_this = s; ++g_size_calls; return g_doc_size; }
extern "C" int __fastcall StubDoc(const void* s, int, char* buffer)
{
    g_doc_this = s; ++g_doc_calls;
    g_buffer_ok = 1;
    for (unsigned long i = 0; i <= g_doc_size; ++i) if (buffer[i] != 0) g_buffer_ok = 0;   // vector(size + 1, 0)
    if (_msize(buffer) != g_doc_size + 1) g_buffer_ok = 0;                               // its own malloc block of size + 1
    memcpy(buffer, g_doc, g_doc_size);
    return 200;
}
extern "C" const char NullCharStub = 0;
#pragma comment(linker, "/alternatename:?GetDocumentSize@LHHttp2@LHNetworkLib@@QAEKXZ=@StubSize@8")
#pragma comment(linker, "/alternatename:?GetDocument@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@PAD@Z=@StubDoc@12")
#pragma comment(linker, "/alternatename:?NullChar@CCharString@@1DB=_NullCharStub")

// Reference encoder (mode 0 of the TU's CryptoMatic: length word, then XTEA blocks with the evolving key schedule).
static void Encipher(const unsigned long* v, unsigned long* w, const unsigned long* k)
{
    unsigned long v0 = v[0], v1 = v[1], sum = 0;
    for (int i = 0; i < 32; ++i) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += 0x9e3779b9;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
    }
    w[0] = v0; w[1] = v1;
}
static unsigned long Encrypt(const char* text, const char* key, unsigned char* out)
{
    unsigned long keylength = strlen(key);
    char keycopy[64]; memset(keycopy, 0, sizeof keycopy);
    for (unsigned long c = 0; c < keylength; ++c) keycopy[c] = (char)(key[keylength - c] * (c + 1));
    unsigned long k[4] = { (unsigned long)(long)keycopy[3], (unsigned long)(long)keycopy[2], (unsigned long)(long)keycopy[1], (unsigned long)(long)keycopy[0] };
    long dlen = (long)strlen(text); long t = 4;
    unsigned char* enc = out; memcpy(enc, &dlen, 4); enc += 4;
    const unsigned char* data = (const unsigned char*)text;
    for (; dlen >= 8; dlen -= 8) {
        unsigned long in[2]; memcpy(in, data, 8); Encipher(in, (unsigned long*)enc, k);
        data += 8; enc += 8;
        if (t >= (long)keylength) t = 0; k[0] += keycopy[t++];
        if (t >= (long)keylength) t = 0; k[1] -= keycopy[t++];
        if (t >= (long)keylength) t = 0; k[2] += keycopy[t++];
        if (t >= (long)keylength) t = 0; k[3] -= keycopy[t++];
    }
    if (dlen > 0) { unsigned long in[2] = { 0, 0 }; memcpy(in, data, dlen); Encipher(in, (unsigned long*)enc, k); enc += 8; }
    return (unsigned long)(enc - out);
}

struct NetString { char* start; char* finish; char* eos; };          // STLport 4.6 basic_string<char>
struct NetVector { NetString* start; NetString* finish; NetString* eos; };
struct KeyData { const char* text; };                                  // CCharStringData (Data's buffer at +0)
static int g_fail;
static void Check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static int LineIs(const NetVector& v, int i, const char* want)
{
    if (i >= v.finish - v.start) return 0;
    const NetString& s = v.start[i];
    return (unsigned long)(s.finish - s.start) == strlen(want) && memcmp(s.start, want, strlen(want)) == 0 && *s.finish == 0;
}
static int Run(NetVector& lines, const char* text, const char* key_text)
{
    KeyData data = { key_text }; KeyData* pdata = &data; const void* key = &pdata;
    g_doc_size = text ? Encrypt(text, key_text, g_doc) : 0;
    g_size_calls = g_doc_calls = 0; g_size_this = g_doc_this = 0; g_buffer_ok = 0;
    unsigned char self[0x68]; memset(self, 0xA5, sizeof self); *(void**)(self + 0x0C) = g_http;
    int r = RowGetDecryptedDocument(self, 0, &lines, key);
    Check(g_size_calls == 1 && g_size_this == g_http, "GetDocumentSize once on PHttp");
    Check(*(void**)(self + 0x0C) == g_http, "component untouched");
    return r;
}

int main()
{
    const char* key = "THIS-ISMY-VALI-DCDK-EYOH-YESI-TISM";
    NetVector lines = { 0, 0, 0 };
    Check(Run(lines, 0, key) == -1, "no document returns -1");
    Check(g_doc_calls == 0 && lines.start == 0 && lines.finish == 0, "no document: nothing read or appended");

    Check(Run(lines, "SID:OK\r\nalpha\r\n\r\nbeta\r\ntail", key) == 4, "four lines");
    Check(g_doc_calls == 1 && g_doc_this == g_http && g_buffer_ok, "GetDocument once on PHttp, zero-filled size+1 buffer");
    Check(lines.finish - lines.start == 4, "vector size 4");
    Check(LineIs(lines, 0, "SID:OK") && LineIs(lines, 1, "alpha") && LineIs(lines, 2, "") && LineIs(lines, 3, "beta"), "first lines");

    Check(Run(lines, "1000\r\n7.5\r\nHero name with spaces\r\n", "ANOTHER-KEY-FOR-THE-TEST") == 7, "appends three more");
    Check(LineIs(lines, 0, "SID:OK") && LineIs(lines, 3, "beta"), "earlier lines kept");
    Check(LineIs(lines, 4, "1000") && LineIs(lines, 5, "7.5") && LineIs(lines, 6, "Hero name with spaces"), "second lines");

    Check(Run(lines, "no separator at all, long enough for blocks", key) == 7, "no CRLF appends nothing");
    Check(Run(lines, "x\r\n", key) == 8 && LineIs(lines, 7, "x"), "one short line");

    if (g_fail) { printf("00560bb0_TEST FAIL\n"); return 1; }
    printf("00560bb0_TEST PASS\n");
    return 0;
}