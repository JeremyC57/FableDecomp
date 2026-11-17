// Behaviour test for 0x006C9B70 std::__copy(map iterator range, back_insert_iterator<vector<pair<long, COpinionDeed*> > >):
// walk the red-black tree in order and push_back every (key, deed) pair, growing the vector through _M_insert_overflow.
// NF74 (round 7): on the shared STLport headers (no local std model); links the row's explicit instantiation, declared here
// without a body, over a hand-built five-node tree.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_BACK_INSERTER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

class COpinionDeed;

typedef std::pair<const long, COpinionDeed*> DeedMapValue;
typedef std::vector<std::pair<long, COpinionDeed*> > DeedVector;
typedef std::_Rb_tree_iterator<DeedMapValue, std::_Nonconst_traits<DeedMapValue> > DeedMapIterator;
namespace std { template<class I, class O, class D> O __fastcall __copy(I first, I last, O result, const input_iterator_tag&, D*); }

int main()
{
    typedef std::_Rb_tree_node<DeedMapValue> Node;
    std::_Rb_tree_node_base header;
    Node* n[5];
    for (int k = 0; k < 5; ++k) {
        n[k] = (Node*)malloc(sizeof(Node));
        *(long*)&n[k]->_M_value_field.first = 10 * (k + 1);
        n[k]->_M_value_field.second = (COpinionDeed*)(size_t)(0x100 + k);
        n[k]->_M_color = false;
        n[k]->_M_left = n[k]->_M_right = 0;
    }
    // tree: n2 root; n1 (left) with n0 left child; n3 (right) with n4 right child
    header._M_color = true; header._M_parent = n[2]; header._M_left = n[0]; header._M_right = n[4];
    n[2]->_M_parent = &header; n[2]->_M_left = n[1]; n[2]->_M_right = n[3];
    n[1]->_M_parent = n[2]; n[1]->_M_left = n[0];
    n[0]->_M_parent = n[1];
    n[3]->_M_parent = n[2]; n[3]->_M_right = n[4];
    n[4]->_M_parent = n[3];
    DeedVector v;
    std::back_insert_iterator<DeedVector> out(v);
    std::input_iterator_tag tag;
    std::__copy<DeedMapIterator, std::back_insert_iterator<DeedVector>, int>(DeedMapIterator(n[0]), DeedMapIterator((Node*)&header), out, tag, (int*)0);
    int ok = 1;
    if (v.size() != 5) ok = 0;
    for (int k = 0; k < 5 && ok; ++k)
        if (v.begin()[k].first != 10 * (k + 1) || v.begin()[k].second != (COpinionDeed*)(size_t)(0x100 + k)) ok = 0;
    printf(ok ? "006c9b70_TEST PASS\n" : "006c9b70_TEST FAIL\n");
    return ok ? 0 : 1;
}
