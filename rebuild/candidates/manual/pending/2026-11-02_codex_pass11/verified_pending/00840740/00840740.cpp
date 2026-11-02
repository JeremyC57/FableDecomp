// LHNetworkLib::LHHttp2::ReceiveHeaderAsync(void) -- receive and parse the pending HTTP response headers.
// Leaf: ?ReceiveHeaderAsync@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@XZ
// Flags: /GS
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_RECEIVE_HEADER
#include <new>
void __cdecl operator delete[](void*);
#include "engine/LHHttp2.h"
#include "engine/LSocket.h"
#include <time.h>
#include "stlport/fable_string_bodies.h"
#include "engine/LHStringFunctions_bodies.h"
#include "engine/LHHttpHeaders_bodies.h"
// Layout: complete native LHHttp2 and LSocket fields, with original names.
namespace LHNetworkLib {
LH_HTTP_STATUS LHHttp2::ReceiveHeaderAsync()
{
    unsigned long receivesize=1;
    unsigned short bcounter;
    bool newline=false;
    LH_HTTP_STATUS returnstatus=HTTP_STATUS_RECEIVING_HEADER;
    if(CurrentAction!=1) return HTTP_STATUS_INCORRECT_ACTION;
    if(CurrentHeaderPos==0 && ReceivedHeaders.HdrDocumentHeader.empty() && !ActionStarted) {
        ActionStarted=true;
        time((time_t*)&ActionStartTime);
    }
    if(Host && Host->IsReadable(0)) {
        std::vector<char> tmpbuffer;
        bcounter=0;
        for(;bcounter<1024;++bcounter) {
            if(receivesize==0) break;
            tmpbuffer.resize(receivesize);
            if(!Host->Receive(tmpbuffer)) return HTTP_STATUS_TIMEOUT_ERROR;
            receivesize=Host->NrLastDataReceived;
            memcpy(CurrentHeaderPart+CurrentHeaderPos,&tmpbuffer[0],receivesize);
            if(receivesize!=0) {
                if(CurrentHeaderPart[CurrentHeaderPos]=='\r') newline=true;
                if(CurrentHeaderPart[CurrentHeaderPos]=='\n' && newline) {
                    CurrentHeaderPart[CurrentHeaderPos-1]=0;
                    if(strcmp(CurrentHeaderPart,"")==0) {
                        CurrentAction=2;
                        ActionStarted=false;
                        DocumentEstimatedLength=0;DocumentLength=0;
                        memset(CurrentDocumentPart,0,sizeof CurrentDocumentPart);
                        CurrentDocumentPos=0;InChunkMode=false;
                        if(CurrentDocumentChunk) { delete CurrentDocumentChunk;CurrentDocumentChunk=0; }
                        return HTTP_STATUS_HEADER_RECEIVED;
                    }
                    if(StartsWith(CurrentHeaderPart,"http/1."))
                        ReceivedHeaders.AddHeader("ServerCode",CurrentHeaderPart);
                    else
                        ReceivedHeaders.AddHeader(CurrentHeaderPart);
                    CurrentHeaderPos=0;
                    memset(CurrentHeaderPart,0,sizeof CurrentHeaderPart);
                } else CurrentHeaderPos+=receivesize;
            } else if((unsigned long)time(0)>ActionStartTime+TimeOutValue) {
                ReceivedHeaders.Reset();
                CurrentHeaderPos=0;
                DocumentEstimatedLength=0;
                DocumentLength=0;
                CurrentDocumentPos=0;
                memset(CurrentHeaderPart,0,sizeof CurrentHeaderPart);
                InChunkMode=false;
                returnstatus=HTTP_STATUS_TIMEOUT_ERROR;
            }
        }
        if(bcounter!=0) time((time_t*)&ActionStartTime);
    } else if((unsigned long)time(0)>ActionStartTime+TimeOutValue) {
        ReceivedHeaders.Reset();
        ResetCurrentHeaderPart();
        returnstatus=HTTP_STATUS_TIMEOUT_ERROR;
    }
    return returnstatus;
}
}
