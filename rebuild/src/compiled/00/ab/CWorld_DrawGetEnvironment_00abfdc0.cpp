class CIDrawEnvironment;

class CWorld { public:
	char pad0[0x10];
	CIDrawEnvironment* m_pDrawEnvironment;
	CIDrawEnvironment* DrawGetEnvironment();
};

CIDrawEnvironment* CWorld::DrawGetEnvironment()
{
	return this->m_pDrawEnvironment;
}