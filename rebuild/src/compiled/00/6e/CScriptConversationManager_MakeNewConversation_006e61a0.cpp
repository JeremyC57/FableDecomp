// CScriptConversationManager::MakeNewConversation(CThing& person, bool soundIn2D, bool playDuringCutScenes) -- start a conversation under the next free index with its first person and add it to the conversation map; returns the index
// Leaf: MakeNewConversation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E61A0, length 275 bytes (`ret 0Ch`).
// FableWin.pdb: ?MakeNewConversation@CScriptConversationManager@@QAEJAAVCThing@@_N1@Z; the conversation ctor is
// ??0CScriptConversation@@QAE@AAVCScriptConversationManager@@JAAVCThing@@_N2@Z (inlined here).
// Callees: CScriptConversation::AddPersonToConversation(CThing&) @ 0x006E5790, vector<CIntelligentPointer<CThing> > copy ctor
// @ 0x0062ED40 and vector<CConversationLine> copy ctor @ 0x006E6A30 (the inline memberwise copy of the first pair), the
// out-of-line CScriptConversation copy ctor @ 0x006E6B60 (the second copy), map<long, CScriptConversation>::insert @ 0x006E6F70
// (pair<iterator, bool> through the hidden pointer), ~CScriptConversation @ 0x004AE0E0 (x3). The manifest labels of all six
// (CConversation::AddPersonToConversation, CCombatWheel::ResetRings, vector::CopyFrom, pair<long,CRegisteredMusicEntry>,
// CResourceMap_Insert, CMiniMapDisplay::ClearPrimitiveHandles) are propagated.
// Layout: retail CScriptConversation (12-byte vectors): Manager +0, ConversationIndex +4, PeopleInConversation +8, Conversation
// +0x14, CurrentLine +0x20, FramesLeftBeforeNewLine +0x24, SoundIn2D +0x28, StartedCurrentLineSpeech +0x29,
// PlayDuringCutScenes +0x2a (0x2c bytes); CScriptConversationManager: Conversations +0, NextConversationIndex +0xc.
// Shape: `insert(make_pair(index, conversation))`: the conversation local (inline ctor), the make_pair temporary (key, inline
// copy), the converted pair<const long, CScriptConversation> temporary (key, out-of-line copy ctor: VC inlines the implicit copy
// at its first use only), the insert result, then the three destructors in reverse order.
struct CThing;
struct CScriptConversationManager;

template<class T> struct CRetailVector {        // retail STLport vector: first, last, end of storage
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    CRetailVector(const CRetailVector& other);  // 0x0062ED40 (people) / 0x006E6A30 (lines)
};

struct CIntelligentPointer_Thing { void* __vftable; CThing* m_pItem; };
struct CConversationLine;

struct CScriptConversation {
    CScriptConversationManager*              Manager;                  // +0x00
    long                                     ConversationIndex;        // +0x04
    CRetailVector<CIntelligentPointer_Thing> PeopleInConversation;     // +0x08
    CRetailVector<CConversationLine>         Conversation;             // +0x14
    long                                     CurrentLine;              // +0x20
    long                                     FramesLeftBeforeNewLine;  // +0x24
    bool                                     SoundIn2D;                // +0x28
    bool                                     StartedCurrentLineSpeech; // +0x29
    bool                                     PlayDuringCutScenes;      // +0x2a
    CScriptConversation(CScriptConversationManager& manager, long index, CThing& person, bool soundIn2D, bool playDuringCutScenes)
        : Manager(&manager), ConversationIndex(index), CurrentLine(-1), FramesLeftBeforeNewLine(0), SoundIn2D(soundIn2D),
          StartedCurrentLineSpeech(false), PlayDuringCutScenes(playDuringCutScenes)
    {
        AddPersonToConversation(person);
    }
    ~CScriptConversation();                                            // 0x004AE0E0
    void AddPersonToConversation(CThing& person);                      // 0x006E5790
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    template<class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) { }
};
template<class T1, class T2> inline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }
}

struct SConversationNode;
struct CConversationMap {                       // STLport map<long, CScriptConversation>: header node, count, comparator
    struct iterator { SConversationNode* _M_node; };
    SConversationNode* _M_header;               // +0x00
    unsigned long      _M_node_count;           // +0x04
    unsigned long      _M_key_compare;          // +0x08
    std::pair<iterator, bool> insert(const std::pair<const long, CScriptConversation>& value);   // 0x006E6F70
};

struct CScriptConversationManager {
    CConversationMap Conversations;             // +0x00
    long             NextConversationIndex;     // +0x0c
    bool             AlwaysShowText;            // +0x10
    long MakeNewConversation(CThing& person, bool soundIn2D, bool playDuringCutScenes);
};

long CScriptConversationManager::MakeNewConversation(CThing& person, bool soundIn2D, bool playDuringCutScenes)
{
    long index = NextConversationIndex++;
    CScriptConversation conversation(*this, index, person, soundIn2D, playDuringCutScenes);
    Conversations.insert(std::make_pair(index, conversation));
    return index;
}