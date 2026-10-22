// CPhysicsFacesSegmentBranch::GetNoFaces  @ 0x00a85fd0
// long __fastcall (this)
// Sums GetNoFaces() (virtual slot 2, vtable+8) of two child segment pointers.

struct CPhysicsFacesSegmentBase
{
	virtual void unk0();
	virtual void unk1();
	virtual long GetNoFaces();   // slot 2 -> [vtbl+8]
};

struct CPhysicsFacesSegmentBranch
{
	char pad[0x14];
	CPhysicsFacesSegmentBase* PositiveChild;  // +0x14
	CPhysicsFacesSegmentBase* NegativeChild;  // +0x18

	long GetNoFaces();
};

long CPhysicsFacesSegmentBranch::GetNoFaces()
{
	return this->PositiveChild->GetNoFaces() + this->NegativeChild->GetNoFaces();
}