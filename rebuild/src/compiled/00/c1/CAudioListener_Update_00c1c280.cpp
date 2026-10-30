// CAudioListener::Update(const C3DVector& position, const C3DVector& front, const C3DVector& top) -- under the audio lock, converts the three vectors to the audio library's axes (Y and Z exchanged) and hands the listener handle (+0xC) its position and its front/top orientation
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1C280, 171 bytes (`ret 0Ch`). Descriptive name (no PDB covers this library):
//   the manifest's "CPhysicalPrimitiveMesh_ComputeLighting" is propagated. Callers: the landed 0x00C10E50 and 0x00C10F40
//   (both `m_pOwner->Update(m_a, m_b, m_c)` on the object at +0x7BC after one of the three vectors changed).
// Callees: PKCProtectionInstance::PKCProtectionInstance / ~PKCProtectionInstance @ 0x00C0FFC0 / 0x00C0FFE0 over the lock @ 0x0143BAFC,
//   C3DVector::SwapYZ @ 0x00C1C070 (hidden-pointer POD result), the library's `__stdcall` SetListenerPosition
//   @ 0x00C338C0 (handle, &position) and SetListenerOrientation @ 0x00C33920 (handle, &front, &top).
// Shape: the converted vectors are declared first and ASSIGNED: each SwapYZ builds into one shared result temporary
//   (+0x30) that is copied into the named POD local (three dword moves); initialising the locals directly builds
//   straight into them (111 bytes). No EH frame (this TU is built without /EHsc).
class PKCCriticalSection;
extern PKCCriticalSection g_soundLock;       // 0x0143BAFC

class PKCProtectionInstance { public:                        // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target); // 0x00C0FFC0
    ~PKCProtectionInstance();                         // 0x00C0FFE0
};

class C3DVector { public:
    float x, y, z;
    C3DVector SwapYZ() const;               // 0x00C1C070
};

void __stdcall SetListenerPosition(void* handle, const C3DVector* position);                       // 0x00C338C0
void __stdcall SetListenerOrientation(void* handle, const C3DVector* front, const C3DVector* top); // 0x00C33920

struct CAudioListener {
    unsigned char _pad[0xc];
    void* m_hListener;                      // +0x0C
    void Update(const C3DVector& position, const C3DVector& front, const C3DVector& top);
};

void CAudioListener::Update(const C3DVector& position, const C3DVector& front, const C3DVector& top)
{
    PKCProtectionInstance lock(&g_soundLock);
    C3DVector pos, fwd, up;
    pos = position.SwapYZ();
    fwd = front.SwapYZ();
    up = top.SwapYZ();
    SetListenerPosition(m_hListener, &pos);
    SetListenerOrientation(m_hListener, &fwd, &up);
}