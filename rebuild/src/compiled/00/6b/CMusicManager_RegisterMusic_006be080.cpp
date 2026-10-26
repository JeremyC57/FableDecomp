// CMusicManager::RegisterMusic() -- for every CMusicEntry of the sound def build "Data\\Sound\\" + the string-table file name as
//   both the source and destination wide names, swap ".wma" for ".ogg" in each, add (ID, entry) to the registered-music vector map
//   (push_back + Dirty), then sort the map once if it is dirty
// Leaf: RegisterMusic
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BE080, 628 bytes (plain `ret`). Caller: CMusicManager::CMusicManager 0x006BE300 (landed).
// Callees: CDefStringTable::GetString 0x009D49B0 on the global table 0x013CA828, CCharString::ToWideString 0x0099E480,
//   ~CCharString 0x0099EAE0, the sound-directory helpers 0x0041A3F0 / 0x0041A3D0 (byte-identical `__fastcall` hidden-result
//   bodies building L"Data\\Sound\\" 0x0122F4EC; manifest "CUserProfileManager::GetAutoSaveFileName" is propagated),
//   operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall), CWideString(const wchar_t*) 0x0099B6B0
//   (L".ogg" 0x01230C48, L".wma" 0x01230C3C), CWideString copy 0x0099B720, ~CWideString 0x0099B510,
//   CWideString::ReplaceSubstring 0x0099C1E0 (manifest module says CCharString), registered-map push_back 0x006BF410
//   (manifest "Vector_PushBackMusicEntry"), the 0x18-element sort 0x006BF6A0 (__fastcall first/last + comparator by value).
// Layout (retail): CSoundDef::MusicEntries 12-byte STLport vector at +0x54 (PDB +0x60, three 16-byte vectors earlier);
//   CMusicManager::PSoundDef +0x18, RegisteredMusic +0x1c (vector +0x1c, Compare +0x28, Dirty +0x29) as the landed ctor.
// Shape: the text is fetched by an inline helper with a named CCharString (it dies before the directory call, and the
//   ToWideString result pointer is pushed after GetString returns); a user copy ctor on CRegisteredMusicEntry gives the
//   in-order Gain/Offset/CacheToHD copy into the pair; the inline Add sets Dirty between the pair and entry destructors.
class CWideString { public:
    void* Data;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    void ReplaceSubstring(const CWideString& from, const CWideString& to);   // 0x0099C1E0
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70

class CCharString { public:
    void* Data;
    ~CCharString();                                     // 0x0099EAE0
    CWideString ToWideString() const;                   // 0x0099E480
};

struct CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable GDefStringTable;                 // 0x013CA828

CWideString __fastcall GetSoundDataDirectory();         // 0x0041A3F0 (L"Data\\Sound\\")
namespace NResourceDirectoryNames { CWideString __fastcall GetHDMusicDir(); }   // 0x0041A3D0        // 0x0041A3D0 (byte-identical twin)

class CMusicEntry { public:
    unsigned long ID;
    unsigned long Filename;
    float Gain;
    bool CacheToHD;
};

struct CMusicEntryVector {
    CMusicEntry* Start;
    CMusicEntry* Finish;
    CMusicEntry* EndOfStorage;
    unsigned long size() const { return Finish - Start; }
    const CMusicEntry& operator[](unsigned long i) const { return *(Start + i); }
};

class CSoundDef { public:
    unsigned char _pad_0x00[0x54];
    CMusicEntryVector MusicEntries;     // +0x54
};

template <class T>
class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

struct CRegisteredMusicEntry {
    CWideString SrcFileName;
    CWideString DstFileName;
    float Gain;
    float Offset;
    bool CacheToHD;
    CRegisteredMusicEntry(const CWideString& src, const CWideString& dst, float gain, bool cache)
        : SrcFileName(src), DstFileName(dst), Gain(gain), Offset(0.0f), CacheToHD(cache) {}
    CRegisteredMusicEntry(const CRegisteredMusicEntry& o)
        : SrcFileName(o.SrcFileName), DstFileName(o.DstFileName), Gain(o.Gain), Offset(o.Offset), CacheToHD(o.CacheToHD) {}
};

struct CRegisteredMusicPair {
    long first;
    CRegisteredMusicEntry second;
    CRegisteredMusicPair(const long& a, const CRegisteredMusicEntry& b) : first(a), second(b) {}
};

struct CKeyPairCompareLess { };

void __fastcall SortRegisteredMusic(CRegisteredMusicPair* first, CRegisteredMusicPair* last, CKeyPairCompareLess comp);   // 0x006BF6A0

struct CRegisteredMusicVector {
    CRegisteredMusicPair* Start;
    CRegisteredMusicPair* Finish;
    CRegisteredMusicPair* EndOfStorage;
    void push_back(const CRegisteredMusicPair& value);  // 0x006BF410
};

struct CRegisteredMusicMap {
    CRegisteredMusicVector Entries;     // +0x00
    CKeyPairCompareLess Compare;        // +0x0c
    bool Dirty;                         // +0x0d
    void Add(const long& key, const CRegisteredMusicEntry& value)
    {
        Entries.push_back(CRegisteredMusicPair(key, value));
        Dirty = true;
    }
    void Sort()
    {
        if (Dirty) {
            SortRegisteredMusic(Entries.Start, Entries.Finish, Compare);
            Dirty = false;
        }
    }
};

class CMusicManager { public:
    long CurrentState[6];                   // +0x00
    CDefPointer<const CSoundDef> PSoundDef; // +0x18
    CRegisteredMusicMap RegisteredMusic;    // +0x1c
    void RegisterMusic();
};

static inline CWideString GetWideText(unsigned long id)
{
    CCharString text = GDefStringTable.GetString(id);
    return text.ToWideString();
}

void CMusicManager::RegisterMusic()
{
    for (unsigned long i = 0; i < PSoundDef->MusicEntries.size(); ++i) {
        const CMusicEntry& entry = PSoundDef->MusicEntries[i];
        CWideString src = GetSoundDataDirectory() + GetWideText(entry.Filename);
        CWideString dst = NResourceDirectoryNames::GetHDMusicDir() + GetWideText(entry.Filename);
        src.ReplaceSubstring(L".wma", L".ogg");
        dst.ReplaceSubstring(L".wma", L".ogg");
        RegisteredMusic.Add(entry.ID, CRegisteredMusicEntry(src, dst, entry.Gain, entry.CacheToHD));
    }
    RegisteredMusic.Sort();
}