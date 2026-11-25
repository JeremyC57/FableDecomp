#include <stdio.h>
#include <string.h>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString
{
public:
	CCharString(const char* str, long len);
	char* m_data;
};

class CCombatSequence_BanditAttackFar
{
public:
	virtual CCharString GetName() const;
};

// Local stand-in for the engine ctor at 0x99ebf0.
static char g_captured[64];
static int  g_capturedLen = 0;
static const char* g_capturedPtr = 0;

CCharString::CCharString(const char* str, long len)
{
	g_capturedLen = len;
	g_capturedPtr = str;
	m_data = (char*)str;
}

int main()
{
	CCombatSequence_BanditAttackFar obj;

	CCharString r = obj.GetName();

	if (g_capturedLen == -1 &&
		RetailTextEquals(g_capturedPtr, "COMBAT_SEQUENCE_BANDIT_ATTACK_LONG") &&
		RetailTextEquals(r.m_data, "COMBAT_SEQUENCE_BANDIT_ATTACK_LONG"))
	{
		printf("GETNAME_BANDITFAR_OK\n");
		return 0;
	}
	printf("GETNAME_BANDITFAR_FAIL len=%d ptr=%p\n", g_capturedLen, g_capturedPtr);
	return 1;
}