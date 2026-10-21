#include <cstdio>
// Behaviour test for NUISystem::CManager::SetInput (0x0041DF10): the input type is stored and the key map is filled per type.
enum EInputType { EIT_KEYBOARD = 0, EIT_PAD = 1 };
enum EUsedKeys { EUK_0, EUK_1, EUK_2, EUK_3, EUK_4, EUK_5, EUK_6, EUK_7, EUK_8, EUK_9, EUK_10, EUK_11, EUK_12, EUK_13, EUK_14, EUK_15 };
struct CKeyMap {
    long m_keys[16];
    long& operator[](const EUsedKeys& key) { return m_keys[key]; }
};
struct CManager {
    CKeyMap    m_keyMap;
    EInputType m_inputType;
    void SetInput(EInputType type)
    {
        m_inputType = type;
        if (type == EIT_KEYBOARD) {
            m_keyMap[EUK_0] = 0x6f; m_keyMap[EUK_1] = 0x70; m_keyMap[EUK_3] = 0x6d; m_keyMap[EUK_2] = 0x72;
            m_keyMap[EUK_4] = 0x73; m_keyMap[EUK_5] = 0x6e; m_keyMap[EUK_6] = 0x1e; m_keyMap[EUK_7] = 0x30;
            m_keyMap[EUK_8] = 0x36; m_keyMap[EUK_9] = 0x67; m_keyMap[EUK_10] = 0x10; m_keyMap[EUK_11] = 0x11;
            m_keyMap[EUK_12] = 0x2c; m_keyMap[EUK_13] = 0x2d; m_keyMap[EUK_14] = 0x18; m_keyMap[EUK_15] = 0x26;
        } else if (type == EIT_PAD) {
            m_keyMap[EUK_0] = 0x0f; m_keyMap[EUK_1] = 0x10; m_keyMap[EUK_3] = 0x0d; m_keyMap[EUK_2] = 0x0e;
        }
    }
};
int main()
{
    CManager m;
    int i;
    for (i = 0; i < 16; i++) m.m_keyMap.m_keys[i] = -1;
    m.SetInput(EIT_KEYBOARD);
    if (m.m_inputType != EIT_KEYBOARD || m.m_keyMap.m_keys[0] != 0x6f || m.m_keyMap.m_keys[3] != 0x6d || m.m_keyMap.m_keys[15] != 0x26) {
        std::printf("0041df10_TEST FAIL\n"); return 1;
    }
    for (i = 0; i < 16; i++) m.m_keyMap.m_keys[i] = -1;
    m.SetInput(EIT_PAD);
    if (m.m_inputType != EIT_PAD || m.m_keyMap.m_keys[2] != 0x0e || m.m_keyMap.m_keys[4] != -1) {
        std::printf("0041df10_TEST FAIL\n"); return 1;
    }
    for (i = 0; i < 16; i++) m.m_keyMap.m_keys[i] = -1;
    m.SetInput((EInputType)2);
    if (m.m_inputType != 2 || m.m_keyMap.m_keys[0] != -1) { std::printf("0041df10_TEST FAIL\n"); return 1; }
    std::printf("0041df10_TEST PASS\n");
    return 0;
}