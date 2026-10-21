// Grade: DIFFER(386v392) -- register allocation: retail keeps the crime iterator in edi (frame-homed at [ebp-0xc]) and caches the deed effects def in ebx with end() only in the frame; ours enregisters it in ebx and end in edi, so the effects def is reloaded into eax and the loop entry jumps to the bottom test
// CTCVillage::GetWarningStringForCurrentCrimes(long index, bool* severe, long* deedHandle) -- walk the hero's crimes against this village and return the index-th warning line of each crime's deed effects def (the deed handle and the def's severe-warning flag through the out pointers); an empty line resets the outputs and ends the walk
// Leaf: GetWarningStringForCurrentCrimes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A075, length 392 bytes (`leave`, `ret 0x10`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed sibling GetRegionLeavingDurationForCurrentCrimes
//   0x0065A5BB (`push 51h ; pop edi`).
// Callees: CCharString() @ 0x0099E4B0, CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0,
//   CCharString::operator=(const CCharString&) @ 0x0099EFB0 / operator=(const char*) @ 0x0099EFE0, CBasicString<char>::
//   operator==(const char*) @ 0x004115A0; CWorld::GetCurrentPlayer 0x0049D9D0; CPlayer::GetHeroThing 0x00487DC0;
//   std::bitset<>::test 0x00410DE0; CVectorMap LowerBound 0x0040F020; CTCHeroOpinionDeedLog::GetDeedFromHandle
//   0x006C58F0 / GetDeedEffectsDef 0x006C5630; STLport tree Successor 0x004292C0. Literal "" @ 0x0122D70E.
// Layout: retail CTCVillage PlayersCrimes +0x64 (as the sibling); retail COpinionDeedEffectsDef warning lines
//   (vector<CCharString>, STLport {start, finish, end}) +0x64 and a flags word +0x7c whose bit 4 marks a severe
//   warning (descriptive; the PDB def is 0x3c-based like the sibling's KnowledgeLifeInSeconds +0x3c).
// Shape: one CCharString local returned by copy; the out values are reset up front and again on an empty line; the
//   deed effects def is a named local (null-tested release, cleared pointer); the "" literal is shared by the empty
//   assignment and the inline ==("") test (strcmp intrinsic on a null string, as landed 0x00E6A000).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <string.h>
struct CTCBase;

#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                       // 0x0099E4B0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
    CCharString& operator=(const char* text);            // 0x0099EFE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    __forceinline SEntry* Find(long interfaceType)
    {
        long type = interfaceType;
        SEntry* it = LowerBound(&type);
        SEntry* found = last;
        if (it != found && !(interfaceType < it->type))
            found = it;
        return found;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        return m_interfaces.Find(interfaceType)->component;
    }
};
struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CWorld {
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
struct CMainGameComponent {
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CCharStringVector {                      // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    CCharString& operator[](unsigned int i) const { return _M_start[i]; }
};
struct COpinionDeedEffectsDef : CDefVirtual, CRefCounted {
    unsigned char     _pad_0x08[0x5c];
    CCharStringVector WarningLines;             // +0x64
    unsigned char     _pad_0x70[0x0c];
    unsigned long     Flags;                    // +0x7c (bit 4: severe warning)
    __forceinline void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
struct COpinionDeed {
    long DeedType;
};
struct CTCHeroOpinionDeedLog {
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};

struct SNode {
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   _M_value_field;                      // +0x10
};
SNode* __cdecl Successor(SNode* node);          // 0x004292C0

struct CTCVillage {
    unsigned char _pad_0x00[0x64];
    SNode*        PlayersCrimes;                // +0x64 set<long> header node
    CCharString GetWarningStringForCurrentCrimes(long index, bool* severe, long* deedHandle);
};

CCharString CTCVillage::GetWarningStringForCurrentCrimes(long index, bool* severe, long* deedHandle)
{
    CCharString warning;
    *severe = false;
    *deedHandle = -1;
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetHeroThing();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        if (hero->HasTC(0x51))
            log = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
        SNode* end = PlayersCrimes;
        for (SNode* it = end->_M_left; it != end; it = Successor(it)) {
            COpinionDeed* deed = log->GetDeedFromHandle(it->_M_value_field);
            if (deed) {
                CDefPointer<COpinionDeedEffectsDef> effects = log->GetDeedEffectsDef(deed->DeedType);
                if (index >= 0 && (unsigned int)index < effects->WarningLines.size()) {
                    *deedHandle = it->_M_value_field;
                    warning = effects->WarningLines[index];
                    if (effects->Flags & 0x10)
                        *severe = true;
                } else {
                    warning = "";
                }
                if (warning == "") {
                    *severe = false;
                    *deedHandle = -1;
                    break;
                }
            }
        }
    }
    return warning;
}
