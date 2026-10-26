#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_00729900(void* member);
void* __fastcall CTCCreatureExpression_CompositeScalarDeletingDestructor(void* expression, int, unsigned int flags)
{
    DestructMember_00729900(static_cast<unsigned char*>(expression) + 12);
    ((CTCBase*)(expression))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(expression);
    return expression;
}