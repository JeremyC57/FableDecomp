// CWorldMap::DrawGetMyPlayerMapNumber() const -- draw interface: the map number under the local player's recorded position (1 when unknown)
// Grade: DIFFER(87v85) -- retail materialises the record address and tests the Set byte in memory (`add eax,ecx ; cmp byte ptr [eax],0`, begin in ecx); the lane folds the load into `mov cl,[eax+edx] ; add eax,edx ; test cl,cl` (begin in edx), the rest is identical
// Leaf: ?DrawGetMyPlayerMapNumber@CWorldMap@@UBEJXZ
// Retail: Fable.exe @ 0x004FC760, 85 bytes. PDB ?DrawGetMyPlayerMapNumber@CWorldMap@@UBEJXZ
// Virtual slot 7 of the CIDrawWorldMap interface (vtable 0x01244AB4, RTTI: CWorldMap : CBaseClassNonCopyable,
// CIDrawWorldMap at +4): `this` arrives at the +4 subobject, World (+0x08) is [esi+4], PlayerPositionInfo (+0x78) is
// [esi+74h] and the primary-vtable call goes through `lea ecx,[esi-4]`.
// Callees: CWorld::PeekMyPlayer() const 0x0049D9D0, primary virtual slot 16 GetMapNumberAt(const C3DVector&)
// 0x004FC0C0 (+0x40). Float pool 0.0f 0x0122DEDC.
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays and
// the map member are 12 bytes, so a local model is used): +0x08 World, +0x78 PlayerPositionInfo (vector of the 13-byte
// packed CPlayerPositionInfo {bool Set; C3DVector Pos;}, PDB +0x8C). CPlayer::Number +0x28 (engine/CPlayer.h).
// Shape: the record is indexed by the player's number (`imul eax,eax,0Dh`); a set record with positive X and Y asks
// the primary interface for the map there, every other path returns 1.
#include "engine/CPlayer.h"

#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };
class CPlayerPositionInfo {
public:
    bool      Set;                              // +0x00
    C3DVector Pos;                              // +0x01
};
#pragma pack(pop)

template <class T> class CVector {             // STLport vector<T> (12 bytes in retail)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};

class CWorld {
public:
    const CPlayer* PeekMyPlayer() const;        // 0x0049D9D0
};

class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass {
public:
    virtual ~CBaseClassNonCopyable();
};

class CIDrawWorldMap {                          // vtable 0x01244AB4 in CWorldMap
public:
    virtual long DrawGetNoMaps() const = 0;                     // slot 0
    virtual void d01() const = 0; virtual void d02() const = 0; virtual void d03() const = 0;
    virtual void d04() const = 0; virtual void d05() const = 0; virtual void d06() const = 0;
    virtual long DrawGetMyPlayerMapNumber() const = 0;          // slot 7
};

class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;    // slot 16 (+0x40) 0x004FC0C0
    CWorld*              World;                 // +0x08
    void*                Component;             // +0x0C
    void*                PThingMapManager;      // +0x10
    unsigned char        _pad14[0x64];
    CVector<CPlayerPositionInfo> PlayerPositionInfo;    // +0x78 (PDB +0x8C)
    virtual long DrawGetMyPlayerMapNumber() const;
};

long CWorldMap::DrawGetMyPlayerMapNumber() const
{
    const CPlayer* player = World->PeekMyPlayer();
    if (player)
    {
        const CPlayerPositionInfo& info = PlayerPositionInfo[player->Number];
        if (info.Set)
        {
            const C3DVector& pos = info.Pos;
            if (pos.X > 0.0f && pos.Y > 0.0f)
                return GetMapNumberAt(pos);
        }
    }
    return 1;
}
