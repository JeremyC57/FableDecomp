// COpinionReactionManager::AttachOpinionSource(CThing* thing, long sourceDefIndex, CTCOpinionSource** ppSource) -- find the thing's opinion-source component (adding a "CTCOpinionSource" TC when it has none), hand it back through ppSource, and push the source def with that index onto it
// Leaf: AttachOpinionSource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00700820, length 146 bytes (padded to 160). The (CThing*, const CCharString&)
// overload at 0x00700780 is the same body with PushNewSourceDef(const CCharString&).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString(const char*, long)
//          @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0,
//          CTCOpinionSource::PushNewSourceDef(long) @ 0x007714B0 (manifest "CTCCreatureGroupTag::DeactivateMember" is wrong:
//          0x00771620 PushNewSourceDef(const CCharString&) resolves the name and calls it). String "CTCOpinionSource" @ 0x0123B61C.
// Shape: with no out pointer the result goes to a local VC homes in the (already loaded) `thing` slot; the
// component lookup is the landed out-parameter GetInterface template (TC type 0x50 = bit 16 of presence word 2);
// the AddTC name is a named CCharString local in its own block (built in the dead ppSource slot before the
// trailing pushes); the result is re-read through ppSource for the null test; `x && f()` with the shared false tail.
struct CTCBase;
struct CTCOpinionSource {
    bool PushNewSourceDef(const struct CCharString& defName);   // 0x00771620
    bool PushNewSourceDef(long defIndex);                      // 0x007714B0
};
struct CTCParamsBase;
struct CCharString {
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
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];         // +0x20 bitset of the interface types present
    CInterfaceMap m_interfaces;             // +0x44
    CTCBase* AddTC(const CCharString& name, bool flag, CTCParamsBase* params);   // 0x004C9D60
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
};

enum { TC_OPINION_SOURCE = 0x50 };

class COpinionReactionManager {
public:
    bool AttachOpinionSource(CThing* thing, long sourceDefIndex, CTCOpinionSource** ppSource);
};

bool COpinionReactionManager::AttachOpinionSource(CThing* thing, long sourceDefIndex, CTCOpinionSource** ppSource)
{
    CTCOpinionSource* source;
    if (!ppSource)
        ppSource = &source;
    if (!thing->GetInterface(ppSource, TC_OPINION_SOURCE))
    {
        CCharString name("CTCOpinionSource", -1);
        *ppSource = (CTCOpinionSource*)thing->AddTC(name, false, 0);
    }
    if (*ppSource && (*ppSource)->PushNewSourceDef(sourceDefIndex))
        return true;
    return false;
}
