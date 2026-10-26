// COpinionReactionManager::AttachOpinionSource(CThing* thing, const CCharString& sourceDefName, CTCOpinionSource** ppSource) -- find the thing's opinion-source component (adding a "CTCOpinionSource" TC when it has none), hand it back through ppSource, and push the named source def onto it
// Leaf: ?AttachOpinionSource@COpinionReactionManager@@QAE_NPAVCThing@@ABVCCharString@@PAPAVCTCOpinionSource@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00700780, length 146 bytes (padded to 160). The (CThing*, long) overload
// at 0x00700820 is the same body with PushNewSourceDef(long).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString(const char*, long)
//          @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0,
//          CTCOpinionSource::PushNewSourceDef(const CCharString&) @ 0x00771620. String "CTCOpinionSource" @ 0x0123B61C.
// Shape: with no out pointer the result goes to a local VC homes in the (already loaded) `thing` slot; the
// component lookup is the landed out-parameter GetInterface template (TC type 0x50 = bit 16 of presence word 2);
// the AddTC name is a named CCharString local in its own block (built in the dead ppSource slot before the
// trailing pushes); the result is re-read through ppSource for the null test; `x && f()` with the shared false tail.
class CTCBase;
class CTCOpinionSource { public:
    bool PushNewSourceDef(const class CCharString& defName);   // 0x00771620
    bool PushNewSourceDef(long defIndex);                      // 0x007714B0
};
class CTCParamsBase;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];         // +0x20 bitset of the interface types present
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};

enum { TC_OPINION_SOURCE = 0x50 };

class COpinionReactionManager {
public:
    bool AttachOpinionSource(CThing* thing, const CCharString& sourceDefName, CTCOpinionSource** ppSource);
};

bool COpinionReactionManager::AttachOpinionSource(CThing* thing, const CCharString& sourceDefName, CTCOpinionSource** ppSource)
{
    CTCOpinionSource* source;
    if (!ppSource)
        ppSource = &source;
    if (!thing->GetInterface(ppSource, TC_OPINION_SOURCE))
    {
        CCharString name("CTCOpinionSource", -1);
        *ppSource = (CTCOpinionSource*)thing->AddTC(name, false, 0);
    }
    if (*ppSource && (*ppSource)->PushNewSourceDef(sourceDefName))
        return true;
    return false;
}