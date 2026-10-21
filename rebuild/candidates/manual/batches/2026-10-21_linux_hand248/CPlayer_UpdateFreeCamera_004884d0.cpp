// CPlayer::UpdateFreeCamera() -- per-frame free-camera update: remember last frame's camera, and while the free camera is in use with a controlled creature, either point the camera at the creature's graphic helper point (YZ tracking) or turn it to face that point in XY (plain tracking), then publish the working camera as the free camera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004884D0, length 331 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb `?UpdateFreeCamera@CPlayer@@AAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice on ControlledCreature), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound 0x0040F020 (inline lookup of type 0x5B = CTCGraphicAppearance, bit 27 of +0x28),
// CTCGraphicAppearance helper-point position 0x004C10E0 (returns a C3DVector through a hidden pointer),
// CCamera::PointAt(const C3DVector&) 0x00A0C2B0, CCamera XY-angle setter 0x00A0C570 (takes the angle in turns),
// the fmod intrinsic `_CIfmod` 0x00BFEB28; pool constants 1/(2*pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
// 1.0 (double) 0x01237F00.
// Layout: engine/CPlayer.h for ControlledCreature +0x2C and the three CCamera blocks (CurrentFreeCamera +0x40,
// FreeCamera +0x78, OldFreeCamera +0xB0, 0x38 bytes each, copied with `rep movsd`). Retail's flag block sits 8 bytes
// below the PDB (PlayerModes is a narrower list in retail, as the landed SetControlledCreature 0x00487CF0 shows:
// InitialPlayerModeSet +0x216), so UsingFreeCamera / FreeCameraTrackingPlayer / FreeCameraTrackingPlayerYZ are read
// at retail +0x220/+0x221/+0x222 through a retail-offset view.
// Shape: the YZ-tracking arm (+0x222) is tested first and is an if/else-if with the XY arm (VC duplicates the tail
// copy into the first arm); the camera position is a `const C3DVector&` local (retail reads Y as [esi+4]);
// HasTC + the landed ternary GetTC with an uninitialised component pointer (the miss arm loads the dead key
// slot); the angle is atan2 in turns wrapped into [0,1) by fmod.
#include "engine/CPlayer.h"
#include <math.h>

struct CTCBase;
struct CBaseObject;

struct C3DVector { float X, Y, Z; };

struct CCamera {                                        // 0x38 bytes
    C3DVector Position;                                 // +0x00
    unsigned char _body[0x38 - 0x0c];
    void PointAt(const C3DVector& target);              // 0x00A0C2B0
    void SetAngleXY(float turns);                       // 0x00A0C570
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CTCGraphicAppearance {                           // interface 0x5B
    C3DVector GetHelperPointPosition();                 // 0x004C10E0
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    template <class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CPlayerRetailFreeCameraFlags {                   // retail view: the flag block 8 bytes below the PDB
    unsigned char _pad00[0x220];
    bool          UsingFreeCamera;                      // +0x220 (PDB +0x228)
    bool          FreeCameraTrackingPlayer;             // +0x221 (PDB +0x229)
    bool          FreeCameraTrackingPlayerYZ;           // +0x222 (PDB +0x22A)
};

struct CPlayer_Methods : CPlayer {
    CIntelligentPointer<CThing>& ControlledCreaturePointer() { return *reinterpret_cast<CIntelligentPointer<CThing>*>(ControlledCreature); }
    CPlayerRetailFreeCameraFlags& Flags() { return *reinterpret_cast<CPlayerRetailFreeCameraFlags*>(this); }
    CCamera& CurrentCamera() { return *reinterpret_cast<CCamera*>(CurrentFreeCamera); }
    CCamera& FreeCam() { return *reinterpret_cast<CCamera*>(FreeCamera); }
    CCamera& OldCam() { return *reinterpret_cast<CCamera*>(OldFreeCamera); }
    void UpdateFreeCamera();
};

void CPlayer_Methods::UpdateFreeCamera()
{
    OldCam() = FreeCam();
    if (Flags().UsingFreeCamera && ControlledCreaturePointer().GetPItem()) {
        CThing* creature = ControlledCreaturePointer().GetPItem();
        CTCGraphicAppearance* appearance;
        if (creature->HasTC(0x5b))
            appearance = creature->GetTC<CTCGraphicAppearance>(0x5b);
        if (Flags().FreeCameraTrackingPlayerYZ) {
            C3DVector target = appearance->GetHelperPointPosition();
            CurrentCamera().PointAt(target);
        } else if (Flags().FreeCameraTrackingPlayer) {
            C3DVector target = appearance->GetHelperPointPosition();
            const C3DVector& from = CurrentCamera().Position;
            float angle = (float)(atan2(target.X - from.X, target.Y - from.Y) * 0.15915494f);
            if (angle < 0.0f || angle >= 1.0f) {
                angle = (float)fmod((double)angle, 1.0);
                if (angle < 0.0f)
                    angle += 1.0f;
            }
            CurrentCamera().SetAngleXY(angle);
        }
    }
    FreeCam() = CurrentCamera();
}
