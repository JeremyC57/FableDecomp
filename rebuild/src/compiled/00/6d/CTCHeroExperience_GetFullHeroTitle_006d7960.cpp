// SW2 sweep (advisor/006d7960/probe/t1.cpp), on the chunk-301 lane source.
// CTCHeroExperience::GetFullHeroTitle() const -- hero title text, plus " - " and the best earned title from the experience def's list
class CWideString {
public:
    void* Data;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    bool operator!=(const unsigned short* text) const;         // 0x0099B150
    CWideString& operator+=(const unsigned short* text);       // 0x0099B940
    CWideString& operator+=(const CWideString& other);  // 0x0099B8D0
};
template<class T> class CDefPointer;
class CTitleDef;
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
    bool GetSubDef(CDefPointer<const CTitleDef>* out) const;   // 0x005B1E5A
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
template<class T> class CCountedPointer {
public:
    T* Object;
    __forceinline ~CCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
template<class T> class CIVCountedPointer {
public:
    T* Object;
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
class CTitleDef : public CDefClassBase {
public:
    unsigned char _pad08[0x2c - 0x08];
    long TextID;                                        // +0x2C
    unsigned char _pad30[0x3c - 0x30];
    long MinAge;                                        // +0x3C
    long MinStat[3];                                    // +0x40
    unsigned char _pad4c;
    bool Enabled;                                       // +0x4D
};
class CDefinitionManager {
public:
    CCountedPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
};
namespace NGameText {
class CTextEntry {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
    CWideString GetText() const;                        // 0x009C7A40
};
}
using NGameText::CTextEntry;
namespace NGameText {
class CDataBank {
public:
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long id, bool flag) const;   // 0x009C9070
};
}
using NGameText::CDataBank;
struct CGameTextManager {
    unsigned char _pad00[0x14];
    CDataBank* PDataBank;                               // +0x14
};
extern CGameTextManager* g_GameText;                    // 0x013B86A0
inline CDataBank* GetGameTextBank() { return g_GameText->PDataBank; }
template<class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; const T* begin() const { return _M_start; } const T* end() const { return _M_finish; } long size() const { return end() - begin(); } const T& operator[](long n) const { return *(begin() + n); } };
class CHeroExperienceDef {
public:
    unsigned char _pad00[0x34];
    vector<long> Titles;                                // +0x34
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;         // 0x004365B0
};
// same-TU stand-in for the interface-map LowerBound (VC7.1 then knows the entry is not derived from the key)
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* lo = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
    CDefinitionManager* GetDefinitions() const;         // 0x006869D0
};
class CTCHero : public CTCBase {
public:
    CWideString GetTitleGuiText() const;                // 0x006CA0A0
};
class CTCAge : public CTCBase {
public:
    unsigned char _pad0c[0x70 - 0x0c];
    long Age;                                           // +0x70
};
class CTCHeroExperience : public CTCBase {
public:
    unsigned char _pad0c[0x54 - 0x0c];
    const CHeroExperienceDef* PDef;                     // +0x54
    long GetTotalStatLevel(long category) const;        // 0x006D6660
    CWideString GetFullHeroTitle() const;
};

// Same-TU stand-in for the out-of-line GetSubDef (retail's link-time codegen saw that the out pointer does not escape).
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<const CTitleDef>* out) const
{
    out->Object = (const CTitleDef*)this;
    return true;
}
#pragma auto_inline(on)

CWideString CTCHeroExperience::GetFullHeroTitle() const
{
    CTCHero* hero;
    if (Thing->HasTC(0x29))
        hero = (CTCHero*)Thing->GetTC(0x29);
    CWideString title = hero->GetTitleGuiText();
    CTCAge* age;
    if (Thing->HasTC(4))
        age = (CTCAge*)Thing->GetTC(4);
    for (long i = PDef->Titles.size() - 1; i >= 0; --i) {
        CCountedPointer<CDefClassBase> def = GetDefinitions()->GetPDefFromGlobalIndex(PDef->Titles[i]);
        CDefPointer<const CTitleDef> titleDef;
        def->GetSubDef(&titleDef);
        if (titleDef.Object && titleDef->Enabled && age->Age >= titleDef->MinAge && GetTotalStatLevel(0) >= titleDef->MinStat[0]
            && GetTotalStatLevel(1) >= titleDef->MinStat[1] && GetTotalStatLevel(2) >= titleDef->MinStat[2]) {
            CIVCountedPointer<const CTextEntry> text = GetGameTextBank()->GetPTextEntry(titleDef->TextID, true);
            if (text.Object) {
                if (title != (const unsigned short*)L"")
                    title += (const unsigned short*)L" - ";
                title += text->GetText();
                break;
            }
        }
    }
    return title;
}