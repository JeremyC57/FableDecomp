// CAsyncDecoder_00c48640::Run() -- the asynchronous decoder thread's request loop: take each 0x40-byte request from the +0x18 mailbox and serve it by type -- (2) close: delete the decoded stream and acknowledge; (3) seek: seek the stream, answer {ok, position, user data} (not ok without a stream or on a negative position); (4) read: read the frame's samples into it through a 16-bit sample cursor, store the count read (0 on error) and answer {frame, position, user data}; (1) open: create the decompressed stream from the request's opener, name and segment, answer {ok, channels, rate, unit size} (deleting the stream when any is invalid); (5) exit: note the reply queue -- until an exit arrives, then acknowledge it
// Leaf: ?Run@CAsyncDecoder_00c48640@@UAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C47F10, 784 bytes, thiscall, plain `ret`, ebp frame with `and esp,-8`, EH frame
//   (state 0 = the request, 1 = the sample cursor, 2 = the open request copy, 3 = the `new` temporary); 5-entry jump table
//   0x00C4820C over type - 1.
// Identity: libpk .\CASyncDecoder.cpp: slot 5 of the decoder vtable 0x012B8020 (the landed ctor 0x00C48640 names the class
//   CAsyncDecoder_00c48640); its requests are the 0x40-byte records the landed client methods 0x00C48300 / 0x00C483A0 /
//   0x00C48430 / 0x00C484B0 / 0x00C48560 build (types 3 / 4 / 5 / 1 / 2) and post through 0x00C47F00 to this +0x18 mailbox.
//   The manifest label "CTCHeroStats_ProcessStateTransition" is propagated and wrong. Descriptive names (no PDB / RTTI).
// Callees: CMailbox_00c48860::Take 0x00C48860 (by-value result; landed with a one-word result type); the request views
//   0x00C47EB0 (+4) / 0x00C47EC0 (+0x20) / 0x00C47ED0 (+0x24) / 0x00C47EE0 (+0x34) / 0x00C47EF0 (+0x3C) (landed under five
//   per-caller SActionRequest_* spellings: one class here); the reply queues' posts 0x00C48A00 / 0x00C48AA0 / 0x00C48B40 /
//   0x00C48BE0 / 0x00C48C80 (landed spellings); CPKDecompressedStream ctor 0x00C62920 (landed as CAudioStreamOwner_00c62920:
//   the same 0x14-byte class whose dtor 0x00C62750 and Seek 0x00C626A0 are landed as CPKDecompressedStream; NAME_FIXES),
//   Read 0x00C62790 and the accessors 0x00C62780 (position, -1 without a decoder), 0x00C628C0 (user data), 0x00C628A0 /
//   0x00C628D0 / 0x00C628F0 (decoder slots 1 / 4 / 5); C16BitSamplePtr 0x00C62A50 / ~ 0x00C62A20; CCodaFrame::GetData
//   0x00C491D0; the request's name pointer copy ctor 0x00C16900, Get 0x00C168A0, dtor 0x00C167C0; new 0x00BFEA1A / delete
//   0x00BFE9BC.
// Shape: cases in retail block order (2, 3, 4, 1, 5: VC7.1 lays the jump-table blocks out in source order); the close and
//   exit acknowledgements are 4-byte records, the exit one in its own block after the loop (its slot is shared with the
//   `new` temporary); the seek reply is posted from one statement reached by a `goto` from the success branch (VC7.1
//   duplicates it into both paths with one register choice); the open request is copied out of the request.
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

struct CIntelligentPointer_00c16900 {                       // 8 bytes
    void* m_p0;
    void* m_p4;
    CIntelligentPointer_00c16900(const CIntelligentPointer_00c16900& other);   // 0x00C16900
    ~CIntelligentPointer_00c16900();                        // 0x00C167C0
    const char* Get() const;                                // 0x00C168A0
};

struct SStreamSegment { unsigned long m_word0, m_word4, m_word8; };

class IStreamOpener_00c62920;
class C16BitSamplePtr { public:
    short* m_pSamples;
    long   m_channels;
    C16BitSamplePtr(short* pSamples, long channels);        // 0x00C62A50
    ~C16BitSamplePtr();                                     // 0x00C62A20
};
struct CCodaFrame {
    unsigned long m_channels;                               // +0x00
    unsigned long m_samples;                                // +0x04
    unsigned long m_validSamples;                           // +0x08
    short* GetData();                                       // 0x00C491D0
};
class CPKDecompressedStream { public:                      // 0x14 bytes
    unsigned char _body[0x14];
    CPKDecompressedStream(IStreamOpener_00c62920* opener, const char* name, const SStreamSegment& segment);   // 0x00C62920
    ~CPKDecompressedStream();                               // 0x00C62750
    long Seek(long mode, float amount, long userData);      // 0x00C626A0
    long GetPosition();                                     // 0x00C62780
    long Read(C16BitSamplePtr* dst, unsigned long samples); // 0x00C62790
    long GetUnitSize();                                     // 0x00C628A0
    long GetUserData();                                     // 0x00C628C0
    long GetRate();                                         // 0x00C628D0
    long GetChannels();                                     // 0x00C628F0
};

struct CMailboxQueue_00c48a00 { long ProcessMessage_CMailboxQueue_00c48a00(void* message); };   // 0x00C48A00
struct CMailboxQueue_00c48aa0 { long ProcessMessage_CMailboxQueue_00c48aa0(void* message); };   // 0x00C48AA0
struct CMailboxQueue_00c48b40 { long ProcessMessage_CMailboxQueue_00c48b40(void* message); };   // 0x00C48B40
struct CMailboxQueue_00c48be0 { long ProcessMessage_CMailboxQueue_00c48be0(void* message); };   // 0x00C48BE0
struct CMailboxQueue_00c48c80 { long ProcessMessage(void* message); };                          // 0x00C48C80

struct SOpenArgs {                                          // the request viewed from +4
    IStreamOpener_00c62920*      m_opener;                 // +0x00
    SStreamSegment               m_segment;                 // +0x04
    CIntelligentPointer_00c16900 m_name;                    // +0x10
    CMailboxQueue_00c48be0*      m_reply;                   // +0x18
};
struct SSeekArgs { long m_mode; float m_amount; long m_userData; CMailboxQueue_00c48aa0* m_reply; };
struct SReadArgs { CCodaFrame* m_frame; CMailboxQueue_00c48b40* m_reply; };

struct SDecoderRequest {                                    // 0x40 bytes
    long                         m_type;                    // +0x00
    unsigned char                _04[0x10];
    CIntelligentPointer_00c16900 m_name;                    // +0x14
    unsigned char                _1c[0x40 - 0x1c];
    SOpenArgs& OpenArgs();                                  // 0x00C47EB0 (+0x04)
    CMailboxQueue_00c48a00*& CloseReply();                  // 0x00C47EC0 (+0x20)
    SSeekArgs& SeekArgs();                                  // 0x00C47ED0 (+0x24)
    SReadArgs& ReadArgs();                                  // 0x00C47EE0 (+0x34)
    CMailboxQueue_00c48c80*& ExitReply();                   // 0x00C47EF0 (+0x3c)
};

struct CMailbox_00c48860 {
    SDecoderRequest Take();                                 // 0x00C48860
};

struct SCloseReply { long m_result; };
struct SSeekReply { bool m_ok; long m_position; long m_userData; };
struct SReadReply { CCodaFrame* m_frame; long m_position; long m_userData; };
struct SOpenReply { bool m_ok; long m_channels; long m_rate; long m_unitSize; };
struct SExitReply { long m_result; };

class CAsyncDecoder_00c48640 { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Run();                                     // slot 5
    unsigned char     _pad04[0x18 - 4];
    CMailbox_00c48860 m_requests;                           // +0x18
};

void CAsyncDecoder_00c48640::Run()
{
    CMailboxQueue_00c48c80* exitReply = 0;
    CPKDecompressedStream* stream = 0;
    do {
        SDecoderRequest request = m_requests.Take();
        switch (request.m_type) {
        case 2: {
            CMailboxQueue_00c48a00*& reply = request.CloseReply();
            delete stream;
            stream = 0;
            SCloseReply ack;
            reply->ProcessMessage_CMailboxQueue_00c48a00(&ack);
            break;
        }
        case 3: {
            SSeekArgs& args = request.SeekArgs();
            SSeekReply reply;
            if (stream) {
                stream->Seek(args.m_mode, args.m_amount, args.m_userData);
                long position = stream->GetPosition();
                if (position >= 0) {
                    reply.m_ok = true;
                    reply.m_position = position;
                    reply.m_userData = stream->GetUserData();
                    goto send;
                }
            }
            reply.m_ok = false;
        send:
            args.m_reply->ProcessMessage_CMailboxQueue_00c48aa0(&reply);
            break;
        }
        case 4: {
            SReadArgs& args = request.ReadArgs();
            long position = stream->GetPosition();
            long read = -1;
            if (position >= 0) {
                C16BitSamplePtr dst(args.m_frame->GetData(), args.m_frame->m_channels);
                read = stream->Read(&dst, args.m_frame->m_samples);
                if (read >= 0)
                    position = stream->GetPosition();
            }
            args.m_frame->m_validSamples = read < 0 ? 0 : read;
            SReadReply reply;
            reply.m_position = position < 0 ? 0 : position;
            reply.m_frame = args.m_frame;
            reply.m_userData = stream->GetUserData();
            args.m_reply->ProcessMessage_CMailboxQueue_00c48b40(&reply);
            break;
        }
        case 1: {
            SOpenArgs args = request.OpenArgs();
            stream = new CPKDecompressedStream(args.m_opener, args.m_name.Get(), args.m_segment);
            long channels = stream->GetChannels();
            long rate = stream->GetRate();
            long unitSize = stream->GetUnitSize();
            SOpenReply reply;
            if (channels >= 1 && rate >= 1 && unitSize >= 0) {
                reply.m_ok = true;
                reply.m_channels = channels;
                reply.m_rate = rate;
                reply.m_unitSize = unitSize;
            } else {
                reply.m_ok = false;
                delete stream;
                stream = 0;
            }
            args.m_reply->ProcessMessage_CMailboxQueue_00c48be0(&reply);
            break;
        }
        case 5:
            exitReply = request.ExitReply();
            break;
        }
    } while (!exitReply);
    {
        SExitReply ack;
        exitReply->ProcessMessage(&ack);
    }
}