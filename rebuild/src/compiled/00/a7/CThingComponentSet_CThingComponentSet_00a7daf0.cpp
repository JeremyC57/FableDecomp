// Free function at 0xbfeb1c: cdecl, takes the freed pointer as its arg.
void operator delete[](void*);

struct CThingComponentSet {
    void* m_data;   // +0x00
    char  m_flag;   // +0x04
    ~CThingComponentSet();
};

CThingComponentSet::~CThingComponentSet()
{
    operator delete[](m_data);
    m_data = 0;
    m_flag = 0;
}