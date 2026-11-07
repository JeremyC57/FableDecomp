// LHNetworkLib::LHHttp2::ReceiveDocumentAsync(void) -- receive the next piece of the response body: a plain document appends each received block as a new document part, a chunked one reads the chunk-size line and then fills the chunk's buffer; time out after TimeOutValue seconds of silence
// Leaf: ?ReceiveDocumentAsync@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@XZ
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_RECEIVEDOCUMENT
#include <new>
void __cdecl operator delete[](void*);
#include "engine/LHHttp2.h"
#include "engine/LSocket.h"
#include <time.h>
#include <stdio.h>
#include "stlport/fable_string_bodies.h"
namespace LHNetworkLib {
LH_HTTP_STATUS LHHttp2::ReceiveDocumentAsync()
{
    LH_HTTP_STATUS returnstatus = HTTP_STATUS_RECEIVING_DOCUMENT;
    if (DocumentEncoding == CHUNKED) {
        if (!InChunkMode) {
            LH_HTTP_STATUS status = GetUntilCRLF(CurrentDocumentPart, &CurrentDocumentPos);
            if (status == HTTP_STATUS_DOCUMENT_RECEIVED) {
                unsigned long chunksize = 0;
                InChunkMode = true;
                sscanf(CurrentDocumentPart, "%x", &chunksize);
                if (chunksize > 0) {
                    chunksize += 2;
                    CurrentDocumentChunk = new LHHttpDocumentParts;
                    CurrentDocumentChunk->Buffer = new char[chunksize];
                    memset(CurrentDocumentChunk->Buffer, 0, chunksize);
                    CurrentDocumentChunk->BufferLength = chunksize;
                } else {
                    ActionStarted = false;
                    returnstatus = HTTP_STATUS_DOCUMENT_RECEIVED;
                }
                memset(CurrentDocumentPart, 0, sizeof CurrentDocumentPart);
                CurrentDocumentPos = 0;
                return returnstatus;
            }
            if (status == HTTP_STATUS_TIMEOUT_ERROR)
                return status;
            return returnstatus;
        }
        if (CurrentDocumentChunk == 0)
            return HTTP_STATUS_DOCUMENT_RECEIVED;
        if (CurrentDocumentChunk->CurrBufPos >= CurrentDocumentChunk->BufferLength) {
            CurrentDocumentChunk->BufferLength -= 2;
            DocumentParts.push_back(CurrentDocumentChunk);
            InChunkMode = false;
            return returnstatus;
        }
        if (IsDataAvailable() == LH_OK) {
            std::vector<char> tmpbuffer;
            tmpbuffer.resize(CurrentDocumentChunk->BufferLength - CurrentDocumentChunk->CurrBufPos);
            if (!Host->Receive(tmpbuffer))
                return HTTP_STATUS_RECEIVING_ERROR;
            unsigned long receivesize = Host->NrLastDataReceived;
            memcpy(CurrentDocumentChunk->Buffer + CurrentDocumentChunk->CurrBufPos, &tmpbuffer[0], receivesize);
            CurrentDocumentChunk->CurrBufPos += receivesize;
            CurrDownSize += receivesize;
            time((time_t*)&ActionStartTime);
            return returnstatus;
        }
        if (!ActionStarted) {
            time((time_t*)&ActionStartTime);
            ActionStarted = true;
            return returnstatus;
        }
        if ((unsigned long)time(0) > ActionStartTime + TimeOutValue) {
            if (CurrentDocumentChunk) {
                delete CurrentDocumentChunk;
                CurrentDocumentChunk = 0;
            }
            ResetDocumentRecv();
            returnstatus = HTTP_STATUS_TIMEOUT_ERROR;
        }
        return returnstatus;
    }
    if (Host && Host->IsReadable(0)) {
        unsigned long receivesize = 0;
        std::vector<char> tmpbuffer;
        tmpbuffer.resize(receivesize);
        Host->Receive(tmpbuffer);
        receivesize = Host->NrLastDataReceived;
        if (receivesize == 0) {
            ActionStarted = false;
            returnstatus = HTTP_STATUS_DOCUMENT_RECEIVED;
        } else {
            CurrentDocumentChunk = new LHHttpDocumentParts;
            CurrentDocumentChunk->Buffer = new char[receivesize];
            memset(CurrentDocumentChunk->Buffer, 0, receivesize);
            memcpy(CurrentDocumentChunk->Buffer, &tmpbuffer[0], Host->NrLastDataReceived);
            CurrentDocumentChunk->BufferLength = receivesize;
            CurrDownSize += receivesize;
            DocumentParts.push_back(CurrentDocumentChunk);
            time((time_t*)&ActionStartTime);
        }
        return returnstatus;
    }
    if (!ActionStarted) {
        time((time_t*)&ActionStartTime);
        ActionStarted = true;
        return returnstatus;
    }
    if ((unsigned long)time(0) > ActionStartTime + TimeOutValue) {
        if (CurrentDocumentChunk) {
            delete CurrentDocumentChunk;
            CurrentDocumentChunk = 0;
        }
        ResetDocumentRecv();
        returnstatus = HTTP_STATUS_TIMEOUT_ERROR;
    }
    return returnstatus;
}
}
