// LHNetworkLib::LHHttp2::ReceiveDocumentAsync(void) -- receive the next piece of the response body: a plain document appends each received block as a new document part, a chunked one reads the chunk-size line and then fills the chunk's buffer; time out after TimeOutValue seconds of silence
// Leaf: ?ReceiveDocumentAsync@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083EFE0, 1074 bytes, thiscall, plain ret.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?ReceiveDocumentAsync@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@XZ; the body reads the
//   retail LHHttp2 fields of engine/LHHttp2.h (Host +0, TimeOutValue +0x58, ActionStartTime +0x5C, ActionStarted +0x60, CurrDownSize +0x64,
//   DocumentParts +0x884, DocumentEncoding +0x890, CurrentDocumentPart +0x89C, CurrentDocumentPos +0x109C, InChunkMode +0x10A0,
//   CurrentDocumentChunk +0x10A4) and is the only caller of the push_back 0x00841A70.
// Callees: LSocket::IsReadable(long) 0x008F8210 / Receive(std::vector<char>&) 0x008F99A0 (NrLastDataReceived +0x94),
//   vector<char>::_M_fill_insert 0x004110D0 (the inline resize(0)) / resize(size_type) 0x00412880, operator new 0x00BFEA1A / new[] 0x00BFEB22 /
//   delete 0x00BFE9BC / delete[] 0x00BFEB1C, vector<LHHttpDocumentParts*>::push_back 0x00841A70, _time 0x00BFED22, sscanf 0x00BFED1C ("%x"),
//   LHHttp2::GetUntilCRLF 0x0083EEE0 / IsDataAvailable 0x0083EA50 / ResetDocumentRecv 0x0083EB90, free 0x00BFEA14.
// Shape: one `switch (DocumentEncoding)` (`dec eax ; je` to the CHUNKED case, the plain body as default) and one `return returnstatus`
//   exit (VC7.1 duplicates the epilogue and keeps returnstatus in its frame slot, `mov eax,[esp+18h]`); only the CRLF timeout and the
//   chunk receive failure return directly. LHHttpDocumentParts' inline default ctor zeroes the three words after `new`.
// Header overlay: engine/LHHttp2.h FABLE_RETAIL_LHHTTP2_RECEIVEDOCUMENT (opt-in; the ctor body and four member declarations), with an
//   unchanged copy of engine/LHNetworkTypes.h beside it so the overlay copy's `#include "LHNetworkTypes.h"` resolves.
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_RECEIVEDOCUMENT
#include <new>
void* __cdecl operator new[](unsigned int);
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
    switch (DocumentEncoding) {
    case CHUNKED:
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
            } else if (status == HTTP_STATUS_TIMEOUT_ERROR) {
                return status;
            }
        } else if (CurrentDocumentChunk) {
            if (CurrentDocumentChunk->CurrBufPos < CurrentDocumentChunk->BufferLength) {
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
                } else if (!ActionStarted) {
                    time((time_t*)&ActionStartTime);
                    ActionStarted = true;
                } else if ((unsigned long)time(0) > ActionStartTime + TimeOutValue) {
                    if (CurrentDocumentChunk) {
                        delete CurrentDocumentChunk;
                        CurrentDocumentChunk = 0;
                    }
                    ResetDocumentRecv();
                    returnstatus = HTTP_STATUS_TIMEOUT_ERROR;
                }
            } else {
                CurrentDocumentChunk->BufferLength -= 2;
                DocumentParts.push_back(CurrentDocumentChunk);
                InChunkMode = false;
            }
        } else {
            returnstatus = HTTP_STATUS_DOCUMENT_RECEIVED;
        }
        break;
    default:
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
        } else if (!ActionStarted) {
            time((time_t*)&ActionStartTime);
            ActionStarted = true;
        } else if ((unsigned long)time(0) > ActionStartTime + TimeOutValue) {
            if (CurrentDocumentChunk) {
                delete CurrentDocumentChunk;
                CurrentDocumentChunk = 0;
            }
            ResetDocumentRecv();
            returnstatus = HTTP_STATUS_TIMEOUT_ERROR;
        }
        break;
    }
    return returnstatus;
}
}