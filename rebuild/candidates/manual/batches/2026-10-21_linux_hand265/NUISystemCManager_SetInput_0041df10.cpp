// NUISystem::CManager::SetInput(EInputType type) -- store the input type (+0x30) and fill the UI-key -> engine-key map (+0x24) for it: sixteen keyboard bindings for type 0, four pad bindings for type 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041DF10, length 509 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: the key map's operator[] @ 0x0042D201 (map<EUsedKeys, long>, const EUsedKeys& -> long&).
// Layout (retail NUISystem::CManager, see the landed IsKeyHeld 0x0041E10D): key map +0x24, input type +0x30; engine/CManager.h is the
//   Xbox/PDB view (map at +0x34, InputType at +0x44), so the class is modelled locally.
// Shape: every binding is `m_keyMap[EUK_x] = engineKey;` -- the enum constant becomes a const-ref temporary that VC7.1 homes in the dead
//   `type` argument slot ([ebp+8]); type 0 is known in eax, hence `and [ebp+8],eax` for the first key.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem {
enum EInputType { EIT_KEYBOARD = 0, EIT_PAD = 1 };
struct CManager {
    enum EUsedKeys {
        EUK_0, EUK_1, EUK_2, EUK_3, EUK_4, EUK_5, EUK_6, EUK_7,
        EUK_8, EUK_9, EUK_10, EUK_11, EUK_12, EUK_13, EUK_14, EUK_15
    };
    struct CKeyMap {
        long& operator[](const EUsedKeys& key);    // 0x0042D201
        unsigned char _d[8];
    };
    unsigned char  _pad00[0x24];
    CKeyMap        m_keyMap;                       // +0x24
    unsigned char  _pad2c[4];
    EInputType     m_inputType;                    // +0x30
    void SetInput(EInputType type);
};

void CManager::SetInput(EInputType type)
{
    m_inputType = type;
    if (type == EIT_KEYBOARD) {
        m_keyMap[EUK_0] = 0x6f;
        m_keyMap[EUK_1] = 0x70;
        m_keyMap[EUK_3] = 0x6d;
        m_keyMap[EUK_2] = 0x72;
        m_keyMap[EUK_4] = 0x73;
        m_keyMap[EUK_5] = 0x6e;
        m_keyMap[EUK_6] = 0x1e;
        m_keyMap[EUK_7] = 0x30;
        m_keyMap[EUK_8] = 0x36;
        m_keyMap[EUK_9] = 0x67;
        m_keyMap[EUK_10] = 0x10;
        m_keyMap[EUK_11] = 0x11;
        m_keyMap[EUK_12] = 0x2c;
        m_keyMap[EUK_13] = 0x2d;
        m_keyMap[EUK_14] = 0x18;
        m_keyMap[EUK_15] = 0x26;
    } else if (type == EIT_PAD) {
        m_keyMap[EUK_0] = 0x0f;
        m_keyMap[EUK_1] = 0x10;
        m_keyMap[EUK_3] = 0x0d;
        m_keyMap[EUK_2] = 0x0e;
    }
}
}
