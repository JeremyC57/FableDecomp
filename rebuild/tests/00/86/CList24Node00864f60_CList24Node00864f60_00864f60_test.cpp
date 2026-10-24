// Behaviour test for 0x00864F60: the STLport list default constructor leaves an empty self-linked sentinel.
#include <cstdio>
#include <cstdlib>

struct SList24Node
{
    SList24Node*  _M_next;
    SList24Node*  _M_prev;
    unsigned char _M_data[0x1C];
};

struct CList24Node00864f60
{
    SList24Node* _M_node;
    CList24Node00864f60();
};

CList24Node00864f60::CList24Node00864f60() : _M_node(0)
{
    SList24Node* node = (SList24Node*)std::malloc(sizeof(SList24Node));
    node->_M_next = node;
    node->_M_prev = node;
    _M_node = node;
}

int main()
{
    CList24Node00864f60 list;
    if (list._M_node == 0 || list._M_node->_M_next != list._M_node || list._M_node->_M_prev != list._M_node)
    {
        std::printf("00864f60_TEST FAIL\n");
        return 1;
    }
    std::free(list._M_node);
    std::printf("00864f60_TEST PASS\n");
    return 0;
}