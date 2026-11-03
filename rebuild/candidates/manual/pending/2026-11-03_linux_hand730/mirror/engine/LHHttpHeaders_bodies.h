#ifndef FABLE_HTTP_HEADER_BODIES_H
#define FABLE_HTTP_HEADER_BODIES_H
#include "LHHttp2.h"
namespace LHNetworkLib {
#pragma auto_inline(off)
void LHHttpHeaders::AddHeader(const std::string& name,const std::string& value) {
    std::string header(name);
    if(name[name.size()-1]!=':') header+=":";
    header+=" ";header+=value;
    HdrDocumentHeader.push_back(header);
}
#pragma auto_inline(on)
}
#endif
