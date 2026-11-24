// Leaf: ?Update@CInputProcessJumpingAndRolling@@UAEXAAVCProcessedInput@@@Z
#pragma optimize("s",on)

class CInputProcessJumpingAndRolling { public:
    char pad[0x48];
    char field_4c;
    virtual void Update(class CProcessedInput&);
};

class CProcessedInput;

extern "C" char __stdcall JR_Helper(int a, int b);

void CInputProcessJumpingAndRolling::Update(class CProcessedInput& in)
{
    CInputProcessJumpingAndRolling* process = this;
    char r = JR_Helper(0x3c, 2);
    if (!r)
        process->field_4c = r;
}
// Flatten route (flatten_pilot v6, NF335): CInputProcessJumpingAndRolling_Update -> CInputProcessJumpingAndRolling::Update, the RTTI-placed PDB member; class model: the row's local class model.
