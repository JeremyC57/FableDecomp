// CTCCreatureModeManager::CModeBase::~CModeBase() -- destructor of the creature-mode base: own vtable, then the weapon-based anim filter dies (its group string, then the filter base's vtable)
// Leaf: ??1CModeBase@CTCCreatureModeManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00739E20, 26 bytes (+ padding to 32), __fastcall `ret`, no return value.
// Identity (label correction): the catalog's "pair<unsigned_long,C3DMeshVolume>" is propagated. The body stores vtable
//   0x012673CC (RTTI CTCCreatureModeManager::CModeBase), destroys the CCharString at +0xC and stores 0x0122FFCC (RTTI
//   CAnimFilterBase) at +8: Ego_r.pdb CModeBase (ParentThing +4, WeaponBasedFilter CAnimFilterOfGroup +8 = vptr, Group
//   +0xC, GroupCRC +0x10; FramesBeforeRemovingSelf +0x14, Active +0x18). CModeBase's scalar deleting destructor 0x00739EE0
//   (vtable slot 0) inlines the same body; the PDB name sets have ??1CModeBase@CTCCreatureModeManager@@UAE@XZ. Nine
//   callers: the derived modes' destructors (0x0073EB73 .. 0x008E39DD).
// Callee: CCharString::~CCharString @ 0x0099EAE0.
// Model: as the landed ~CModeHoverBase 0x00743D40 (which inlines this destructor): CAnimFilterOfGroup's implicit
//   destructor does not re-store its own vtable; CAnimFilterBase's inline user destructor stores 0x0122FFCC.
// Shape: an empty user destructor.
class CThing;

class CCharString {
public:
    void* PStringData;                                      // +0x00
    ~CCharString();                                         // 0x0099EAE0
};

class CAnimFilterBase {                                     // vtable 0x0122FFCC
public:
    virtual void v00();
    ~CAnimFilterBase() { }
};

class CAnimFilterOfGroup : public CAnimFilterBase {         // 12 bytes; its own vtable is not re-stored
public:
    CCharString   Group;                                    // +0x04
    unsigned long GroupCRC;                                 // +0x08
};

class CTCCreatureModeManager {
public:
    class CModeBase {                                       // vtable 0x012673CC
    public:
        virtual ~CModeBase();
        CThing*            ParentThing;                     // +0x04
        CAnimFilterOfGroup WeaponBasedFilter;               // +0x08
        long               FramesBeforeRemovingSelf;        // +0x14
        bool               Active;                          // +0x18
    };
};

CTCCreatureModeManager::CModeBase::~CModeBase()
{
}