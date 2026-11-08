// PROBE (chunk 865, not for landing): can the shared-header model carry the landed 0x00840B60 LHHttp2::GetHeader?
// Leaf: ?GetHeader@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@AAULHHttpHeaderStatus@2@@Z
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_GETHEADER
#define FABLE_RETAIL_STLP_STRING_FIND
#define FABLE_RETAIL_STLP_STRING_THROW_INLINE
#include "engine/LHHttp2.h"
#include "engine/LHStringFunctions_bodies.h"
namespace LHNetworkLib {
struct LHHttpHeaderStatus {                              // retail 0x48 bytes; engine/LHHttpHeaderStatus.h is the opaque generated layout
    LH_HTTP_SERVER_CODES hsServerCode;                   // +0x00
    LHHttpHeaders        hsDocumentHeader;               // +0x04
    std::string          hsLocation;                     // +0x1c
    long                 hsDocumentSize;                 // +0x28
};
#pragma auto_inline(off)
std::string LHHttpHeaders::GetHeader(const std::string& name)
{
    for (std::vector<std::string>::iterator it = HdrDocumentHeader.begin(); it != HdrDocumentHeader.end(); ++it) {
        std::string line = *it;
        if (StartsWith(line, name)) {
            int colon = line.find(":");
            if (colon > 0)
                return line.substr(line.find_first_not_of(" ", colon + 1));
            return line;
        }
    }
    return "";
}
#pragma auto_inline(on)
LH_HTTP_STATUS LHHttp2::GetHeader(LHHttpHeaderStatus& status)
{
    if (!ReceivedHeaders.HdrDocumentHeader.empty()) {
        for (std::vector<std::string>::iterator it = ReceivedHeaders.HdrDocumentHeader.begin();
             it != ReceivedHeaders.HdrDocumentHeader.end(); ++it)
            status.hsDocumentHeader.HdrDocumentHeader.push_back(*it);
        std::string serverCode = status.hsDocumentHeader.GetHeader("ServerCode");
        if (serverCode != "") {
            int code = atoi(serverCode.substr(serverCode.find(" ") + 1).c_str());
            status.hsServerCode = (LH_HTTP_SERVER_CODES)code;
        }
        std::string location = status.hsDocumentHeader.GetHeader("Location:");
        if (location != "")
            status.hsLocation = location;
        DocumentEncoding = NORMAL;
        std::string contentLength = status.hsDocumentHeader.GetHeader("Content-Length");
        if (contentLength != "")
            status.hsDocumentSize = atol(contentLength.c_str());
        else
            status.hsDocumentSize = -1;
        std::string transferEncoding = status.hsDocumentHeader.GetHeader("Transfer-Encoding");
        if (transferEncoding != "") {
            if (StartsWith(transferEncoding, "chunked"))
                DocumentEncoding = CHUNKED;
        }
    }
    return HTTP_STATUS_OK;
}
}
