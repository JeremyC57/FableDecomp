// CTCTargetingPlayer::vfunc_28() -- the targeted thing if it is still a valid target for the player (the locked target while Z-targeting; else a targetable-flag-4 thing, an interface-0x4E thing or an enemy), otherwise null; a missing or dead target is returned as is
// Grade: DIFFER(150v152) -- register allocation: retail target in esi / map cursor in edi and `mov ebx,[ebx+4] ; mov edx,esi ; mov ecx,ebx` for AreThingsEnemies, ours target in edi / cursor in esi and `mov ecx,[ebx+4]`
// Leaf: vfunc_28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008031E0, length 152 bytes (padded row 160); CTCTargetingPlayer vtable 0x0124280C slot 28.
// Callees: virtual slot 33 GetPTargetedThing (+0x84, 0x00802610), CThing::GetPlayer @ 0x004C7A10, virtual slot 36 GetPTargetLockedThing (+0x90,
// the thunk 0x004E2328 = GetPItem of the +0xE4 intelligent pointer), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined
// GetInterface for CTCTargeted, interface 9 = bit 9 of CThing+0x20), AreThingsEnemies(const CThing*, const CThing*) __fastcall @ 0x0076D2F0.
// Layout: CPlayer::ZTargeting +0x20A (Ego_r.pdb = retail); CTCTargeted::TargetingFlags +0x0C; CThing dead flag +0x91 bit 0, interface
// 0x4E = bit 14 of +0x28.
// Shape: alive test on the fetched thing, then early returns; the CTCTargeted lookup is the out-parameter GetInterface; the thing is
// returned from esi at the shared exit.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing;
struct CPlayer {
    unsigned char _pad00[0x20a];
    bool ZTargeting;                            // +0x20a
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
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
    CPlayer* GetPlayer();                       // 0x004C7A10
};
bool __fastcall AreThingsEnemies(CThing* a, CThing* b);   // 0x0076D2F0
struct CTCTargeted {
    unsigned char _pad00[0x0c];
    unsigned long TargetingFlags;               // +0x0c
};
struct CTCTargetingPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual CThing* vfunc_28();                 // slot 28
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTargetedThing();        // slot 33 (+0x84)
    virtual void v34(); virtual void v35();
    virtual CThing* GetPTargetLockedThing();    // slot 36 (+0x90)
    CThing* ParentThing;                        // +0x04 (CTCBase)
};

CThing* CTCTargetingPlayer::vfunc_28()
{
    CThing* target = GetPTargetedThing();
    if (target && !(target->m_flags91 & 1)) {
        if (ParentThing->GetPlayer()->ZTargeting) {
            if (GetPTargetLockedThing() != target)
                return 0;
        } else {
            CTCTargeted* targeted;
            if (!(target->GetInterface(&targeted, 9) && (targeted->TargetingFlags & 4))
                && !target->HasTC(0x4e) && !AreThingsEnemies(ParentThing, target))
                return 0;
        }
    }
    return target;
}
