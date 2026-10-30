// CTCCreatureGeneratorCreator::FrameUpdate() -- once its regeneration frame has come (and generation is enabled), pick the creature family whose level is the highest at or below the hero's experience level + 1 (else the lowest), spawn a fresh generator thing from a random family of that level (an object def, or the invisible marker), copy the script name, clone the old generator's settings into it, retarget PGenerator, seed its family list; then schedule the next regeneration once the generator is dead
// Grade: DIFFER(1182v1123) -- 354 difflines (parked 1186/359): the marker def name as an explicit CCharString temporary. Residue: frame 0x38 v 0x30 (retail packs best/lowest/family/key/temps into +0x04..+0x14), this in edi (retail ebx) with the generator in ebx
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C99F0, 1136 bytes, no EH frame.
// Callees: GFGetCurrentFrame 0x0049D870, CTCBase::GetWorldConst 0x00686A80 / GetWorld 0x00686A70 / GetPlayerManager 0x00686D50 /
// GetDefinitionManager 0x006869D0, CWorld::IsCreatureGenerationEnabled 0x0049EA40 / GetPRandomSeed 0x0049D810,
// CPlayerManager::GetMainPlayer (const) 0x00449960, CPlayer::GetControlledCreature 0x00487DD0, LowerBound 0x0040F020 /
// const 0x004365B0, CTCHeroExperience::GetRoughExperienceLevel 0x006D64B0, GetDef<CCreatureGenerationFamilyDef> 0x0074D220,
// vector<CDefPointer>::push_back 0x007CA350, _Destroy_range 0x007CA090, GFRandom 0x004CDAE0, CThing::GetPosition 0x004C73D0,
// CThingObject::Create 0x00703210, CGameDefinitionManager::Get 0x0044C6B0, GetDefGlobalIndexFromName 0x009AD410,
// CThingMarker::Create 0x0083D460, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 / operator= 0x0099EFB0,
// CTCCreatureGenerator::CloneFromCreator 0x00749E40 / SetAsActive 0x00749DD0, IS_THING_ALIVE 0x004CC340, CThing::Kill 0x004C9B80,
// CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760 / ~ 0x00A01C10, CDefString -> CCharString 0x00415DD0,
// vector<long>::push_back 0x007CA160 / operator= 0x004E24BA, free 0x00BFEA14, IsGeneratorAlive 0x007C98A0, __ftol2.
// Layout: as the landed IsGeneratorAlive model (FrameToRegenerate +0x0c, Generated +0x10, PGenerator +0x11 unaligned, pack(1)).
#include <stddef.h>
extern "C" void __cdecl free(void* p);                       // 0x00BFEA14
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const CCharString& other);        // 0x0099EFB0
};
struct CDefString {
    long Index;
    CDefString() { }
    operator CCharString() const;                            // 0x00415DD0
};
extern long g_FramesPerSecond;                               // 0x01375550
long GFGetCurrentFrame();                                    // 0x0049D870
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0

struct CCreatureGenerationFamilyDef {
    virtual void v00();
    virtual void Destroy() const;                            // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetName() const;                      // slot 10 (+0x28)
    long RefCount;                                           // +0x04
    unsigned char _pad08[0x48 - 0x08];
    long Level;                                              // +0x48
    long MarkerDefIndex;                                     // +0x4c
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++const_cast<CCreatureGenerationFamilyDef*>(Object)->RefCount; }
    __forceinline ~CDefPointer() { if (Object) { if (--const_cast<CCreatureGenerationFamilyDef*>(Object)->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};
typedef CDefPointer<const CCreatureGenerationFamilyDef> CFamilyPointer;
struct alloc_tag { alloc_tag() { } };
void __fastcall DestroyFamilyRange(CFamilyPointer* first, CFamilyPointer* last, const alloc_tag& tag);   // 0x007CA090
struct CFamilyVector {
    CFamilyPointer* _M_start;
    CFamilyPointer* _M_finish;
    CFamilyPointer* _M_end_of_storage;
    CFamilyVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CFamilyVector() { DestroyFamilyRange(_M_start, _M_finish, alloc_tag()); if (_M_start) free(_M_start); }
    void push_back(const CFamilyPointer& value);             // 0x007CA350
    unsigned long size() const { return _M_finish - _M_start; }
    CFamilyPointer& operator[](unsigned long n) { return _M_start[n]; }
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector() { if (_M_start) free(_M_start); }
    void push_back(const long& value);                       // 0x007CA160
    CLongVector& operator=(const CLongVector& other);        // 0x004E24BA
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    long operator[](unsigned long n) const { return _M_start[n]; }
};

struct CDefinitionManager {
    bool GetDef(long index, CFamilyPointer* out) const;      // 0x0074D220
    long GetDefGlobalIndexFromName(const CCharString& name); // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();         // 0x0044C6B0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
    const SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x58 - 0x4c];
    CCharString ScriptName;                                  // +0x58
    unsigned char _pad5c[0x74 - 0x5c];
    CCharString DefName;                                     // +0x74
    unsigned char _pad78[0x91 - 0x78];
    unsigned char Flags91;                                   // +0x91 bit 0 Dead
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    void Kill(bool immediately);                             // 0x004C9B80
    template<class T> __forceinline bool GetInterface(long interfaceType, T** ppOut)
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
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        GetInterface(interfaceType, &tc);
        return tc;
    }
    template<class T> __forceinline T* GetTCConst(long interfaceType) const
    {
        T* tc;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            tc = (T*)it->component;
        }
        return tc;
    }
};
struct CRightHandedSet;
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                           unsigned long flags, const CCharString& name);   // 0x00703210
};
struct CThingMarker : CThing {
    static CThingMarker* __fastcall Create(long defIndex, const C3DVector* position, const CCharString& name);   // 0x0083D460
};
bool __fastcall IS_THING_ALIVE(const struct CBaseIntelligentPointer& p);   // 0x004CC340
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                           // +0x04
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item);                  // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
};
#pragma pack(pop)
struct CTCHeroExperience {
    long GetRoughExperienceLevel() const;                    // 0x006D64B0
};
struct CTCCreatureGenerator {
    unsigned char _pad00[0x0c];
    CLongVector CreatureFamilies;                            // +0x0c
    unsigned char _pad18[0x4e - 0x18];
    bool ReadyToInitialise;                                  // +0x4e
    void CloneFromCreator(CTCCreatureGenerator* creator);    // 0x00749E40
    void SetAsActive(bool active);                           // 0x00749DD0
};
struct CPlayer {
    CThing* GetControlledCreature();                         // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                          // 0x00449960
};
struct CGameSettings { unsigned char _pad[0xc8]; float GeneratorRespawnSeconds; };   // +0xc8
struct CGameRules { unsigned char _pad[0xd0]; CGameSettings* Settings; };              // +0xd0
struct CWorld {
    unsigned char _pad00[0x10];
    CGameRules* Rules;                                       // +0x10
    bool IsCreatureGenerationEnabled(const CThing& thing) const;   // 0x0049EA40
    unsigned long* GetPRandomSeed();                         // 0x0049D810
};
#pragma pack(push, 1)
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                     // +0x04
    bool    ToBeRemoved;                                     // +0x08
    bool    IsInGlobalUpdateTCs;                             // +0x09
    unsigned char _pad0a[2];
    const CWorld* GetWorldConst() const;                     // 0x00686A80
    CWorld* GetWorld();                                      // 0x00686A70
    CPlayerManager* GetPlayerManager();                      // 0x00686D50
    CDefinitionManager* GetDefinitionManager() const;        // 0x006869D0
};
class CTCCreatureGeneratorCreator : public CTCBase {
public:
    long FrameToRegenerate;                                  // +0x0c
    bool Generated;                                          // +0x10
    CIntelligentPointerThing PGenerator;                     // +0x11 CIntelligentPointer<CThing>
    unsigned char _pad19[0x20 - 0x19];
    bool IsGeneratorAlive();                                 // 0x007C98A0
    void FrameUpdate();
};
#pragma pack(pop)

void CTCCreatureGeneratorCreator::FrameUpdate()
{
    if (!Generated) {
        if (GFGetCurrentFrame() >= FrameToRegenerate && GetWorldConst()->IsCreatureGenerationEnabled(*ParentThing)) {
            CTCCreatureGenerator* generator;
            ParentThing->GetInterface(0x3d, &generator);
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
            if (hero && !(hero->Flags91 & 1) && !generator->CreatureFamilies.empty()) {
                const CTCHeroExperience* experience = ((const CThing*)hero)->GetTCConst<CTCHeroExperience>(0x68);
                long lowestLevel = -1;
                long bestLevel = -1;
                long heroLevel = experience->GetRoughExperienceLevel() + 1;
                unsigned long i;
                for (i = 0; i < generator->CreatureFamilies.size(); i++) {
                    CFamilyPointer family;
                    GetDefinitionManager()->GetDef(generator->CreatureFamilies[i], &family);
                    if (family) {
                        long level = family->Level;
                        if (level <= heroLevel && level > bestLevel)
                            bestLevel = level;
                        if (lowestLevel == -1 || level < lowestLevel)
                            lowestLevel = level;
                    }
                }
                if (bestLevel == -1)
                    bestLevel = lowestLevel;
                if (bestLevel != -1) {
                    CFamilyVector candidates;
                    for (i = 0; i < generator->CreatureFamilies.size(); i++) {
                        CFamilyPointer family;
                        GetDefinitionManager()->GetDef(generator->CreatureFamilies[i], &family);
                        if (family) {
                            if (family->Level == bestLevel)
                                candidates.push_back(family);
                        }
                    }
                    unsigned long count = candidates.size();
                    CFamilyPointer chosen = candidates[GFRandom(count, GetWorld()->GetPRandomSeed())];
                    if (chosen) {
                        CThing* thing;
                        if (chosen->MarkerDefIndex > 0) {
                            thing = CThingObject::Create(chosen->MarkerDefIndex, &ParentThing->GetPosition(), 4, 0, 0, ParentThing->DefName);
                        } else {
                            long markerIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(CCharString("MARKER_CREATURE_GENERATOR_INVISIBLE"));
                            thing = CThingMarker::Create(markerIndex, &ParentThing->GetPosition(), ParentThing->DefName);
                        }
                        if (thing && !(thing->Flags91 & 1)) {
                            thing->ScriptName = ParentThing->ScriptName;
                            CTCCreatureGenerator* newGenerator = thing->GetTC<CTCCreatureGenerator>(0x3d);
                            newGenerator->CloneFromCreator(generator);
                            if (IS_THING_ALIVE(PGenerator))
                                PGenerator.GetPItem()->Kill(false);
                            PGenerator = thing;
                            FrameToRegenerate = 0;
                            if (newGenerator) {
                                long familyIndex = GetDefinitionManager()->GetDefGlobalIndexFromName(chosen->GetName());
                                CLongVector families;
                                families.push_back(familyIndex);
                                newGenerator->CreatureFamilies = families;
                                newGenerator->ReadyToInitialise = true;
                            }
                        }
                    }
                }
            }
            generator->SetAsActive(false);
        }
        Generated = true;
    }
    if (FrameToRegenerate == 0 && !IsGeneratorAlive()) {
        float seconds = GetWorldConst()->Rules->Settings->GeneratorRespawnSeconds;
        long framesPerSecond = g_FramesPerSecond;
        FrameToRegenerate = (long)(GFGetCurrentFrame() + framesPerSecond * seconds);
    }
}
