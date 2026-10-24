// CScriptConversation::AddLineToConversation(const CThing& speaker, const CThing* listener, const CCharString& line, bool showSubtitle) -- append a line (speaker index, listener index or -1, text, subtitle flag) when the speaker is in the conversation
// Leaf: ?AddLineToConversation@CScriptConversation@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E58A0, 163 bytes (176 with int3 padding), thiscall, `ret 10h`.
// Identity: Ego_r.pdb name set `?AddLineToConversation@CScriptConversation@@QAEXABVCThing@@PBV2@ABVCCharString@@_N@Z`;
//   the landed CScriptConversationManager::AddLineToConversation 0x006E5950 forwards to this address on &it->second.
//   The catalog's "CConversation_AddLineToConversation" has the wrong class.
// Callees: STLport __find over the people vector @ 0x006E67A0 (landed; /Gr __fastcall first/last in ecx/edx, the value
//   and the random-access tag by const reference), CCharString copy ctor @ 0x0099EC30, the line vector's
//   push_back(const CConversationLine&) @ 0x006E6D30, CCharString dtor @ 0x0099EAE0.
// Layout: retail STLport vectors (12 bytes): PeopleInConversation +0x08, Conversation +0x14 (PDB 16-byte Dinkumware
//   puts it at +0x18). CConversationLine (Ego_r.pdb, 16 bytes): PersonSpeaking, PersonSpokenTo, LineToSay, ShowSubtitle.
//   CThing Dead = +0x91 bit 0 (as the landed CThingDeadCreature::WorldUpdate).
// Shape: `find(begin, end, &speaker)` makes a `const CThing*` temporary that VC7.1 homes in the speaker argument's own
//   slot; `find(..., listener)` passes the listener argument's address; the empty tag shares the bool's slot. The line
//   is an inline ctor temporary built in the frame (member init order), pushed, then destroyed.
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead; }
};

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

class CBaseIntelligentPointer {
public:
    void* PItem;
    void* PNext;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

namespace std {
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};
}

CIntelligentPointer<CThing>* Find_006e67a0(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last,
                                           const CThing* const* value);
namespace std {
#pragma auto_inline(off)
__declspec(noinline) CIntelligentPointer<CThing>* __fastcall __find(CIntelligentPointer<CThing>* first,
                                                                    CIntelligentPointer<CThing>* last,
                                                                    const CThing* const& value,
                                                                    const random_access_iterator_tag&)   // 0x006E67A0 stand-in
{
    return Find_006e67a0(first, last, &value);
}
#pragma auto_inline(on)

inline CIntelligentPointer<CThing>* find(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last,
                                         const CThing* const& value)
{
    return __find(first, last, value, random_access_iterator_tag());
}
}

class CScriptConversationManager;

class CScriptConversation {
public:
    class CConversationLine {
    public:
        long        PersonSpeaking;                         // +0x00
        long        PersonSpokenTo;                         // +0x04
        CCharString LineToSay;                              // +0x08
        bool        ShowSubtitle;                           // +0x0C
        CConversationLine(long speaking, long spokenTo, const CCharString& line, bool showSubtitle)
            : PersonSpeaking(speaking), PersonSpokenTo(spokenTo), LineToSay(line), ShowSubtitle(showSubtitle) { }
    };
    struct CPeopleVector {                                  // STLport vector<CIntelligentPointer<CThing> >
        CIntelligentPointer<CThing>* _M_start;
        CIntelligentPointer<CThing>* _M_finish;
        CIntelligentPointer<CThing>* _M_end_of_storage;
        CIntelligentPointer<CThing>* begin() { return _M_start; }
        CIntelligentPointer<CThing>* end() { return _M_finish; }
    };
    struct CLineVector {                                    // STLport vector<CConversationLine>
        CConversationLine* _M_start;
        CConversationLine* _M_finish;
        CConversationLine* _M_end_of_storage;
        void push_back(const CConversationLine& line);      // 0x006E6D30
    };

    CScriptConversationManager* Manager;                    // +0x00
    long                        ConversationIndex;          // +0x04
    CPeopleVector               PeopleInConversation;       // +0x08
    CLineVector                 Conversation;               // +0x14

    void AddLineToConversation(const CThing& speaker, const CThing* listener, const CCharString& line, bool showSubtitle);
};

void CScriptConversation::AddLineToConversation(const CThing& speaker, const CThing* listener, const CCharString& line,
                                                bool showSubtitle)
{
    CIntelligentPointer<CThing>* it = std::find(PeopleInConversation.begin(), PeopleInConversation.end(), &speaker);
    if (it != PeopleInConversation.end()) {
        long speakerIndex = it - PeopleInConversation.begin();
        long listenerIndex = -1;
        if (listener && !listener->IsDead()) {
            CIntelligentPointer<CThing>* found = std::find(PeopleInConversation.begin(), PeopleInConversation.end(), listener);
            if (found != PeopleInConversation.end())
                listenerIndex = found - PeopleInConversation.begin();
        }
        Conversation.push_back(CConversationLine(speakerIndex, listenerIndex, line, showSubtitle));
    }
}