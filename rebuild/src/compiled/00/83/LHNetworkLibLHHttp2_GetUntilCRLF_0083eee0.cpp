// LHNetworkLib::LHHttp2::GetUntilCRLF(char* buffer, unsigned long* offset) -- read the host socket one byte at a time into buffer at *offset until a CR LF pair (the CR is then replaced by the terminator); at most 1024 bytes per call; RECEIVING_DOCUMENT when no line ended, TIMEOUT_ERROR when a receive fails
// Leaf: ?GetUntilCRLF@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@PADPAK@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083EEE0, thiscall `ret 8`. Identity: Ego_r.pdb 0x0084DB20 + FableWin.pdb 0x029E8E50
//   ?GetUntilCRLF@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@PADPAK@Z; its only caller, the landed LHHttp2::ReceiveDocumentAsync
//   0x0083EFE0, binds that spelling.
// Callees: LSocket::IsReadable(long) 0x008F8210 (with 0), LSocket::Receive(std::vector<char>&) 0x008F99A0, vector<char>::_M_fill_insert
//   0x004110D0 (the inline resize(1) on the empty vector), free 0x00BFEA14 (~vector).
// Naming (NF180 / NF184, 2026-11-20, q4190): was a free __fastcall stand-in ?LHHttp2_GetUntilCRLF@@YI... over a local LSocket and a
//   Packet_0083eee0 byte vector (its own Insert body bound to 0x004110D0); now the member on the shared engine/LHHttp2.h and
//   engine/LSocket.h, so Receive binds the PDB spelling ?Receive@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@@Z.
// Shape: FABLE_RETAIL_STLP_VECTOR (STLport's _vector.c body of _M_fill_insert visible, as in every retail TU that includes
//   _vector.h; the call stays out of line): with the declaration alone the resize(1) char() temporary is homed in the dead `offset`
//   argument slot, with the body it gets its own frame byte next to foundCR (0x0083EF14 `mov [esp+13h],bl`), as retail.
//   FABLE_RETAIL_STLP_FILL_CHAR (STLport 4.6 _algobase.h's char fill / fill_n memset overloads) and
//   FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22 (its guarded __copy_trivial_backward) make the _M_fill_insert<char>
//   COMDAT this TU must emit the retail body: it grades RELOCATION_MATCH against 0x004110D0 (289 bytes; without them 266 bytes,
//   a different body). Declaration-only (the header body hidden, or an extern template) loses the frame byte above: DIFFER(255v255).
//   The other COMDATs it emits that the binding table owns elsewhere grade RELOCATION_MATCH at their rows too: __copy_trivial
//   0x00405BA0, __copy_trivial_backward 0x00405BC0, resize(n) 0x00412880, resize(n, x) 0x004122D0 (NF184 pick-any check).
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_RECEIVEDOCUMENT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22
#include <new>
#include "engine/LHHttp2.h"
#include "engine/LSocket.h"
namespace LHNetworkLib {
LH_HTTP_STATUS LHHttp2::GetUntilCRLF(char* buffer, unsigned long* offset)
{
    bool foundCR = false;
    if (Host && Host->IsReadable(0)) {
        unsigned short count = 0;
        do {
            std::vector<char> tmpbuffer;
            tmpbuffer.resize(1);
            if (!Host->Receive(tmpbuffer))
                return HTTP_STATUS_TIMEOUT_ERROR;
            buffer[*offset] = tmpbuffer[0];
            if (buffer[*offset] == '\r')
                foundCR = true;
            if (buffer[*offset] == '\n' && foundCR) {
                buffer[*offset - 1] = 0;
                return HTTP_STATUS_DOCUMENT_RECEIVED;
            }
            ++*offset;
        } while (++count < 1024);
    }
    return HTTP_STATUS_RECEIVING_DOCUMENT;
}
}
