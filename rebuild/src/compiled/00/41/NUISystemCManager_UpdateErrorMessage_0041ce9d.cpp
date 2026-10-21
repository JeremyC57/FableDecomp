// NUISystem::CManager::UpdateErrorMessage(float) -- advances the on-screen error message by the frame time
// Leaf: UpdateErrorMessage@CManager
// Retail: Fable.exe @ 0x0041CE9D, 22 bytes, `ret 4`; Ego_r.pdb ?UpdateErrorMessage@CManager@NUISystem@@QAEXM@Z. Calls slot 1 (+0x04)
// of the error-message component held at +0xa0 with the time. Caller 0x0041D064.
// Shape: size TU (retail calls then `ret 4`; /O2 turns it into a tail jump).
#pragma optimize("s", on)
namespace NUISystem {
class CErrorMessage {
public:
    virtual ~CErrorMessage();                            // slot 0
    virtual void Update(float time);                     // slot 1 (+0x04)
};
class CManager {
public:
    unsigned char  _pad00[0xa0];
    CErrorMessage* ErrorMessage;                         // +0xa0
    void UpdateErrorMessage(float time);
};

void CManager::UpdateErrorMessage(float time)
{
    ErrorMessage->Update(time);
}
}