// CTCHero::SetTitle(long titleObjectDef) -- take the hero title from a title object def's CHeroTitleDef (a hero thing whose type byte is not 2 and that lacks interface 0x50 also gets the RIVAL_HERO_TITLE opinion source); an unknown def clears the title
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA400, length 214 bytes (+ int3 padding). __fastcall, `ret 4`.
//         Ego_r.pdb: `?SetTitle@CTCHero@@QAEXJ@Z`.
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex(long) @ 0x009AD9E0
//          (CDefPointer by hidden pointer), CDefClassBase::GetSubDef<CHeroTitleDef>(CDefPointer<const T>*) const @ 0x005B1E5A
//          (straight into the member), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0
//          ("OPINION_SOURCE_RIVAL_HERO_TITLE" @ 0x0125EBF8), CTCBase::GetWorld @ 0x00686A70,
//          COpinionReactionManager::AttachOpinionSource(CThing*, const CCharString&, CTCOpinionSource**) @ 0x00700780,
//          the defs' virtual slot 1.
// Shape: the looked-up def is a by-value CDefPointer temporary released at the end through the ref-count base's DecRef /
//        GetRefCount pair (the long `add eax,4 ; dec ; cmp ; mov [eax]` form); the cleared sub-def pointer is the
//        release-plus-two-zero-stores `operator=(0)`; one ebp zero serves every compare and store; the opinion name is
//        homed in the dead argument slot; the parent (re-read after it) and the world are named locals, so
//        GetWorld runs before the pushes.
// Layout: Ego_r.pdb CTCHero (matches retail): Title +0x10, HeroTitleObjectDef +0x14, PHeroTitleSubDef +0x18; CHeroTitleDef
//         ::Title +0x28; retail CThing type byte +0x10 (anything but 2), interface 0x50 = word 2 bit 16; retail CWorld opinion
//         reaction manager +0x74.
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CRefCounted {                                // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                         // slot 1
};
template<class T> class CDefPointerT;
struct CDefBase : CDefVirtual, CRefCounted {        // CDefClassBase
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
    template<class T> bool GetSubDef(CDefPointerT<const T>* out) const;   // 0x005B1E5A for CHeroTitleDef
};
class CHeroTitleDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long Title;                                     // +0x28 EHeroTitle
};

template<class T> class CDefPointerT {              // CDefPointer<const T> member
public:
    T* Object;
    T* operator->() const { return Object; }
    CDefPointerT& operator=(int)
    {
        if (Object) {
            ((CDefBase*)Object)->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};

class CDefClassBase;
template <class T> class CDefPointer { public:                                // CDefPointer<CDefClassBase>: the manager's return type
    CDefBase* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};

class CDefinitionManager { public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const; // 0x009AD9E0 (hidden result pointer)
};
class CGameDefinitionManager : public CDefinitionManager { };

class CTCOpinionSource;
class COpinionReactionManager { public:
    bool AttachOpinionSource(CThing* thing, const CCharString& name, CTCOpinionSource** ppSource);   // 0x00700780
};
class CWorld { public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* OpinionReactionManager;   // +0x74
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                        // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};

class CTCBase { protected:                         // Ego_r.pdb CTCBase protected accessors
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    CWorld& GetWorld() const;                      // 0x00686A70
};

class CTCHero : public CTCBase {
public:
    void* __vfptr;                                  // +0x00
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    long Title;                                     // +0x10 EHeroTitle
    long HeroTitleObjectDef;                        // +0x14
    CDefPointerT<const CHeroTitleDef> PHeroTitleSubDef;   // +0x18
    void SetTitle(long titleObjectDef);
};

void CTCHero::SetTitle(long titleObjectDef)
{
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(titleObjectDef);
    if (def.m_p && def.m_p->GetSubDef(&PHeroTitleSubDef)) {
        Title = PHeroTitleSubDef->Title;
        HeroTitleObjectDef = titleObjectDef;
        if (Title != 0) {
            if (ParentThing->ThingType != 2 && !ParentThing->HasTC(0x50)) {
                CTCOpinionSource* source;
                CCharString name("OPINION_SOURCE_RIVAL_HERO_TITLE", -1);
                CThing* thing = ParentThing;
                CWorld* world = &GetWorld();
                world->OpinionReactionManager->AttachOpinionSource(thing, name, &source);
            }
        }
    } else {
        Title = 0;
        PHeroTitleSubDef = 0;
        HeroTitleObjectDef = 0;
    }
}