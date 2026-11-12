class CPersistContext { public:
    void Transfer(int a, int b);
    // Callee: CPersistContext::Transfer<unsigned long>(const char*, unsigned long&, const unsigned long&) @ 0x004106F0
    // (agentNF19: Ego_r/FableWin ??$Transfer@K@CPersistContext@@QAEXPBDAAKABK@Z)
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
};

void CPersistContext::Transfer(int a, int b)
{
    long v = 0;
    Transfer<unsigned long>((const char*)a, *(unsigned long*)b, *(unsigned long*)&v);
}