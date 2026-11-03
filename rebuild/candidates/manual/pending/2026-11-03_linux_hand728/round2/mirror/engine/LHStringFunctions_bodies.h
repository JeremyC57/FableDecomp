#ifndef FABLE_HTTP_STRING_FUNCTIONS_BODIES_H
#define FABLE_HTTP_STRING_FUNCTIONS_BODIES_H
#include "LHHttp2.h"
#include <ctype.h>
namespace LHNetworkLib {
#pragma auto_inline(off)
std::string __fastcall toLowerCase(const std::string& text) {
    std::string result;int length=text.size();
    for(int i=0;i<length;++i) result+=(char)tolower(text[i]);
    return result;
}
bool __fastcall StartsWith(const std::string& text,const std::string& prefix) {
    return toLowerCase(text).substr(0,prefix.size())==toLowerCase(prefix);
}
#pragma auto_inline(on)
}
#endif
