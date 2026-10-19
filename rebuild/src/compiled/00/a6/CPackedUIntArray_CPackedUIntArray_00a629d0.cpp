// CPackedUIntArray destructor @ 00a629d0
// Layout: [+0] unsigned int* buffer, [+4] u32, [+8] bool, [+0xc] u32

void operator delete[](void*);

struct CPackedUIntArray {
    unsigned int* m_buffer;   // +0
    unsigned int  m_count;    // +4
    bool          m_flag;     // +8
    unsigned int  m_capacity; // +0xc
    ~CPackedUIntArray();
};

CPackedUIntArray::~CPackedUIntArray()
{
    operator delete[](m_buffer);
    m_buffer = 0;
    m_count = 0;
    m_flag = false;
    m_capacity = 0;
}