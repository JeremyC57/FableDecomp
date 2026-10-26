#include <cstdio>

class CIDrawEnvironment;

class CWorld { public:
	char pad0[0xC];
	CIDrawEnvironment* m_pDrawEnvironment;
	CIDrawEnvironment* DrawGetEnvironment();
};

CIDrawEnvironment* CWorld::DrawGetEnvironment()
{
	return this->m_pDrawEnvironment;
}

int main()
{
	CWorld w;
	CIDrawEnvironment* marker = (CIDrawEnvironment*)0xDEADBEEF;
	w.m_pDrawEnvironment = marker;
	CIDrawEnvironment* got = w.DrawGetEnvironment();
	if (got == marker) {
		printf("ENV_GETTER_OK\n");
		return 0;
	}
	printf("FAIL\n");
	return 1;
}