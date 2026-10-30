// CSubtitledMessage::GetMessagesFromTextName(CArray<CSubtitledMessage>& messages, const char* textName, const char* narrator, ETextGroupSelectionMethod method) -- look a text symbol up in the game text bank: a single text entry gives one message; a text group gives all of its messages, or the first one, or one picked at random / at random without repeating the last pick / in sequence (the last pick per group name is remembered); an unknown symbol gives one placeholder message "(HC)<name>"
// Grade: DIFFER(771v771) -- not-found path only: the placeholder text and the ToWideString temporary sit in swapped dead argument slots (retail text at +0x64 = narrator's slot, temporary at +0x68 = method's slot; ours the reverse); 5 instructions (4 lea offsets, 1 hidden-pointer lea), every other byte matches
// Leaf: ?GetMessagesFromTextName@CSubtitledMessage@@SIXAAV?$CArray@VCSubtitledMessage@@@@PBD1W4ETextGroupSelectionMethod@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CEBB0, 771 bytes, static `__fastcall` (messages in ecx, the name in edx), `ret 8`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?GetMessagesFromTextName@CSubtitledMessage@@SIXAAV?$CArray@VCSubtitledMessage@@@@PBD1W4ETextGroupSelectionMethod@@@Z;
//   it follows CSubtitledMessage::GetMessageFromTextIndex (0x004CEA60, PDB `SI?AV1@KPBD@Z`), which it calls for every message,
//   and its element is the 0x44-byte retail CSubtitledMessage (Ego_r 0x50 less 4 per CVectorMap).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / operator!= 0x0099E900 / operator= 0x0099EFB0 /
//   ToWideString 0x0099E480, CCRC::Calc 0x004014A0, CBankFile::FindIndexBySymbolCRC 0x009CC410 and GetEntryDataType
//   0x009CC210, NGameText::CDataBank::GetPGroupEntry 0x009C8890, CWorld::GetWorldSeed 0x0049D810, GFRandom 0x004CDAE0,
//   operator+(const unsigned short*, const CWideString&) 0x0099BFF0, ~CWideString 0x0099B510, CSubtitledMessage(const CWideString&)
//   0x004CDBD0 / (const CSubtitledMessage&) 0x004D1FE0 / ~ 0x004D1DB0, std::vector<CSubtitledMessage>::_M_insert_overflow
//   0x004D2760, CArray<CSubtitledMessage>::push_back 0x004D2880 (out of line).
// Globals: g_pMainGameComponent 0x013B86A0 (PTextBank +0x14, PWorld +0x24), CSubtitledMessage::MostRecentGroupQueried
//   0x013B8A50 (its dynamic initializer is in both PDB name sets), the last group pick 0x01378E24 (.data, initial -1;
//   descriptive name), the empty string 0x0129AAF4, L"(HC)" 0x01239EDC.
#include "engine/CCountedPointer.h"
#define CBankFile CBankFileLayout
#include "engine/CBankFile.h"
#undef CBankFile
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld

inline void* __cdecl operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    _Tp* begin() { return this->_M_start; }
    const _Tp* begin() const { return this->_M_start; }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend);
};
}   // namespace std

template <class T> class CArray : public std::vector<T> {
public:
    void push_back(const T& value) { std::vector<T>::push_back(value); }   // 0x004D2880 for CSubtitledMessage (out of line)
};

class CCharStringData {
public:
    const char* Data;                                   // +0
    long        Length;                                 // +4
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall operator+(const unsigned short* lhs, const CWideString& rhs);   // 0x0099BFF0
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator!=(const CCharString& other) const;    // 0x0099E900
    CWideString ToWideString() const;                   // 0x0099E480
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->Data; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
    static unsigned long __fastcall CalcFromString(const CCharString& s) { return Calc(0, s.GetAsCharArray(), s.GetLength()); }
};
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0

class CWorld : public CWorldLayout {
public:
    unsigned long* GetWorldSeed() const;                // 0x0049D810
};
template <class T> class scoped_ptr {
public:
    T* ptr;
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
};

class CIVRefCount {
public:
    virtual ~CIVRefCount();
    virtual void Delete() const;                        // slot 1
    long IVRefCount;                                    // +4
    long Release() const
    {
        long count = --const_cast<CIVRefCount*>(this)->IVRefCount;
        if (count == 0)
            Delete();
        return count;
    }
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};

class CBankFile : public CBankFileLayout {
public:
    unsigned long GetEntryDataType(unsigned long index) const;        // 0x009CC210
    unsigned long FindIndexBySymbolCRC(unsigned long crc) const;      // 0x009CC410
};
class CBankFileAsync : public CBankFile {
public:
};
namespace NGameText {
class CGroupEntry : public CIVRefCount {
public:
    unsigned char _pad08[0x1c - 8];
    CArray<long>  Entries;                              // +0x1C
};
class CDataBank : public CBankFileAsync {
public:
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C8890
};
}   // namespace NGameText

class CMainGameComponent {
public:
    unsigned char                         _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;    // +0x14
    unsigned char                         _pad1c[0x24 - 0x1c];
    scoped_ptr<CWorld>                    PWorld;       // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0

enum ETextGroupSelectionMethod {
    GROUP_SELECT_NONE,
    GROUP_SELECT_RANDOM,
    GROUP_SELECT_RANDOM_NO_REPEAT,
    GROUP_SELECT_SEQUENTIAL,
    GROUP_SELECT_FIRST_ONLY
};

class CASoundBank;
template <class K, class V> class CKeyPairCompareLess { };
class CSubtitledMessage {
public:
    class CCameraTag;
    CASoundBank*  PSpeechBank;                          // +0x00
    long          SpeechIndex;                          // +0x04
    bool          Speech3D;                             // +0x08
    CWideString   Text;                                 // +0x0C
    CCharString   Narrator;                             // +0x10
    unsigned long AttitudeTags[4];                      // +0x14 CVectorMap<float, EConversationAttitude, ..>
    unsigned long AnimationTags[4];                     // +0x24 CVectorMap<float, CCharString, ..>
    unsigned long CameraTags[4];                        // +0x34 CVectorMap<float, CSubtitledMessage::CCameraTag, ..>
    CSubtitledMessage(const CWideString& text);         // 0x004CDBD0
    CSubtitledMessage(const CSubtitledMessage& other);  // 0x004D1FE0
    ~CSubtitledMessage();                               // 0x004D1DB0
    static CSubtitledMessage __fastcall GetMessageFromTextIndex(unsigned long index, const char* narrator);   // 0x004CEA60
    static void __fastcall GetMessagesFromTextName(CArray<CSubtitledMessage>& messages, const char* textName,
                                                   const char* narrator, ETextGroupSelectionMethod method);
private:
    static CCharString MostRecentGroupQueried;          // 0x013B8A50
};
extern long g_MostRecentGroupIndex_01378e24;           // 0x01378E24 (.data, -1)

void __fastcall CSubtitledMessage::GetMessagesFromTextName(CArray<CSubtitledMessage>& messages, const char* textName,
                                                           const char* narrator, ETextGroupSelectionMethod method)
{
    const NGameText::CDataBank* bank = g_pMainGameComponent->PTextBank.Data;
    unsigned long index = bank->FindIndexBySymbolCRC(CCRC::CalcFromString(CCharString(textName)));
    if (index > 0) {
        if (bank->GetEntryDataType(index) == 0) {
            messages.push_back(GetMessageFromTextIndex(index, narrator));
            return;
        }
        const CArray<long>& entries = bank->GetPGroupEntry(index, true)->Entries;
        if (method == GROUP_SELECT_NONE) {
            for (unsigned long i = 0; i < entries.size(); ++i)
                messages.push_back(GetMessageFromTextIndex(entries[i], narrator));
            return;
        }
        if (method == GROUP_SELECT_FIRST_ONLY) {
            if (!entries.empty())
                messages.push_back(GetMessageFromTextIndex(entries[0], narrator));
            return;
        }
        CWorld& world = *g_pMainGameComponent->PWorld;
        if (CCharString(textName) != MostRecentGroupQueried) {
            MostRecentGroupQueried = CCharString(textName);
            g_MostRecentGroupIndex_01378e24 = -1;
        }
        long choice = 0;
        switch (method) {
        case GROUP_SELECT_RANDOM:
            choice = GFRandom(entries.size(), world.GetWorldSeed());
            break;
        case GROUP_SELECT_RANDOM_NO_REPEAT:
            if (entries.size() > 1 && g_MostRecentGroupIndex_01378e24 != -1) {
                choice = GFRandom(entries.size() - 1, world.GetWorldSeed());
                if (choice >= g_MostRecentGroupIndex_01378e24)
                    ++choice;
            } else
                choice = GFRandom(entries.size(), world.GetWorldSeed());
            break;
        case GROUP_SELECT_SEQUENTIAL:
            choice = g_MostRecentGroupIndex_01378e24 + 1;
            if (choice >= entries.size())
                choice = 0;
            break;
        }
        g_MostRecentGroupIndex_01378e24 = choice;
        messages.push_back(GetMessageFromTextIndex(entries[choice], narrator));
    } else {
        CWideString text = L"(HC)" + CCharString(textName).ToWideString();
        messages.push_back(CSubtitledMessage(text));
    }
}
