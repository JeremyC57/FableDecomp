// CBulletTimeManager::SetBulletTimeSlowdownMultiplier(long, void*, float) -- register (or replace) one requester's bullet-time slowdown and sound pitch, then recompute the pitch
// Leaf: ?SetBulletTimeSlowdownMultiplier@CBulletTimeManager@@QAEXJPAXM@Z
// Retail: Fable.exe @ 0x004C6170, 81 bytes (16-aligned /O2 entry), thiscall `ret 0Ch`.
// Identity: Ego_r.pdb `?SetBulletTimeSlowdownMultiplier@CBulletTimeManager@@QAEXJPAXM@Z`, the catalog row name; callers
// CTCBulletTime 0x0079D640, 0x007AAEDC, 0x00857B57, 0x00890AE1/0x00890B2B.
// Callees: map<void*, CBulletTimeManager::CBulletTimeInfo>::operator[](void* const&) 0x004C6E00 (STLport; landed),
// CBulletTimeManager::RecalculateSoundPitch() 0x004C5F10 (private, PDB `?RecalculateSoundPitch@CBulletTimeManager@@AAEXXZ`).
// Layout: engine/CBulletTimeManager.h (retail offsets): BulletTimeInfoMap +0x00 (12-byte STLport map: header, node count),
// Override +0x0C, UseDefaultMultiplier +0x0D, LastStrikeActionTick +0x10. The header keeps the map as opaque bytes, so the
// class is modelled locally with the real map. CBulletTimeInfo (Ego_r.pdb): Multiplier +0, SoundPitch +4.
// Shape: with no override and an empty map the strike tick is reset; the CBulletTimeInfo(float, float) temporary (the long
// first argument converted by `fild`) is built in the frame and assigned through operator[]'s reference.
class CWorld;
class CPlayerManager;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    _Tp& operator[](const _Key& __k);                    // out of line
    bool empty() const { return _M_node_count == 0; }
    void*        _M_header;                              // +0x00
    unsigned int _M_node_count;                          // +0x04
    _Compare     _M_key_compare;                         // +0x08
};
}
class CBulletTimeManager { public:
    class CBulletTimeInfo { public:
        CBulletTimeInfo() {}
        CBulletTimeInfo(float multiplier, float soundPitch) : Multiplier(multiplier), SoundPitch(soundPitch) {}
        float Multiplier;                                // +0x00
        float SoundPitch;                                // +0x04
    };
    std::map<void*, CBulletTimeInfo> BulletTimeInfoMap;  // +0x00
    bool            Override;                            // +0x0C
    bool            UseDefaultMultiplier;                // +0x0D
    long            LastStrikeActionTick;                // +0x10
    CWorld*         World;                               // +0x14
    CPlayerManager* PlayerManager;                       // +0x18
    void SetBulletTimeSlowdownMultiplier(long multiplier, void* requester, float soundPitch);
private:
    void RecalculateSoundPitch();                        // 0x004C5F10
};
void CBulletTimeManager::SetBulletTimeSlowdownMultiplier(long multiplier, void* requester, float soundPitch)
{
    if (!Override && BulletTimeInfoMap.empty())
        LastStrikeActionTick = 0;
    UseDefaultMultiplier = false;
    BulletTimeInfoMap[requester] = CBulletTimeInfo((float)multiplier, soundPitch);
    RecalculateSoundPitch();
}