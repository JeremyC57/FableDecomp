// CGameJoystickManager::Get() -- lazily created singleton: when the static counted pointer PClass is empty, new a CGameJoystickManager (CBase root, device number 0, "set" flag false) and Reset PClass to it; return the managed object
// Leaf: ?Get@CGameJoystickManager@@SIAAV1@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049BB90, 88 bytes, plain `ret` (static __fastcall with no arguments:
//   FableWin.pdb `?Get@CGameJoystickManager@@SIAAV1@XZ`, `static CGameJoystickManager& Get()`).
// Callees: 0x00BFEA1A operator new (0x0C bytes, null-tested), 0x0099A2F0 CBase::CBase, 0x0049BCE0
//   CCountedPointer<CGameJoystickManager>::Reset (landed, __fastcall on &PClass = 0x013B89A0, `mov ecx,offset`).
// Globals: 0x013B89A0 CGameJoystickManager::PClass (CCountedPointer {object +0, count block +4}; its dynamic
//   initializer/atexit destructor are named in the PDB). Vtable 0x01238B54 (CGameJoystickManager, rtti-vtables.tsv).
// Shape: `if (!PClass.Get()) PClass.Reset(new CGameJoystickManager); return *PClass;` -- the inline constructor
//   (`: m_MainPlayerJoystickDeviceNumber(0), m_MainPlayerJoystickDeviceNumberSet(false)`, as the landed out-of-line
//   constructor 0x0049BB60) is expanded into both new-arms (the null arm pushes `xor esi,esi`), and the object is
//   re-read from PClass after the Reset. Members from engine/CGameJoystickManager.h (POD snapshot; the polymorphic class
//   is modelled locally with the same offsets, like the landed constructor).
// Label correction: catalogued as `_global::Get`; the PDB name is CGameJoystickManager::Get.
struct CBase {
    virtual ~CBase();
    CBase();                                        // 0x0099A2F0
};

template <class T>
struct CCountedPointer {
    T*    m_pObject;                                // +0x00
    void* m_pCount;                                 // +0x04 count block {references, destroy, object}
    T* Get() const { return m_pObject; }
    T& operator*() const { return *m_pObject; }
    void Reset(T* object);                          // 0x0049BCE0
};

struct CGameJoystickManager : CBase {
    long m_MainPlayerJoystickDeviceNumber;          // +0x04
    bool m_MainPlayerJoystickDeviceNumberSet;       // +0x08
    CGameJoystickManager()
        : m_MainPlayerJoystickDeviceNumber(0), m_MainPlayerJoystickDeviceNumberSet(false)
    {
    }
    static CCountedPointer<CGameJoystickManager> PClass;   // 0x013B89A0
    static CGameJoystickManager& __fastcall Get();
};

CGameJoystickManager& __fastcall CGameJoystickManager::Get()
{
    if (!PClass.Get())
        PClass.Reset(new CGameJoystickManager);
    return *PClass;
}