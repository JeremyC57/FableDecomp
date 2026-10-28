// CLibpkObject1D8::SetWord1A0(long value) -- Store a changed parameter and raise its dirty bit.
// Leaf: ?SetWord1A0@CLibpkObject1D8@@QAEXJ@Z
// Identity: Own wrapper00c34229 and call00c4ee75 on libpk source object; neighboring float setters share dirty472. Parameter role unknown. All owner/function names descriptive, no original PDB recovery claimed.
// Layout: no active shared header. Actual vptr0, parameter416, dirty472. Untouched prefix and intervening members not modeled.
class CLibpkObject1D8{public:virtual void UnusedSlot0(){}virtual bool Accepts(){return true;}unsigned char Unmodeled04[412];long Parameter;unsigned char UnmodeledGap[52];unsigned long DirtyFlags;void SetWord1A0(long);};
void CLibpkObject1D8::SetWord1A0(long value){if(Parameter!=value){Parameter=value;DirtyFlags|=0x400000;}}