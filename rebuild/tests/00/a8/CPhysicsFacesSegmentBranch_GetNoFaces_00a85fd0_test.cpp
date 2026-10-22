#include <cstdio>

struct CPhysicsFacesSegmentBase
{
	virtual void unk0() {}
	virtual void unk1() {}
	virtual long GetNoFaces() { return 0; }
};

struct CPhysicsFacesSegmentBranch
{
	char pad[0x14];
	CPhysicsFacesSegmentBase* PositiveChild;
	CPhysicsFacesSegmentBase* NegativeChild;
	long GetNoFaces();
};

long CPhysicsFacesSegmentBranch::GetNoFaces()
{
	return this->PositiveChild->GetNoFaces() + this->NegativeChild->GetNoFaces();
}

struct Leaf : CPhysicsFacesSegmentBase
{
	long n;
	Leaf(long v) : n(v) {}
	virtual long GetNoFaces() { return n; }
};

int main()
{
	Leaf a(17), b(25);
	CPhysicsFacesSegmentBranch br;
	br.PositiveChild = &a;
	br.NegativeChild = &b;
	long r = br.GetNoFaces();
	if (r == 42)
		printf("GETNOFACES_OK\n");
	else
		printf("FAIL %ld\n", r);
	return r == 42 ? 0 : 1;
}