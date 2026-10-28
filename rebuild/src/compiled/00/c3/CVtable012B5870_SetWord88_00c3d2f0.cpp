// CVtable_012B5870::SetWord88(long value) -- Store a changed parameter and raise its dirty bit.
// Leaf: ?SetWord88@CVtable_012B5870@@QAEXJ@Z
// Identity: Own global singleton wrapper00c335f0 loads receiver0143bb58; existing SetSign8C00c3d310 and GetPair8000c3d2d0 prove same table012b5870. All owner/function names descriptive, no original PDB recovery claimed.
// Model amendment: Actual table012b5870 first entry00c3cbb0 is deleting destructor invoking00c3cb30; plain destructor stores012b5870 then destroys four20-byte members and finally stores original base012b5868. Original virtual-object base and virtual destructor slot0 are modeled; inactive opaque field regions remain explicit projections.
// Layout: no active shared header. Actual vptr0, parameter136, dirty164. Untouched prefix and intervening members not modeled.
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868();};
struct CVtable_012B5870:public CUnprovenLibraryObject_012B5868{public:virtual ~CVtable_012B5870();unsigned char Unmodeled04[132];long Parameter;unsigned char UnmodeledGap[24];unsigned long DirtyFlags;void SetWord88(long);};
void CVtable_012B5870::SetWord88(long value){if(Parameter!=value){Parameter=value;DirtyFlags|=0x400000;}}