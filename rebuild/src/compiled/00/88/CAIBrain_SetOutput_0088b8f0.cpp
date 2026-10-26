// CAIBrain::SetOutput(const CBrainOutput& output) -- store the brain's output (CBrainOutput::operator=) and mark it valid
// Leaf: ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B8F0, 27 bytes, thiscall, `ret 4`.
// Identity: CAIBrain::SetOutput -- FINDINGS chunk 482 callee note; evidence: the body is `Output = output; OutputValid = true;`
//   over Ego_r.pdb CAIBrain (Output : CBrainOutput at +0x48, OutputValid at +0x90; retail +0x44 / +0x8C, the 20-byte
//   Dinkumware-era CVectorMap StateGroupList being 16 bytes in retail), the callee 0x0088C840 is the landed
//   CBrainOutput::operator=, the operand is 0x48 bytes (= CBrainOutput, 72 bytes in Ego_r), and every caller (0x0085F7E5,
//   0x00910659 in CAIStateGroup_MinionListenToStory::StandUp, 0x00917C9C, 0x0091D74C, ... ) is AI state-group code. FableWin /
//   Ego_r name set ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z. The catalog's CQuestManager::RemoveAvailableQuestCardType is
//   wrong (that member takes a CCharString and CQuestManager has no member at +0x44 / +0x8C of this shape).
// Model: engine/CAIBrain.h (retail STLport 4.6 layout: Output +0x44, OutputValid +0x8C); the method rides on a
//   derived class so the symbol keeps its retail name.
#define CAIBrain CAIBrainDonor
#include "engine/CAIBrain.h"
#undef CAIBrain
class CBrainOutput {                                     // retail 0x48 bytes (only the size and the assignment matter here)
public:
    unsigned char _body[0x48];
    CBrainOutput& operator=(const CBrainOutput& other);  // 0x0088C840
};

class CAIBrain : public CAIBrainDonor {
public:
    void SetOutput(const CBrainOutput& output);
};

void CAIBrain::SetOutput(const CBrainOutput& output)
{
    reinterpret_cast<CBrainOutput&>(Output) = output;
    OutputValid = true;
}
