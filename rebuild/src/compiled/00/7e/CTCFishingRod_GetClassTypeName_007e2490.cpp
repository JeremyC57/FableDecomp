// CTCFishingRod::GetClassTypeName  @ 007e2490
// Retail shape: struct-return factory.
//   push esi ; esi = hidden return-buffer ptr [esp+8]
//   push -1 ; push offset "..." ; ecx = esi ; call CWideString::ctor
//   eax = esi ; ret 4
// GetActionName ignores its own 'this' and returns CWideString(literal, -1).

class CWideString
{
public:
    CWideString(const char* s, int len);
    void* m_data;
};

class CTCFishingRod
{
public:
    CWideString GetClassTypeName();
    void* vtbl;
};

CWideString CTCFishingRod::GetClassTypeName()
{
    return CWideString("DoCreatureAction", -1);
}