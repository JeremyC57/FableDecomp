// Behaviour test for CTCInGameMenu::IsAnySliderReceivingInput (0x0047D483): true only when some component's
// "UI_OPTIONS_LIST" or "UI_AUDIO_OPTIONS_LIST" child is in input state 5 or 3.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; CCharString(const char* text, long) : Text(text) {} };
struct CComponent {
    const char* Name; long State; CComponent* Children[2]; int nChildren;
    CComponent* FindChild(CCharString name)
    { for (int i = 0; i < nChildren; ++i) if (std::strcmp(Children[i]->Name, name.Text) == 0) return Children[i]; return 0; }
    long GetInputState() { return State; }
};
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CTCInGameMenu { public:
    CRetailVector<CComponent*> Components;
    bool IsAnySliderReceivingInput() const;
};
bool CTCInGameMenu::IsAnySliderReceivingInput() const
{
    for (unsigned long i = 0; i < Components.size(); ++i) {
        CComponent* videoList = Components[i]->FindChild(CCharString("UI_OPTIONS_LIST", -1));
        if (videoList && (videoList->GetInputState() == 5 || videoList->GetInputState() == 3))
            return true;
        CComponent* audioList = Components[i]->FindChild(CCharString("UI_AUDIO_OPTIONS_LIST", -1));
        if (audioList && (audioList->GetInputState() == 5 || audioList->GetInputState() == 3))
            return true;
    }
    return false;
}
int main()
{
    CComponent video = { "UI_OPTIONS_LIST", 1, {0, 0}, 0 };
    CComponent audio = { "UI_AUDIO_OPTIONS_LIST", 2, {0, 0}, 0 };
    CComponent empty = { "X", 0, {0, 0}, 0 };
    CComponent screen = { "S", 0, {&video, &audio}, 2 };
    CComponent* list[2] = { &empty, &screen };
    CTCInGameMenu menu; menu.Components._M_start = list; menu.Components._M_finish = menu.Components._M_end_of_storage = list + 2;
    if (menu.IsAnySliderReceivingInput()) { std::printf("0047d483_TEST FAIL\n"); return 1; }
    audio.State = 3;
    if (!menu.IsAnySliderReceivingInput()) { std::printf("0047d483_TEST FAIL\n"); return 1; }
    audio.State = 4; video.State = 5;
    if (!menu.IsAnySliderReceivingInput()) { std::printf("0047d483_TEST FAIL\n"); return 1; }
    menu.Components._M_finish = list + 1;
    if (menu.IsAnySliderReceivingInput()) { std::printf("0047d483_TEST FAIL\n"); return 1; }
    std::printf("0047d483_TEST PASS\n");
    return 0;
}