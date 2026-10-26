// CThingAICreature::OnHeroPush(CThing* pusher, float distance, const C3DVector& direction) -- when the pusher is the living main hero carrying the hero component, the creature's push component (added on demand) accumulates the distance, takes the direction and restarts its frame counter
// Leaf: OnHeroPush
// Retail: Fable.exe (TLC, VC7.1) @ 0x008328D0, 218 bytes (+6 int3). `ret 0Ch`.
// Callees: CThing::GetPlayerManager() 0x004C7A00 (manifest "CViewBase::GetDisplayEngine" propagated: g_pThingManager->PlayerManager),
//          CPlayerManager::GetMainPlayer() 0x00449970, CPlayer::GetHeroThing() 0x00487DC0, CVectorMap::LowerBound(const long*)
//          0x0040F020, CCharString::CCharString(const char*, long) 0x0099EBF0 ("CTCOnHeroPush" @ 0x0123CBB0), CThing::AddTC
//          (const CCharString&, bool, CTCParamsBase*) 0x004C9D60, ~CCharString 0x0099EAE0.
// Shape: the hero's two id words are compared against the pusher's; the pusher's 0x51 bit is word +0x28 bit 17 and the
//        creature's 0xC4 bit word +0x38 bit 4; the lookup key and the name temporary share the pusher's dead home slot;
//        the component pointer (esi, the `this` register reused) joins from both arms before the three stores.
//        The key and the name share that slot only with a same-TU noinline LowerBound stand-in (its address no longer
//        escapes) and the name as a named local in the AddTC block (008967f0 idiom; with the temporary argument 35 lines).
// Layout: CThing UniqueID +0x18, presence bits +0x20, Components +0x44, Dead +0x91 bit 0; CTCOnHeroPush DistPushed +0x0C,
//         PushDir +0x10, FrameSincePushed +0x1C (Ego_r.pdb).
class C3DVector { public: float X, Y, Z; };
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};
class CTCBase; class CTCParamsBase;
class CTCOnHeroPush { public:                               // interface type 0xC4
    unsigned char _pad00[0x0c];
    float         DistPushed;                        // +0x0c
    C3DVector     PushDir;                           // +0x10
    long          FrameSincePushed;                  // +0x1c
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (not the retail body).
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                       // +0x18
    unsigned long    m_presentBits[9];               // +0x20 UsingComponents
    CInterfaceMap    m_interfaces;                   // +0x44
    unsigned char    _pad4c[0x91 - 0x4c];
    unsigned char    Dead : 1;                       // +0x91 bit 0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
};
class CPlayer { public:
    CThing* GetHeroThing() const;                    // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                        // 0x00449970
};
class CThingAICreature : public CThing { public:
    CPlayerManager* GetPlayerManager();              // 0x004C7A00
    void OnHeroPush(CThing* pusher, float distance, const C3DVector& direction);
};
void CThingAICreature::OnHeroPush(CThing* pusher, float distance, const C3DVector& direction)
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (hero && !hero->Dead && hero->UniqueID == pusher->UniqueID && pusher->HasTC(0x51)) {
        CTCOnHeroPush* push;
        if (!GetInterface(&push, 0xc4)) {
            CCharString name("CTCOnHeroPush", -1);
            push = (CTCOnHeroPush*)AddTC(name, true, 0);
        }
        push->DistPushed += distance;
        push->PushDir = direction;
        push->FrameSincePushed = 0;
    }
}