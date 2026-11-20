#include <cstdio>

class CSoundPlayParameters
{ public:
    unsigned char pad0;
    unsigned char pad1;
    unsigned char flags;
    bool GetStreamed() const;
};

bool CSoundPlayParameters::GetStreamed() const
{
    return (flags >> 1) & 1;
}

int main()
{
	CSoundPlayParameters o;
	int fails = 0;
	for (int v = 0; v < 256; ++v)
	{
		o.flags = (unsigned char)v;
		bool expect = ((v >> 1) & 1) != 0;
		if (o.GetStreamed() != expect)
			++fails;
	}
	if (fails == 0)
		printf("CANCOLLIDE_OK\n");
	else
		printf("CANCOLLIDE_FAIL %d\n", fails);
	return fails == 0 ? 0 : 1;
}
