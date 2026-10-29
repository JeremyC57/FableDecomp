// Behaviour test for NUISystem::CManager::GetCorrectComponentInterface @ 0x0041CD93: types 0..43 except 26, 28-31 and 37
// hand the component back; every other type (and anything past 43) gives null.
#include <stdio.h>
namespace NUISystem {
enum EType { ETYPE_LAST = 100 };
class CComponent { public:
    EType Type;
    virtual EType GetType() const { return Type; }
};
class CManager { public:
    CComponent* GetCorrectComponentInterface(CComponent* component);
};
CComponent* CManager::GetCorrectComponentInterface(CComponent* component)
{
    CComponent* result = 0;
    switch (component->GetType()) {
    case 0:
        result = component;
        break;
    case 1:
        result = component;
        break;
    case 2:
        result = component;
        break;
    case 3:
        result = component;
        break;
    case 4:
        result = component;
        break;
    case 5:
        result = component;
        break;
    case 6:
        result = component;
        break;
    case 7:
        result = component;
        break;
    case 8:
        result = component;
        break;
    case 9:
        result = component;
        break;
    case 10:
        result = component;
        break;
    case 11:
        result = component;
        break;
    case 12:
        result = component;
        break;
    case 13:
        result = component;
        break;
    case 14:
        result = component;
        break;
    case 15:
        result = component;
        break;
    case 16:
        result = component;
        break;
    case 17:
        result = component;
        break;
    case 18:
        result = component;
        break;
    case 19:
        result = component;
        break;
    case 20:
        result = component;
        break;
    case 21:
        result = component;
        break;
    case 22:
        result = component;
        break;
    case 23:
        result = component;
        break;
    case 24:
        result = component;
        break;
    case 25:
        result = component;
        break;
    case 27:
        result = component;
        break;
    case 32:
        result = component;
        break;
    case 33:
        result = component;
        break;
    case 34:
        result = component;
        break;
    case 35:
        result = component;
        break;
    case 36:
        result = component;
        break;
    case 38:
        result = component;
        break;
    case 39:
        result = component;
        break;
    case 40:
        result = component;
        break;
    case 41:
        result = component;
        break;
    case 42:
        result = component;
        break;
    case 43:
        result = component;
        break;
    }
    return result;
}
} // namespace NUISystem
int main()
{
    NUISystem::CManager manager;
    NUISystem::CComponent component;
    for (int t = 0; t < 60; ++t) {
        component.Type = (NUISystem::EType)t;
        bool expected = t <= 43 && t != 26 && !(t >= 28 && t <= 31) && t != 37;
        NUISystem::CComponent* got = manager.GetCorrectComponentInterface(&component);
        if ((got == &component) != expected || (!expected && got != 0)) {
            printf("0041cd93_TEST FAIL\n");
            printf("type %d\n", t);
            return 1;
        }
    }
    printf("0041cd93_TEST PASS\n");
    return 0;
}