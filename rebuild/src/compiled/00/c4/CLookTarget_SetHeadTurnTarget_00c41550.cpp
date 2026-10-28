// CLookTarget::SetHeadTurnTarget(long value) -- Store a changed parameter and raise its dirty bit.
// Leaf: ?SetHeadTurnTarget@CLookTarget@@QAEXJ@Z
// Identity: Own wrapper00c34180 virtual slot1, existing exact callee binding ?SetHeadTurnTarget@CLookTarget@@QAEXJ@Z. Head-turn role is unsupported libpk labeling; spelling retained only for binding consistency. All owner/function names descriptive, no original PDB recovery claimed.
// Layout: no active shared header. Actual vptr0, parameter404, dirty472. Untouched prefix and intervening members not modeled.
struct CLookTarget{public:virtual void UnusedSlot0(){}virtual bool Accepts(){return true;}unsigned char Unmodeled04[400];long Parameter;unsigned char UnmodeledGap[64];unsigned long DirtyFlags;void SetHeadTurnTarget(long);};
void CLookTarget::SetHeadTurnTarget(long value){if(Parameter!=value){Parameter=value;DirtyFlags|=0x100000;}}