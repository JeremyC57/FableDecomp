// CAIStateGroupBase::GetWorld() const -- the world the state group's brain lives in (PBrain->ThingManager->World)
// Leaf: ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FCFC0, 10 bytes: mov eax,[ecx+4] ; mov ecx,[eax+0x1c] ; mov eax,[ecx+0x24] ; ret.
// Identity (proven, chunk c667): Ego_r.pdb + FableWin.pdb (protected const, CWorld& result); three landed rows already bind it
//   so; called on a CAIStateGroup `this` (0x0091D8F7) whose result's +0x4C is CWorld::PThingCombatManager.
// Layout: Ego_r.pdb CAIStateGroupBase PBrain +0x04, CAIBrain ThingManager +0x1C; the thing manager's world pointer is the retail
//   +0x24 word (Ego_r CThingManager pads it: descriptive member name).
// Naming wave 16: landed as the descriptive CAIStateGroupBase::GetPWorld() (void* result) over CFirst / CSecond views;
//   respelled on the PDB member (name-only: the bytes are unchanged).
class CWorld;
class CThingManager {
public:
    unsigned char _pad_0x00[0x24];
    CWorld* World;                                      // +0x24 (retail; descriptive)
};
class CAIBrain {
public:
    unsigned char _pad_0x00[0x1c];
    CThingManager* ThingManager;                        // +0x1C
};
class CAIStateGroupBase {
protected:
    CWorld& GetWorld() const;
public:
    unsigned char _pad_0x00[0x4];
    CAIBrain* PBrain;                                   // +0x04
};

CWorld& CAIStateGroupBase::GetWorld() const
{
    return *PBrain->ThingManager->World;
}
