// CSubtitledMessage::CSubtitledMessage(const CTextEntry&) -- build a subtitled message from a text-bank entry: copy the text and narrator, resolve the speech bank by name (falling back to bank 1), then turn each text tag into an attitude / animation / camera tag keyed by its relative position in the text, and sort the three tag maps
// Grade: DIFFER(1128v1281) -- frame 0x64 vs 0x74 (retail gives every prefix/substring temp its own slot), the inline c_str()/Length() pair of each StartsWith is two tests where retail merges them into one, and retail spends ebx on the strncmp bool (reloading the entry from the frame) while ours keeps the entry in ebx
// Leaf: ??0CSubtitledMessage
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CE550, 1281 bytes, `ret 4`.
// Literals "DEFAULT_GLOBAL_DEFS" 0x01239EC0, "ANIM:" 0x01239EB8, "CAM_SHOT:" 0x01239EAC, "CAM:" 0x01239EA4, "CAM" 0x01239EA0, "" 0x0122D70E / 0x0129AAF4.
// Callees: CTextEntry::GetText 0x009C7A40 (CWideString by value into Text), CCharString copy 0x0099EC30 / (const char*) 0x0099EBF0 / dtor 0x0099EAE0 /
//   GetSubString 0x0099EC70, CWideString::GetLength 0x0099B220, sound-bank registry (0x013B8394) by name 0x009917E0 / by id 0x00991840,
//   CGameDefinitionManager::Get 0x0044C6B0, GetDef<CGlobalDef> 0x004D0910, strncmp 0x00BFEAF8, AddMasterShotCameraTagFromText 0x004CE1B0,
//   AddCameraTagFromText 0x004CDC40, AddCameraTagFromIndex 0x004CE4E0, CStringParser ctor 0x00404720 / NextItemExists 0x009BA4F0 /
//   PeekNextItemType 0x009BA900 / ReadAsInteger 0x009BA540 (inline dtor: comment list 0x004046B0, ~CBase 0x0099A2E0),
//   tag symbol table Lookup 0x00A38420 (object 0x013B8A54), vector insert_overflow 0x004D1940 (CCharString pairs) / 0x004D1250 (attitude pairs),
//   sort 0x004D2470 / 0x004D24F0 / 0x004D2570 (__fastcall first/last + comparator by value).
#include <string.h>
inline void* operator new(unsigned int, void* p) { return p; }

struct CStringData { char* Data; long Length; };
struct CCharString {
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString GetSubString(long start, long count) const;   // 0x0099EC70
    const char* c_str() const { return PStringData ? PStringData->Data : ""; }
    long Length() const { return PStringData ? PStringData->Length : 0; }
    bool StartsWith(const CCharString& prefix) const { return strncmp(c_str(), prefix.c_str(), prefix.Length()) == 0; }
};
struct CWideString {
    unsigned short* PData;
    long GetLength() const;                            // 0x0099B220
};
struct CRefCounted { virtual void v00(); virtual void Release(); long RefCount; };
template <class T> struct CDefPointer {
    CRefCounted* P;
    CDefPointer() : P(0) { }
    CDefPointer(const CDefPointer& o) : P(o.P) { if (P) ++P->RefCount; }
    ~CDefPointer() { if (P && --P->RefCount == 0) P->Release(); }
};
struct CGlobalDef;
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
    bool GetDef(const CCharString& name, CDefPointer<const CGlobalDef>* def);   // 0x004D0910
};
struct CBase { virtual void v00(); ~CBase(); };         // 0x0099A2E0
struct CStringParserCommentList { void* m_pHead; ~CStringParserCommentList(); };   // 0x004046B0
struct CStringParser : CBase {
    char* String;
    unsigned long BaseLineOffset;
    CCharString Filename;
    CCharString TempCharString;
    CStringParserCommentList Comments;
    void (*ErrorCallback)(CCharString*);
    long CursorPos[3];
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename, long baseLineOffset);   // 0x00404720
    bool NextItemExists();                             // 0x009BA4F0
    long PeekNextItemType();                           // 0x009BA900
    long ReadAsInteger();                              // 0x009BA540
};
struct CTagSymbolTable { bool Lookup(const CCharString& name, long* value) const; };   // 0x00A38420
extern CTagSymbolTable g_TextTagSymbols;               // 0x013B8A54
struct CASoundBank;
struct CSoundBankRegistry {
    CASoundBank* GetBank(const CCharString& name);     // 0x009917E0
    CASoundBank* GetBank(long id);                     // 0x00991840
};
extern CSoundBankRegistry* g_pSoundBankRegistry;       // 0x013B8394

struct CTextTag { long Position; CCharString Tag; };
struct CTextTagArray {
    CTextTag* First; CTextTag* Last; CTextTag* End;
    unsigned long size() const { return Last - First; }
    const CTextTag& operator[](unsigned long i) const { return First[i]; }
};
struct CTextEntry {
    unsigned char _pad00[0x1c];
    CCharString SpeechBank;                            // +0x1c
    CCharString Narrator;                              // +0x20
    CCharString SoundName;                             // +0x24
    CWideString Text;                                  // +0x28
    CTextTagArray Tags;                                // +0x2c
    CWideString GetText() const;                       // 0x009C7A40
};

enum EConversationAttitude { ATTITUDE_NONE };
struct CCameraTag;
template <class T> struct CKeyPair {
    float first; T second;
    CKeyPair(const float& k, const T& v) : first(k), second(v) { }
};
template <class T> struct CKeyPairCompareLess { };
struct __false_type_ { };
typedef CKeyPair<CCharString> CAnimPair;
typedef CKeyPair<EConversationAttitude> CAttitudePair;
struct CAnimVector {
    CAnimPair* First; CAnimPair* Last; CAnimPair* End;
    CAnimVector() : First(0), Last(0), End(0) { }
    void insert_overflow(CAnimPair* pos, const CAnimPair& x, const __false_type_&, unsigned long n, bool atEnd);   // 0x004D1940
    void push_back(const CAnimPair& x) {
        if (Last != End) { new (Last) CAnimPair(x); ++Last; }
        else insert_overflow(Last, x, __false_type_(), 1, true);
    }
};
struct CAttitudeVector {
    CAttitudePair* First; CAttitudePair* Last; CAttitudePair* End;
    CAttitudeVector() : First(0), Last(0), End(0) { }
    void insert_overflow(CAttitudePair* pos, const CAttitudePair& x, const __false_type_&, unsigned long n, bool atEnd);   // 0x004D1250
    void push_back(const CAttitudePair& x) {
        if (Last != End) { new (Last) CAttitudePair(x); ++Last; }
        else insert_overflow(Last, x, __false_type_(), 1, true);
    }
};
void __fastcall SortAttitudes(CAttitudePair* first, CAttitudePair* last, CKeyPairCompareLess<EConversationAttitude> comp);   // 0x004D2470
void __fastcall SortAnims(CAnimPair* first, CAnimPair* last, CKeyPairCompareLess<CCharString> comp);   // 0x004D24F0
struct CCameraPair { float first; long v[6]; };
void __fastcall SortCameras(CCameraPair* first, CCameraPair* last, CKeyPairCompareLess<CCameraTag> comp);   // 0x004D2570

struct CAttitudeMap {
    CAttitudeVector Vector; CKeyPairCompareLess<EConversationAttitude> Compare; bool Dirty;
    CAttitudeMap() : Dirty(false) { }
    void Add(const float& k, const EConversationAttitude& v) { Vector.push_back(CAttitudePair(k, v)); Dirty = true; }
    void Sort() { if (Dirty) { SortAttitudes(Vector.First, Vector.Last, Compare); Dirty = false; } }
};
struct CAnimMap {
    CAnimVector Vector; CKeyPairCompareLess<CCharString> Compare; bool Dirty;
    CAnimMap() : Dirty(false) { }
    void Add(const float& k, const CCharString& v) { Vector.push_back(CAnimPair(k, v)); Dirty = true; }
    void Sort() { if (Dirty) { SortAnims(Vector.First, Vector.Last, Compare); Dirty = false; } }
};
struct CCameraMap {
    CCameraPair* First; CCameraPair* Last; CCameraPair* End; CKeyPairCompareLess<CCameraTag> Compare; bool Dirty;
    CCameraMap() : First(0), Last(0), End(0), Dirty(false) { }
    void Sort() { if (Dirty) { SortCameras(First, Last, Compare); Dirty = false; } }
};

class CSubtitledMessage {
public:
    CASoundBank* PSpeechBank;                          // +0x00
    long SpeechIndex;                                  // +0x04
    bool Speech3D;                                     // +0x08
    CWideString Text;                                  // +0x0c
    CCharString Narrator;                              // +0x10
    CAttitudeMap AttitudeTags;                         // +0x14
    CAnimMap AnimationTags;                            // +0x24
    CCameraMap CameraTags;                             // +0x34
    CSubtitledMessage(const CTextEntry& entry);
    bool AddMasterShotCameraTagFromText(float time, const CCharString& text);   // 0x004CE1B0
    bool AddCameraTagFromText(float time, const CCharString& text);             // 0x004CDC40
    bool AddCameraTagFromIndex(long index, float time, CDefPointer<const CGlobalDef> defs);   // 0x004CE4E0
};

CSubtitledMessage::CSubtitledMessage(const CTextEntry& entry)
    : PSpeechBank(0), SpeechIndex(0), Speech3D(true), Text(entry.GetText()), Narrator(entry.Narrator)
{
    PSpeechBank = g_pSoundBankRegistry->GetBank(entry.SpeechBank);
    if (PSpeechBank == 0)
        PSpeechBank = g_pSoundBankRegistry->GetBank(1);
    long textLength = Text.GetLength();
    CDefPointer<const CGlobalDef> globalDefs;
    {
        CCharString defsName("DEFAULT_GLOBAL_DEFS");
        CGameDefinitionManager::Get().GetDef(defsName, &globalDefs);
    }
    for (unsigned long i = 0; i < entry.Tags.size(); ++i) {
        float time = (float)entry.Tags[i].Position / (float)textLength;
        const CCharString& tag = entry.Tags[i].Tag;
        if (tag.StartsWith(CCharString("ANIM:"))) {
            AnimationTags.Add(time, tag.GetSubString(5, tag.Length()));
        } else if (tag.StartsWith(CCharString("CAM_SHOT:"))) {
            AddMasterShotCameraTagFromText(time, tag.GetSubString(9, tag.Length()));
        } else if (tag.StartsWith(CCharString("CAM:"))) {
            AddCameraTagFromText(time, tag.GetSubString(4, tag.Length()));
        } else if (tag.StartsWith(CCharString("CAM"))) {
            CCharString rest = tag.GetSubString(3, tag.Length());
            CStringParser parser(rest, 0, CCharString(""), 0);
            if (parser.NextItemExists() && parser.PeekNextItemType() == 3)
                AddCameraTagFromIndex(parser.ReadAsInteger(), time, globalDefs);
        } else {
            long attitude = 0;
            if (g_TextTagSymbols.Lookup(entry.Tags[i].Tag, &attitude))
                AttitudeTags.Add(time, (EConversationAttitude)attitude);
        }
    }
    AttitudeTags.Sort();
    AnimationTags.Sort();
    CameraTags.Sort();
}
