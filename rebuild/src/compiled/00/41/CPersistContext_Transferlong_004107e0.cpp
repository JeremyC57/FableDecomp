// CPersistContext::Transfer<long> @ 004107e0
// __fastcall void Transfer(CPersistContext* self, char* name, long* value)
// Forwards to Transfer<signed_char> @ 0x410620 with a zero-inited stack temp.

class CPersistContext { public:
    // Callee: CPersistContext::Transfer<float>(const char*, float&, const float&) @ 0x00410620
    // (agentNF19: Ego_r/FableWin ??$Transfer@M@CPersistContext@@QAEXPBDAAMABM@Z; the body writes through CDataOutputStream 0x00993EE0, the float writer)
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    // Target forwarder
    void Transfer_long_(char* name, long* value);
};

void CPersistContext::Transfer_long_(char* name, long* value)
{
    long local = 0;
    this->Transfer<float>(name, *(float*)value, *(float*)&local);
}