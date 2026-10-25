// CScriptConversation::CScriptConversation(const CScriptConversation& other) -- copy constructor of a scripted conversation: manager, index, the two vectors (people, lines), the line counters and the three flags
// Leaf: ??0CScriptConversation@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E6B60, 79 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<long,CMusicManager::CRegisteredMusicEntry>" is propagated. Ego_r.pdb
//   CScriptConversation: Manager +0, ConversationIndex +4, PeopleInConversation (vector<CIntelligentPointer<CThing> >) +8,
//   Conversation (vector<CScriptConversation::CConversationLine>) +0x14 with retail's 12-byte STLport vectors, CurrentLine
//   +0x20, FramesLeftBeforeNewLine +0x24, SoundIn2D +0x28, StartedCurrentLineSpeech +0x29, PlayDuringCutScenes +0x2A; the
//   PDB name sets have ??0CScriptConversation@@QAE@ABV0@@Z. Nine callers, all in CScriptConversationManager's TU (the
//   map<long, CScriptConversation> node construction).
// Callees: vector<CIntelligentPointer<CThing> >::vector(const vector&) @ 0x0062ED40,
//   vector<CScriptConversation::CConversationLine>::vector(const vector&) @ 0x006E6A30.
// Model: engine/CScriptConversation.h is not used: it carries Ego_r.pdb's 16-byte vectors (Conversation +0x18, CurrentLine
//   +0x28, ...), while retail's STLport vectors are 12 bytes (Conversation +0x14, CurrentLine +0x20).
// Shape: the memberwise copy constructor.
class CThing;
class CScriptConversationManager;

template <class T> class CIntelligentPointer {
public:
    void* VTable;                                       // +0x00 (vptr, 0x01238C6C for CThing)
    void* PData;                                        // +0x04
};

namespace std {
template <class _Tp> class allocator {
};
template <class _Tp, class _Alloc> class vector {     // 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector(const vector<_Tp, _Alloc>& __x);             // out of line: 0x0062ED40 / 0x006E6A30
};
}

class CScriptConversation {
public:
    class CConversationLine;
    CScriptConversationManager* Manager;                                            // +0x00
    long ConversationIndex;                                                         // +0x04
    std::vector<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > > PeopleInConversation;  // +0x08
    std::vector<CConversationLine, std::allocator<CConversationLine> > Conversation;                               // +0x14
    long CurrentLine;                                                               // +0x20
    long FramesLeftBeforeNewLine;                                                   // +0x24
    bool SoundIn2D;                                                                 // +0x28
    bool StartedCurrentLineSpeech;                                                  // +0x29
    bool PlayDuringCutScenes;                                                       // +0x2A
    CScriptConversation(const CScriptConversation& other);
};

CScriptConversation::CScriptConversation(const CScriptConversation& other)
    : Manager(other.Manager), ConversationIndex(other.ConversationIndex),
      PeopleInConversation(other.PeopleInConversation), Conversation(other.Conversation),
      CurrentLine(other.CurrentLine), FramesLeftBeforeNewLine(other.FramesLeftBeforeNewLine),
      SoundIn2D(other.SoundIn2D), StartedCurrentLineSpeech(other.StartedCurrentLineSpeech),
      PlayDuringCutScenes(other.PlayDuringCutScenes)
{
}