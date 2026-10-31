// CTCDExperienceOrb::FrameUpdate() -- experience orbs flock: gather the orbs within 5 units (rough-area search), merge with one that is touching once the merge delay has run out, else accelerate towards the group centre (fast inverse square root); count the delay down; when the game is not paused, pull the orb towards the living hero while his "collect" action is queued, and once the orb has expired kill it (if its fade component is idle) or start a 1.5 s fade
// Grade: DIFFER(1054v1115) 277 diff lines (parked 1067/301): the opening round is the engine's inline-assembly ceiling helper (landed retail_asm CeilToLong 0x0079C5F0 / 0x00747DA0 precedent): prologue, ebp frame and the helper block now match retail. Left: frame 0x60 v 0x48 -- retail reuses the helper's slots (back / value) for the later rough-area / filter locals, ours keeps them apart (scoping the call does not change it); the rest is the parked first draft. Retail-asm evidence for landing: 0x0077A45B fld [ebp-10h] / 0x0077A45E fadd [ebp-0Ch] / 0x0077A461 fistp dword ptr [ebp-4] / 0x0077A464 fild / 0x0077A467 fsub [ebp-8] / 0x0077A46D mov eax,[ebp-10h] ; cmp eax,[ebp-1Ch] ; jne ; dec dword ptr [ebp-4] (retry wave 2 sec2c)
// Was: DIFFER(1067v1115) -- lane gap: the opening round-to-int is an inlined __asm helper (fistp/fild/fsub in ebp slots) that C++ cannot reproduce; the rest is a semantic first draft
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077A430, 1120 bytes, ebp frame, no EH.
// Callees: CThing::GetPosition 0x004C73D0, __ftol2 0x00BFEA70, CTCBase::GetThingSearchTools 0x00686D90,
// GetPBestThingInRoughArea<filter> 0x0077ADF0, CTCPhysicsBase::ApplyAcceleration 0x006B0210, CombineWithExperienceOrb
// 0x0077A130, CTCBase::GetGameInfo 0x00686AE0 / GetPlayerManager 0x00686D40 (0x00686D50 const), CPlayerManager::
// GetMainPlayer 0x00449970 / 0x00449960 / GetMainPlayerIndex 0x00449980, CPlayer::GetCharacterThing 0x00487DC0,
// CGamePlayerInterface::IsGameActionEventInQueue 0x00446B10, C3DVector::Normalise 0x00A14480, LowerBound 0x0040F020,
// CThing::Kill 0x004C9B80 / AddTC 0x004C9D60, CTCFadeOutAndIn::FadeIn 0x006C3B70, free 0x00BFEA14.
// Shape note: the body opens with an inlined `__asm` round-to-int helper (fld/fadd 0.5/fistp/fild/fsub 1.0 compare in ebp
// slots) and an inlined table-driven fast inverse square root (0x013CE558); the first is not reachable from C++ (GOTCHAS).
extern "C" void __cdecl free(void* p);                       // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float Normalise();                                       // 0x00A14480
};
extern unsigned long g_InvSqrtTable[];                       // 0x013CE558
inline float FastInvSqrt(float value)
{
    unsigned long bits = *(unsigned long*)&value;
    unsigned long result = ((0x17c - ((bits >> 23) & 0xff)) << 22) & 0xff800000;
    result |= g_InvSqrtTable[(bits >> 17) & 0x7f] << 15;
    float y = *(float*)&result;
    return (3.0f - y * y * value) * y * 0.5f;
}
// the engine's inline-assembly FPU ceiling helper (standalone copy 0x004FA9A0; landed inlined form 0x0079C5F0)
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
struct CThing;
struct CThingVector {
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
};
struct CRoughArea { long MinX, MinY, MaxX, MaxY; };
struct COrbFilter {
    void* Thing;
    C3DVector* Position;
    float Radius;
};
struct CThingSearchTools {
    void GetPBestThingInRoughArea(const CRoughArea& area, const COrbFilter& filter, CThingVector* out);   // 0x0077ADF0
};
struct CTCPhysicsBase { void ApplyAcceleration(const C3DVector& acceleration); };   // 0x006B0210
struct CTCFadeOutAndIn { void FadeIn(float seconds, bool a); };                     // 0x006C3B70
struct CTCFadeState { unsigned char _pad[0x0c]; bool Fading; bool Waiting; };        // interface 0xf9
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* Physics;                                 // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                                   // +0x91 bit 0 Dead
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    void Kill(bool immediately);                             // 0x004C9B80
    CTCBase* AddTC(const CCharString& name, bool persist, void* params);   // 0x004C9D60
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CGamePlayerInterface {
    bool IsGameActionEventInQueue(long action, long a, long player);   // 0x00446B10
};
struct CPlayer {
    unsigned char _pad00[0x0c];
    CGamePlayerInterface* Interface;                         // +0x0c
    CThing* GetCharacterThing();                             // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                                // 0x00449970
    CPlayer* GetMainPlayer() const;                          // 0x00449960
    long GetMainPlayerIndex();                               // 0x00449980
};
struct CGameInfo { unsigned char _pad[0x54]; bool Paused; bool InCutscene; };
struct CTCDExperienceOrb {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    unsigned char _pad08[0x14 - 0x08];
    long FramesUntilCanCombine;                              // +0x14
    long FramesToLive;                                       // +0x18
    bool Expiring;                                           // +0x1c
    CThingSearchTools* GetThingSearchTools();                // 0x00686D90
    CGameInfo* GetGameInfo();                                // 0x00686AE0
    CPlayerManager* GetPlayerManager();                      // 0x00686D40
    const CPlayerManager* GetPlayerManagerConst();           // 0x00686D50
    void CombineWithExperienceOrb(CThing* orb);              // 0x0077A130
    void FrameUpdate();
};

void CTCDExperienceOrb::FrameUpdate()
{
    CThingVector orbs;
    float radius = 5.0f;
    long reach = CeilToLong(radius);
    const C3DVector& pos = ParentThing->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    CRoughArea area;
    area.MinX = x - reach;
    area.MinY = y - reach;
    area.MaxX = x + reach;
    area.MaxY = y + reach;
    C3DVector centre = ParentThing->GetPosition();
    COrbFilter filter;
    filter.Thing = ParentThing;
    filter.Position = &centre;
    filter.Radius = 5.0f;
    GetThingSearchTools()->GetPBestThingInRoughArea(area, filter, &orbs);
    if (orbs._M_start != orbs._M_finish) {
        unsigned long count = orbs._M_finish - orbs._M_start;
        C3DVector sum(0.0f, 0.0f, 0.0f);
        unsigned long i;
        for (i = 0; i < count; i++) {
            if (FramesUntilCanCombine <= 0) {
                const C3DVector& mine = ParentThing->GetPosition();
                const C3DVector& theirs = orbs._M_start[i]->GetPosition();
                float dx = theirs.X - mine.X;
                float dy = theirs.Y - mine.Y;
                float dz = theirs.Z - mine.Z;
                if (dz * dz + dy * dy + dx * dx < 0.0016f) {
                    CombineWithExperienceOrb(orbs._M_start[i]);
                    goto done;
                }
            }
            const C3DVector& other = orbs._M_start[i]->GetPosition();
            sum.X += other.X;
            sum.Y += other.Y;
            sum.Z += other.Z;
        }
        {
            float scale = 1.0f / count;
            sum.X *= scale;
            sum.Y *= scale;
            sum.Z *= scale;
            const C3DVector& mine = ParentThing->GetPosition();
            C3DVector d(sum.X - mine.X, sum.Y - mine.Y, sum.Z - mine.Z);
            float lengthSq = d.X * d.X + d.Y * d.Y + d.Z * d.Z;
            if (lengthSq > 0.0001f * 0.0001f) {
                float inv = FastInvSqrt(lengthSq);
                C3DVector acceleration(d.X * inv * 0.006f, d.Y * inv * 0.006f, d.Z * inv * 0.006f);
                ParentThing->Physics->ApplyAcceleration(acceleration);
            }
        }
    }
done:
    --FramesUntilCanCombine;
    if (GetGameInfo()->Paused || GetGameInfo()->InCutscene)
        return;
    {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing();
        if (hero && !(hero->Flags91 & 1)
            && GetPlayerManagerConst()->GetMainPlayer()->Interface->IsGameActionEventInQueue(0x1a, 2, GetPlayerManager()->GetMainPlayerIndex())) {
            const C3DVector& mine = ParentThing->GetPosition();
            const C3DVector& target = hero->GetPosition();
            C3DVector d(target.X - mine.X, target.Y - mine.Y, target.Z - mine.Z);
            float scale = 0.7f / d.Normalise();
            C3DVector acceleration(d.X * scale, d.Y * scale, d.Z * scale);
            ParentThing->Physics->ApplyAcceleration(acceleration);
        }
    }
    if (Expiring && --FramesToLive < 0) {
        CThing* thing = ParentThing;
        if (thing->m_presentBits[7] & 0x2000000) {
            CTCFadeState* fade = thing->GetTC<CTCFadeState>(0xf9);
            if (!fade->Waiting && !fade->Fading)
                thing->Kill(true);
        } else {
            CTCFadeOutAndIn* fade = (CTCFadeOutAndIn*)ParentThing->AddTC("CTCFadeOutAndIn", false, 0);
            fade->FadeIn(1.5f, true);
        }
    }
}
