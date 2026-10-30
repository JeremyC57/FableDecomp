// CTCCreatureHitNotification::DrawHitNotification(const CEnginePrimitivePrepareInfo&) const -- hand every hit flash's geometry to the engine as a weapon-trail primitive (global weapon-trail graphic, current world time in seconds)
// Leaf: ?DrawHitNotification@CTCCreatureHitNotification
// Grade: DIFFER(227v227) -- register allocation (this edi/ebx swap, info edx/eax, end() compare folding); tried (this sweep): reversed end() != it compare, named primitive local
// Retail 0x00864FF0, 0xE3 bytes, __thiscall `ret 4`. PDB (Ego_r): ?DrawHitNotification@CTCCreatureHitNotification@@QBEXABVCEnginePrimitivePrepareInfo@@@Z.
// Shape: the engine pointer and the thing's UniqueID are loop-body locals (retail holds the engine in esi across the
// temporary's constructor and loads the 64-bit id into eax:ecx before the pushes; `info.Engine->` re-reads it after the
// constructor); the definition manager's global def is a plain pointer member (a CDefPointer operator-> gives
// `add eax,0D0h ; mov eax,[eax]`); the frames-per-second global is a named local read before GetWorldFrame (copied to
// the stack across the call, then `fild ; fidiv`); the temporary's inline STLport list destructor is clear() + free(sentinel).
// Tried: CDefPointer member (a), engine local (b), engine + id locals (c, best), iterator declared at the top (d, same as c).
// Caller: the creature's primitive preparation @ 0x00663D75, behind HasTC(0xCD) (CTCCreatureHitNotification's
// GetInterfaceType returns 0xCD, vtable 0x012436CC slot 21).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CTCBase::GetWorld() const @ 0x00686A80, the static
// CWorld::GetWorldFrame() @ 0x0049D870, CEnginePrimitiveWeaponTrail ctor @ 0x007703B0 (landed), CIEngine vtable slot 20
// (+0x50), free @ 0x00BFEA14 (the temporary's inline STLport list destructor).
// Globals: g_framesPerSecond @ 0x01375550.
// Layout (retail): CTCBase (engine/CTCBase.h) ParentThing +0x04; Flashes (STLport vector<NCreatureHitNotification::CFlash>,
// 8-byte elements: the 4-byte list FlashGeometry + CreationFrame) +0x0C; CThing UniqueID (unsigned __int64) +0x18; the
// definition manager's CDefPointer<CGlobalDef> +0xD0, CGlobalDef GraphicWeaponTrail +0x5C; CEnginePrimitivePrepareInfo
// Engine +0x34.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
#include "engine/CTCBase.h"

struct CWeaponTrailEntry { unsigned long m_data[7]; };   // 28 bytes
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };

struct CWeaponTrailList {                                // STLport list<CWeaponTrailEntry>: the node pointer only
    SListNode* m_node;
    void clear()
    {
        SListNode* cur = (SListNode*)m_node->m_next;
        while (cur != m_node) {
            SListNode* tmp = cur;
            cur = (SListNode*)cur->m_next;
            free(tmp);
        }
        m_node->m_next = m_node;
        m_node->m_prev = m_node;
    }
    ~CWeaponTrailList()
    {
        clear();
        free(m_node);
    }
};

struct CEnginePrimitive {                                // 12 bytes
    long          Type;
    long          InterpolationSet;
    unsigned char ThemeChannelDesc[4];
};

struct CEnginePrimitiveWeaponTrail : CEnginePrimitive {
    CWeaponTrailList Trail;                              // +0x0c
    unsigned long    GraphicIndex;                       // +0x10
    unsigned long    GraphicFrame;                       // +0x14
    long             TrailType;                          // +0x18 EWeaponTrailType
    float            SkipPoints;                         // +0x1c
    float            FadeStart;                          // +0x20
    unsigned char    Alpha;                              // +0x24
    float            CurrentTime;                        // +0x28
    float            FallOffTime;                        // +0x2c
    CEnginePrimitiveWeaponTrail(unsigned __int64 handle, const CWeaponTrailList& trail, unsigned long graphicIndex,
                                unsigned long graphicFrame, long trailType, unsigned char alpha, float skipPoints,
                                float currentTime, float fallOffTime, float fadeStart, long interpolationSet);   // 0x007703B0
};

struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void SetPrimitive(const CEnginePrimitive& primitive);   // slot 20 (+0x50)
};

struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CIEngine*     Engine;                                // +0x34
};

struct CGlobalDef {
    unsigned char _pad00[0x5c];
    long          GraphicWeaponTrail;                    // +0x5c
};
struct CDefinitionManager {
    unsigned char _pad00[0xd0];
    CGlobalDef*   PGlobalDef;                            // +0xd0 (CDefPointer<CGlobalDef>)
};

struct CWorld {
    static long GetWorldFrame();                         // 0x0049D870
};
extern long g_framesPerSecond;                           // 0x01375550

struct CThing {
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                           // +0x18
};

namespace NCreatureHitNotification
{
    struct CFlash {
        CWeaponTrailList FlashGeometry;                  // +0x00
        long             CreationFrame;                  // +0x04
    };
}

struct CFlashVector {                                    // STLport vector<CFlash>
    NCreatureHitNotification::CFlash* _M_start;
    NCreatureHitNotification::CFlash* _M_finish;
    NCreatureHitNotification::CFlash* _M_end_of_storage;
    const NCreatureHitNotification::CFlash* begin() const { return _M_start; }
    const NCreatureHitNotification::CFlash* end() const { return _M_finish; }
};

struct CTCCreatureHitNotification : CTCBase {
    CFlashVector Flashes;                                // +0x0c
    CDefinitionManager* GetDefinitionManager() const;    // 0x006869D0 (CTCBase)
    const CWorld* GetWorld() const;                      // 0x00686A80 (CTCBase)
    void DrawHitNotification(const CEnginePrimitivePrepareInfo& info) const;
};

void CTCCreatureHitNotification::DrawHitNotification(const CEnginePrimitivePrepareInfo& info) const
{
    long graphic = GetDefinitionManager()->PGlobalDef->GraphicWeaponTrail;
    const CWorld* world = GetWorld();
    long framesPerSecond = g_framesPerSecond;
    float time = (float)world->GetWorldFrame() / framesPerSecond;
    for (const NCreatureHitNotification::CFlash* it = Flashes.begin(); it != Flashes.end(); ++it) {
        CIEngine* engine = info.Engine;
        unsigned __int64 id = ParentThing->UniqueID;
        engine->SetPrimitive(CEnginePrimitiveWeaponTrail(id, it->FlashGeometry, graphic, 0, 2, 1,
                                                         0.0f, time, 100.0f, 0.0f, 0));
    }
}
