// LHNetworkLib::LHHttp2::PrepareRequest(LHNetworkLib::HTTP_REQUEST_TYPE,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,LHNetworkLib::LHHttpHeaders *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,unsigned long) -- construct the HTTP request and its headers.
// Leaf: ?PrepareRequest@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@W4HTTP_REQUEST_TYPE@2@ABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@PAVLHHttpHeaders@2@1K@Z
// Flags: /GS
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_LHHTTP2
#include "engine/LHHttp2.h"
#include <stdlib.h>
#include "stlport/fable_string_bodies.h"
#include "engine/LHHttpHeaders_bodies.h"
// Layout: engine/LHHttp2.h restores all PDB fields with native STLport widths.
namespace LHNetworkLib {
LH_HTTP_STATUS LHHttp2::PrepareRequest(HTTP_REQUEST_TYPE Method,const std::string& URI,LHHttpHeaders* requestheaders,const std::string& body,unsigned long bodylength)
{
    std::string requesttype;
    std::string reqheaderstring="";
    std::string myheaderstring="";
    PreparedRequest="";
    RequestBytesSent=0;
    switch(Method) {
    case GET: requesttype="GET ";break;
    case PUT: requesttype="PUT ";break;
    case POST: requesttype="POST ";break;
    default: requesttype="GET ";break;
    }
    if(ProxyHost!="") {
        requesttype+="HTTP://"+Hostname;
        if(Port!=80) {
            requesttype+=":";
            char number[20];
            _itoa(Port,number,10);
            requesttype+=number;
        }
        requesttype+="/";
    }
    requesttype+=URI;
    requesttype+=" HTTP/1.1\r\n";
    if(requestheaders) reqheaderstring=requestheaders->GetHeaderString();
    MyHeaders.AddHeader("Host",Hostname);
    MyHeaders.AddHeader("Connection: close");
    if(body!="") {
        std::string content_length_str="Content-Length: ";
        char number[20];
        _itoa(bodylength,number,10);
        content_length_str+=number;
        MyHeaders.AddHeader(content_length_str);
    }
    myheaderstring=MyHeaders.GetHeaderString();
    PreparedRequest=requesttype;
    if(reqheaderstring!="") PreparedRequest+=reqheaderstring;
    PreparedRequest+=myheaderstring;
    PreparedRequest+="\r\n";
    RequestLength=PreparedRequest.size();
    if(body!="") {
        PreparedRequest+=body;
        RequestLength+=body.size();
    }
    MyHeaders.DeleteHeaderString();
    if(requestheaders) requestheaders->DeleteHeaderString();
    return HTTP_STATUS_DOCUMENT_PREPARED;
}
}
