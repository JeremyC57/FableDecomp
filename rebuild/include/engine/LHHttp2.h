#ifdef FABLE_RETAIL_LHHTTP2
// Retail opt-in (Codex pass 11): original LHNetworkLib TPI names and full native 0x10a8 field layout; 008400b0/00840740 operand offsets.
#ifndef FABLE_RETAIL_LHHTTP2_HEADER
#define FABLE_RETAIL_LHHTTP2_HEADER
#include "stlport/fable_string.h"
#include "LHNetworkTypes.h"
namespace LHNetworkLib {
class LSocket;
struct LHHttpDocumentParts {
    char* Buffer;
    unsigned long BufferLength;
    unsigned long CurrBufPos;
#ifdef FABLE_RETAIL_LHHTTP2_RECEIVEDOCUMENT
    // Nested opt-in (chunk 854, row 0x0083EFE0 LHHttp2::ReceiveDocumentAsync): FableWin.pdb ??0LHHttpDocumentParts@LHNetworkLib@@QAE@XZ,
    // inline in retail (after `new`: `mov [eax],edi ; mov [eax+4],edi ; mov [eax+8],edi`).
    LHHttpDocumentParts() { Buffer=0;BufferLength=0;CurrBufPos=0; }
#endif
    ~LHHttpDocumentParts() { if(Buffer) { delete[] Buffer;Buffer=0; } BufferLength=0; }
};
class LHHttpHeaders {
public:
    std::vector<std::string> HdrDocumentHeader;
    std::string HeaderString;
#ifdef FABLE_RETAIL_LHHTTP2_SCOREBOARD
    // Nested opt-in (chunk 777, row 0x0055F710 CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest): Reset is out of line in
    // retail (0x0083FBC0 ?Reset@LHHttpHeaders@LHNetworkLib@@QAEXXZ, landed) and the user destructor (Ego_r + FableWin
    // ??1LHHttpHeaders@LHNetworkLib@@QAE@XZ, inline in retail) calls it before the members' own destructors (0x0055FB41: Reset,
    // HeaderString's free, ~vector<string> 0x00569940).
    void Reset();
    ~LHHttpHeaders() { Reset(); }
#else
    void Reset() { HeaderString="";HdrDocumentHeader.clear(); }
#ifdef FABLE_RETAIL_LHHTTP2_DTOR
    // NF123 opt-in (row 0x008410D0 ~LHHttp2, q4193): the user destructor (Ego_r 0x005E03E0 + FableWin 0x022DE110
    // ??1LHHttpHeaders@LHNetworkLib@@QAE@XZ, inline in retail) with Reset's body visible, as the LHHttp TU has it (0x008410D0 expands
    // both: assign + erase, then the members' destructors).
    ~LHHttpHeaders() { Reset(); }
#endif
#endif
    void DeleteHeaderString() { HeaderString=""; }
    void AddHeader(const std::string& line) { HdrDocumentHeader.push_back(line); }
    void AddHeader(const std::string& name,const std::string& value);
    std::string GetHeaderString();
#ifdef FABLE_RETAIL_LHHTTP2_GETHEADER
    // Nested opt-in (chunk 865, row 0x0083F650): Ego_r.pdb ?GetHeader@LHHttpHeaders@LHNetworkLib@@QAE?AV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@ABV34@@Z
    // (the value of the first header line starting with name), called by LHHttp2::GetHeader 0x00840B60 (landed).
    std::string GetHeader(const std::string& name);                       // 0x0083F650
#endif
};
class LHHttp2 {
    LSocket* Host;
    std::string Hostname;
    unsigned short Port;
    std::string PreparedRequest;
    unsigned long RequestBytesSent;
    unsigned long RequestLength;
    LHHttpHeaders MyHeaders;
    LHHttpHeaders ReceivedHeaders;
    unsigned long TimeOutValue;
    unsigned long ActionStartTime;
    bool ActionStarted;
    unsigned short CurrentAction;
    unsigned long CurrDownSize;
    unsigned long Forwardings;
    unsigned long CurrForwarding;
    char CurrentHeaderPart[2048];
    unsigned long CurrentHeaderPos;
    std::string ProxyHost;
    unsigned long ProxyPort;
    std::vector<LHHttpDocumentParts*> DocumentParts;
    HTTP_TRANSFER_ENCODING DocumentEncoding;
    unsigned long DocumentEstimatedLength;
    unsigned long DocumentLength;
    char CurrentDocumentPart[2048];
    unsigned long CurrentDocumentPos;
    bool InChunkMode;
    LHHttpDocumentParts* CurrentDocumentChunk;
public:
#ifdef FABLE_RETAIL_LHHTTP2_SCOREBOARD
    // Nested opt-in (chunk 777, row 0x0055F710): the members CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest calls, all
    // out of line in retail and landed; Ego_r.pdb + FableWin.pdb spellings (the PDBs' Dinkumware `U?$char_traits` becomes STLport `V`).
#ifdef FABLE_RETAIL_LHHTTP2_DOCUMENT
    // Both PDB PUBLIC signatures, native 0x0083EA70 / 0x0083EAA0.
    unsigned long GetDocumentSize();
    LH_HTTP_STATUS GetDocument(char* buffer);
#endif
    LHHttp2();                                                           // 0x00840F50 ??0LHHttp2@LHNetworkLib@@QAE@XZ
    ~LHHttp2();                                                          // 0x008410D0 ??1LHHttp2@LHNetworkLib@@QAE@XZ
    void SetProxySettings(const std::string&,unsigned short);            // 0x0083F870
    LH_RETURN SetConnectionDetails(const std::string&,unsigned short);   // 0x0083FB20
#endif
#ifdef FABLE_RETAIL_LHHTTP2_OPEN
    // NF116 opt-in (row 0x0083FB20 SetConnectionDetails): Ego_r.pdb / FableWin.pdb ?Open@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@XZ
    // (0x0083F9B0, public), called out of line. Declaration only, no layout change.
    LH_RETURN Open();                                                    // 0x0083F9B0
#endif
#if defined(FABLE_RETAIL_LHHTTP2_DTOR) && !defined(FABLE_RETAIL_LHHTTP2_SCOREBOARD)
    // NF123 opt-in (row 0x008410D0, q4193): Ego_r 0x0084EE40 + FableWin 0x029E6D50 ??1LHHttp2@LHNetworkLib@@QAE@XZ and Ego_r 0x0084EB80 +
    // FableWin 0x029E7710 ?Close@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@XZ (0x00840070; the destructor's TU expands it, so the
    // includer supplies the body). Declarations only, no layout change.
    ~LHHttp2();                                                          // 0x008410D0
#endif
#ifdef FABLE_RETAIL_LHHTTP2_DTOR
    LH_RETURN Close();                                                   // 0x00840070
#endif
#ifdef FABLE_RETAIL_LHHTTP2_SENDREQUEST
    // NF123 opt-in (row 0x008405F0, q4193): Ego_r.pdb + FableWin.pdb ?SendRawData@LHHttp2@LHNetworkLib@@QAE?AW4LH_RETURN@2@PADKPAK_N@Z
    // (0x0083F8B0), Ego_r ?ResetRequest@LHHttp2@LHNetworkLib@@QAEXXZ (0x0083F980), Ego_r ?SendRequestAsync@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@XZ
    // (0x008405F0). Declarations only, no layout change.
    LH_RETURN SendRawData(char* data, unsigned long size, unsigned long* bytesSent, bool wait);   // 0x0083F8B0
    void ResetRequest();                                                 // 0x0083F980
    LH_HTTP_STATUS SendRequestAsync();                                   // 0x008405F0
#endif
#ifdef FABLE_RETAIL_LHHTTP2_RESET
    // NF123 opt-in (row 0x0083FF80, q4193): Ego_r.pdb 0x0084EA50 + FableWin.pdb 0x029E6E20 ?Reset@LHHttp2@LHNetworkLib@@QAEXXZ and
    // Ego_r 0x0084DA50 + FableWin 0x029E6FE0 ?ResetDocumentParts@LHHttp2@LHNetworkLib@@QAEXXZ (0x0083EB00, out of line). Declarations only.
    void ResetDocumentParts();                                           // 0x0083EB00
    void Reset();                                                        // 0x0083FF80
#endif
    LH_HTTP_STATUS PrepareRequest(HTTP_REQUEST_TYPE,const std::string&,LHHttpHeaders*,const std::string&,unsigned long);
    LH_HTTP_STATUS ReceiveHeaderAsync();
#ifdef FABLE_RETAIL_LHHTTP2_GETHEADER
    // Nested opt-in (chunk 865): Ego_r.pdb ?GetHeader@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@AAULHHttpHeaderStatus@2@@Z (landed 0x00840B60).
    LH_HTTP_STATUS GetHeader(struct LHHttpHeaderStatus& status);        // 0x00840B60
#endif
#ifdef FABLE_RETAIL_LHHTTP2_RECEIVEDOCUMENT
    // Nested opt-in (chunk 854, row 0x0083EFE0): Ego_r.pdb + FableWin.pdb spellings; the three callees out of line in retail and landed.
    LH_HTTP_STATUS ReceiveDocumentAsync();
    LH_HTTP_STATUS GetUntilCRLF(char*,unsigned long*);                   // 0x0083EEE0
    LH_RETURN IsDataAvailable();                                         // 0x0083EA50
    void ResetDocumentRecv();                                            // 0x0083EB90
#endif
    void ResetCurrentHeaderPart() { CurrentHeaderPos=0;memset(CurrentHeaderPart,0,sizeof CurrentHeaderPart); }
    void ResetCurrentDocumentPart() {
        DocumentEstimatedLength=0;DocumentLength=0;memset(CurrentDocumentPart,0,sizeof CurrentDocumentPart);
        CurrentDocumentPos=0;InChunkMode=false;
        if(CurrentDocumentChunk) { delete CurrentDocumentChunk;CurrentDocumentChunk=0; }
    }
};
bool __fastcall StartsWith(const std::string&,const std::string&);
}
#ifdef _MSC_VER
typedef char LHHttp2SizeCheck[sizeof(LHNetworkLib::LHHttp2)==0x10a8?1:-1];
#endif
#endif

#else
#ifndef FABLE_ENGINE_LHHTTP2_H
#define FABLE_ENGINE_LHHTTP2_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof LHHttp2 = 0x1104.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof LHHttp2 = 0x10a8.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct LHHttpDocumentParts;
struct LSocket;

#pragma pack(push, 1)
struct LHHttp2 {
    LSocket*             Host;                               // +0x00
    unsigned char        Hostname[0xc];                      // +0x04 basic_string<char,std::char_traits<char>,std::allocator<char>_>
    unsigned short       Port;                               // +0x10
    unsigned char        _pad_0x12[0x2];                     // +0x12
    unsigned char        PreparedRequest[0xc];               // +0x14 basic_string<char,std::char_traits<char>,std::allocator<char>_>
    unsigned long        RequestBytesSent;                   // +0x20
    unsigned long        RequestLength;                      // +0x24
    unsigned char        MyHeaders[0x18];                    // +0x28 LHHttpHeaders
    unsigned char        ReceivedHeaders[0x18];              // +0x40 LHHttpHeaders
    unsigned long        TimeOutValue;                       // +0x58
    unsigned long        ActionStartTime;                    // +0x5c
    bool                 ActionStarted;                      // +0x60
    unsigned char        _pad_0x61[0x1];                     // +0x61
    unsigned short       CurrentAction;                      // +0x62
    unsigned long        CurrDownSize;                       // +0x64
    unsigned long        Forwardings;                        // +0x68
    unsigned long        CurrForwarding;                     // +0x6c
    char                 CurrentHeaderPart[2048];            // +0x70
    unsigned long        CurrentHeaderPos;                   // +0x870
    unsigned char        ProxyHost[0xc];                     // +0x874 basic_string<char,std::char_traits<char>,std::allocator<char>_>
    unsigned long        ProxyPort;                          // +0x880
    unsigned char        DocumentParts[0xc];                 // +0x884 vector<LHNetworkLib::LHHttpDocumentParts_*,std::allocator<LHNetworkLib::LHHttpDocumentParts_*>_>
    unsigned char        DocumentEncoding[0x4];              // +0x890 HTTP_TRANSFER_ENCODING
    unsigned long        DocumentEstimatedLength;            // +0x894
    unsigned long        DocumentLength;                     // +0x898
    char                 CurrentDocumentPart[2048];          // +0x89c
    unsigned long        CurrentDocumentPos;                 // +0x109c
    bool                 InChunkMode;                        // +0x10a0
    unsigned char        _pad_0x10a1[0x3];                   // +0x10a1
    LHHttpDocumentParts* CurrentDocumentChunk;               // +0x10a4
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(158, sizeof(LHHttp2) == 0x10a8);
FABLE_STATIC_ASSERT_AT(159, offsetof(LHHttp2, Host) == 0x0);
FABLE_STATIC_ASSERT_AT(160, offsetof(LHHttp2, Hostname) == 0x4);
FABLE_STATIC_ASSERT_AT(161, offsetof(LHHttp2, Port) == 0x10);
FABLE_STATIC_ASSERT_AT(162, offsetof(LHHttp2, PreparedRequest) == 0x14);
FABLE_STATIC_ASSERT_AT(163, offsetof(LHHttp2, RequestBytesSent) == 0x20);
FABLE_STATIC_ASSERT_AT(164, offsetof(LHHttp2, RequestLength) == 0x24);
FABLE_STATIC_ASSERT_AT(165, offsetof(LHHttp2, MyHeaders) == 0x28);
FABLE_STATIC_ASSERT_AT(166, offsetof(LHHttp2, ReceivedHeaders) == 0x40);
FABLE_STATIC_ASSERT_AT(167, offsetof(LHHttp2, TimeOutValue) == 0x58);
FABLE_STATIC_ASSERT_AT(168, offsetof(LHHttp2, ActionStartTime) == 0x5c);
FABLE_STATIC_ASSERT_AT(169, offsetof(LHHttp2, ActionStarted) == 0x60);
FABLE_STATIC_ASSERT_AT(170, offsetof(LHHttp2, CurrentAction) == 0x62);
FABLE_STATIC_ASSERT_AT(171, offsetof(LHHttp2, CurrDownSize) == 0x64);
FABLE_STATIC_ASSERT_AT(172, offsetof(LHHttp2, Forwardings) == 0x68);
FABLE_STATIC_ASSERT_AT(173, offsetof(LHHttp2, CurrForwarding) == 0x6c);
FABLE_STATIC_ASSERT_AT(174, offsetof(LHHttp2, CurrentHeaderPart) == 0x70);
FABLE_STATIC_ASSERT_AT(175, offsetof(LHHttp2, CurrentHeaderPos) == 0x870);
FABLE_STATIC_ASSERT_AT(176, offsetof(LHHttp2, ProxyHost) == 0x874);
FABLE_STATIC_ASSERT_AT(177, offsetof(LHHttp2, ProxyPort) == 0x880);
FABLE_STATIC_ASSERT_AT(178, offsetof(LHHttp2, DocumentParts) == 0x884);
FABLE_STATIC_ASSERT_AT(179, offsetof(LHHttp2, DocumentEncoding) == 0x890);
FABLE_STATIC_ASSERT_AT(180, offsetof(LHHttp2, DocumentEstimatedLength) == 0x894);
FABLE_STATIC_ASSERT_AT(181, offsetof(LHHttp2, DocumentLength) == 0x898);
FABLE_STATIC_ASSERT_AT(182, offsetof(LHHttp2, CurrentDocumentPart) == 0x89c);
FABLE_STATIC_ASSERT_AT(183, offsetof(LHHttp2, CurrentDocumentPos) == 0x109c);
FABLE_STATIC_ASSERT_AT(184, offsetof(LHHttp2, InChunkMode) == 0x10a0);
FABLE_STATIC_ASSERT_AT(185, offsetof(LHHttp2, CurrentDocumentChunk) == 0x10a4);

#endif // FABLE_ENGINE_LHHTTP2_H

#endif
