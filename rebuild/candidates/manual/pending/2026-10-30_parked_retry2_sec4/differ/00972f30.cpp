// CAIStateGroup_JackOfBladesStage2::SetupArena() -- base arena setup, then collect the live "jack_pillar<n>" markers (n = 1, 2, ... until one is missing) within 100 units, every OBJECT_FRESCO_DOME_PILLAR_01 thing in the 100-unit box, the JACK_TO_LOOK cutscene look-at marker, and switch the camera manager to CAMERA_MANAGER_SET_JACK_BOSS_FIGHT
// Grade: DIFFER(1026v1026) nd=174 -- REBASED: the parked source no longer compiled (engine/CAIStateGroup_JackOfBladesStage2.h now names +0x34 as one 0x18 pad and +0x4c PCutsceneLookAtMarker); accessors repointed. Residue: frame slot layout only (retail sprintf buffer at +0x4c, the top of the frame; ours lower). Tried marker/index order, name[32]/[40] (128 lines but frame +8). Next: find which local retail places below the name buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972F30, length 1026 bytes (+ int3 padding; /GS: the 32-byte name buffer).
// Callees: CAIStateGroup_JackOfBladesBase::SetupArena @ 0x00972830, sprintf @ 0x00BFEB10 ("jack_pillar%d" @ 0x0129772C),
//          CCharString(const char*, long) @ 0x0099EBF0 / copy ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, CThing::GetPosition
//          @ 0x004C73D0, __ftol2 @ 0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
//          CThingSearchTools::GetPFirstThingInRoughArea @ 0x007FD110 (same-TU noinline stand-in, see below),
//          CThingSearchTools::GetAllThingsInRoughArea<CThingFilter_HasDefGlobalIndex> @ 0x0096BCB0 (fills a vector<CThing*>),
//          CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
//          CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (vtable
//          0x01238C6C), vector _M_insert_overflow @ 0x004AC940, CAIStateGroupBase::GetPWorld @ 0x008FCFC0, the world's
//          +0x34 camera host slot 9, GetPDefinitionManager @ 0x008FCFB0, GetDef<CCameraManagerSetDef> @ 0x004AB990,
//          CGameCameraManager::SetCameraManagerSetDef @ 0x0069AC90, free @ 0x00BFEA14, __security_check_cookie @ 0x00BFE9F9.
// Flags: /GS
// Shape: the pillar loop is the landed HornetQueen 0x0095AB50 form (`do { ...; if (IS_THING_ALIVE(t)) push_back; } while
//          (IS_THING_ALIVE(t))`, the search a same-TU noinline stand-in so the loop-bottom null test folds away).
// Layout: engine/CAIStateGroup_JackOfBladesStage2.h; the PDB vectors are 16 bytes but retail STLport vectors are 12, so
//          PillarMarkers sits at +0x34, Pillars at +0x40 and PCutsceneLookAtMarker at +0x4c (header +0x54).
#include "engine/CAIStateGroup_JackOfBladesStage2.h"

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector {
    float X, Y, Z;
};
struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
};
#define IS_THING_ALIVE(p) ((p) != 0 && !((p)->m_flags91 & 1))
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
struct CThingFilter_HasTrackName {
    CCharString Name;
    CThingFilter_HasTrackName(const CCharString& name) : Name(name) { }
};
struct CThingFilter_HasDefGlobalIndex {
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) { }
};
template <class T> struct CRetailVector;
struct CThingSearchTools {
    template <class TFilter, class T> bool GetPFirstThingInRoughArea(const C2DBoxI& area, const TFilter& filter, T** out) const;   // 0x007FD110
    CThing* FindFirstInArea(const C2DBoxI& area, const void* filter) const;   // stand-in placeholder (not a retail symbol)
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter,
                                 CRetailVector<CThing*>* things);   // 0x0096BCB0
};
// Same-TU stand-in for 0x007FD110: only its visibility (it writes nothing but *out) matters to the caller's code.
template <class TFilter, class T> __declspec(noinline) bool CThingSearchTools::GetPFirstThingInRoughArea(const C2DBoxI& area, const TFilter& filter, T** out) const
{
    CThing* thing = FindFirstInArea(area, &filter);
    if (thing == 0)
        return false;
    *out = (T*)thing;
    return true;
}
extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);   // 0x00BFEB10
extern "C" void __cdecl free(void* p);                                    // 0x00BFEA14
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) free(_M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x004AC940
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CCameraManagerSetDef : CRefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
};
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointer<CCameraManagerSetDef>* def);   // 0x004AB990
    long GetDefGlobalIndexFromName(const CCharString& name);                        // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CGameCameraManager {
    void SetCameraManagerSetDef(const CDefPointer<CCameraManagerSetDef>& def);       // 0x0069AC90
};
struct CCameraHost {                            // the world's +0x34 object
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9 (+0x24)
};
struct CWorld {
    unsigned char _pad[0x34];
    CCameraHost* PCameraHost;                   // +0x34
    CCameraHost* GetPCameraHost() { return PCameraHost; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupJackOfBladesStage2Methods : CAIStateGroup_JackOfBladesStage2 {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CRetailVector<CIntelligentPointer>& PillarMarkers() { return *(CRetailVector<CIntelligentPointer>*)_pad_0x34; }
    CRetailVector<CIntelligentPointer>& Pillars() { return *(CRetailVector<CIntelligentPointer>*)(_pad_0x34 + 0xc); }
    CIntelligentPointer& CutsceneLookAtMarker() { return *(CIntelligentPointer*)PCutsceneLookAtMarker; }
    void BaseSetupArena();                      // 0x00972830 CAIStateGroup_JackOfBladesBase::SetupArena
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    CWorld* GetPWorld();                        // 0x008FCFC0
    CDefinitionManager* GetPDefinitionManager();   // 0x008FCFB0
    void SetupArena();
};

void CAIStateGroupJackOfBladesStage2Methods::SetupArena()
{
    BaseSetupArena();
    {
        long index = 0;
        CThing* marker;
        do {
            ++index;
            marker = 0;
            char name[28];
            sprintf(name, "jack_pillar%d", index);
            {
                CCharString track(name);
                CThingFilter_HasTrackName filter(track);
                const C3DVector& pos = GetCreature()->GetPosition();
                long x = (long)pos.X;
                long y = (long)pos.Y;
                PeekThingSearchTools()->GetPFirstThingInRoughArea(C2DBoxI(x - 100, y - 100, x + 100, y + 100), filter, &marker);
            }
            if (IS_THING_ALIVE(marker))
                PillarMarkers().push_back(CIntelligentPointer(marker));
        } while (IS_THING_ALIVE(marker));
    }

    CRetailVector<CThing*> things;
    {
        long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_FRESCO_DOME_PILLAR_01");
        CThingFilter_HasDefGlobalIndex filter(defIndex);
        const C3DVector& pos = GetCreature()->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        PeekThingSearchTools()->GetAllThingsInRoughArea(C2DBoxI(x - 100, y - 100, x + 100, y + 100),
                                                        filter, &things);
    }
    for (CThing** it = things.begin(); it != things.end(); ++it)
        Pillars().push_back(CIntelligentPointer(*it));

    {
        CThing* lookAt = 0;
        {
            CCharString track("JACK_TO_LOOK");
            CThingFilter_HasTrackName filter(track);
            const C3DVector& pos = GetCreature()->GetPosition();
            long x = (long)pos.X;
            long y = (long)pos.Y;
            PeekThingSearchTools()->GetPFirstThingInRoughArea(C2DBoxI(x - 100, y - 100, x + 100, y + 100), filter, &lookAt);
        }
        if (lookAt)
            CutsceneLookAtMarker() = CIntelligentPointer(lookAt);
    }
    CGameCameraManager* camera;
    if (GetPWorld()->GetPCameraHost()->GetCameraManager(&camera)) {
        CDefPointer<CCameraManagerSetDef> def;
        GetPDefinitionManager()->GetDef("CAMERA_MANAGER_SET_JACK_BOSS_FIGHT", &def);
        camera->SetCameraManagerSetDef(def);
    }
}
