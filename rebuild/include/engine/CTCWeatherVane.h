#ifndef FABLE_ENGINE_CTCWEATHERVANE_H
#define FABLE_ENGINE_CTCWEATHERVANE_H
#ifdef FABLE_RETAIL_CTCWEATHERVANE_PASS30
#include "engine/CTCBase.h"
// Ego_r layout: the component is 16 bytes, with InitialOffsetXY at +12.
// The original virtual entry is also named by FableWin and occupies the CTCBase activation slot.
class CTCWeatherVane : public CTCBase {
public:
    virtual void OnInitialActivate();
private:
    float InitialOffsetXY;
};
FABLE_STATIC_ASSERT_AT(12, sizeof(CTCWeatherVane) == 16);
#endif
#endif
