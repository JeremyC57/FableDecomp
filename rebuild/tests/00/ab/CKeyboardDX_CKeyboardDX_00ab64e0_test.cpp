// Behaviour test for CKeyboardDX::CKeyboardDX (0x00AB64E0): device set-up chain and the key table.
#include <cstdio>
#include <cstring>
struct PropHeader { unsigned int dwSize, dwHeaderSize, dwObj, dwHow; };
struct PropDword { PropHeader diph; unsigned int dwData; };
struct FakeDevice {
    long createResult, formatResult, propResult, acquireResult;
    unsigned long bufferSize; int acquires;
};
struct Keyboard {
    FakeDevice* DKeyboard; unsigned char KeyStates[0x100]; unsigned short InputKeyToDXKey[0x7a]; bool Acquired;
    bool exclusiveOk;
    bool SetExclusiveMode(bool) { return exclusiveOk; }
    void Construct(FakeDevice* dev, bool exclusive)
    {
        std::memset(KeyStates, 0, sizeof(KeyStates));
        DKeyboard = dev;
        if (dev->createResult == 0 && SetExclusiveMode(exclusive) && DKeyboard->formatResult == 0) {
            PropDword prop;
            prop.diph.dwSize = sizeof(PropDword); prop.diph.dwHeaderSize = sizeof(PropHeader);
            prop.diph.dwObj = 0; prop.diph.dwHow = 0; prop.dwData = 0x100;
            DKeyboard->bufferSize = prop.dwData;
            if (DKeyboard->propResult == 0) { DKeyboard->acquires++; if (DKeyboard->acquireResult == 0) Acquired = true; }
        }
        static const unsigned short tail[] = { 0x57, 0x58, 0x64, 0x65, 0x66, 0x70, 0x79, 0x7b, 0x7d, 0x8d, 0x90, 0x91, 0x92, 0x93,
            0x94, 0x95, 0x96, 0x97, 0x9c, 0x9d, 0xb3, 0xb5, 0xb7, 0xb8, 0xc7, 0xc8, 0xc9, 0xcb, 0xcd, 0xcf, 0xd0, 0xd1, 0xd2,
            0xd3, 0xdb, 0xdc, 0xdd, 0xc5 };
        for (int i = 0; i < 84; ++i) InputKeyToDXKey[i] = (unsigned short)i;
        for (int j = 0; j < 38; ++j) InputKeyToDXKey[84 + j] = tail[j];
    }
};
int main()
{
    bool ok = true;
    FakeDevice d = { 0, 0, 0, 0, 0, 0 };
    Keyboard k; k.Acquired = false; k.exclusiveOk = true; k.KeyStates[5] = 9;
    k.Construct(&d, true);
    ok = ok && k.Acquired && d.bufferSize == 0x100 && k.KeyStates[5] == 0 && sizeof(PropDword) == 0x14;
    ok = ok && k.InputKeyToDXKey[0] == 0 && k.InputKeyToDXKey[83] == 0x53 && k.InputKeyToDXKey[84] == 0x57 && k.InputKeyToDXKey[121] == 0xc5;
    FakeDevice d2 = { 0, 0, 0, 0, 0, 0 };
    Keyboard k2; k2.Acquired = false; k2.exclusiveOk = false;
    k2.Construct(&d2, false);
    ok = ok && !k2.Acquired && d2.acquires == 0 && k2.InputKeyToDXKey[16] == 0x10;
    FakeDevice d3 = { 0, 0, 0, 1, 0, 0 };
    Keyboard k3; k3.Acquired = false; k3.exclusiveOk = true;
    k3.Construct(&d3, true);
    ok = ok && !k3.Acquired && d3.acquires == 1;
    if (!ok) { std::printf("00ab64e0_TEST FAIL\n"); return 1; }
    std::printf("00ab64e0_TEST PASS\n");
    return 0;
}