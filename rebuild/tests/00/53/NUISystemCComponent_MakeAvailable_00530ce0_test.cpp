#include <cstdio>
// Behaviour test: MakeAvailable walks a three-node STLport-shaped red-black tree in order and scales each state's R, G, B.
struct CRGBColour { unsigned char B, G, R, A; };
struct CUIState { unsigned int flag; int type; float pos[2]; float zoom[2]; CRGBColour Colour; float time; bool linear; };
struct SStateNode { bool _M_color; SStateNode* _M_parent; SStateNode* _M_left; SStateNode* _M_right; unsigned int first; CUIState second; };
struct CIter {
    SStateNode* _M_node;
    CIter(SStateNode* n) : _M_node(n) { }
    bool operator!=(const CIter& o) const { return _M_node != o._M_node; }
    SStateNode* operator->() const { return _M_node; }
    CIter& operator++()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0) _M_node = _M_node->_M_left;
        } else {
            SStateNode* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) { _M_node = y; y = y->_M_parent; }
            if (_M_node->_M_right != y) _M_node = y;
        }
        return *this;
    }
};
struct CStateMap { SStateNode* _M_header; CIter begin() { return CIter(_M_header->_M_left); } CIter end() { return CIter(_M_header); } };
static int g_getStates = 0;
struct CComponent {
    CStateMap states;
    CStateMap& GetStates() { ++g_getStates; return states; }
    void MakeAvailable()
    {
        for (CIter it = GetStates().begin(); it != GetStates().end(); ++it)
        {
            CRGBColour& colour = it->second.Colour;
            colour.R = (unsigned char)(colour.R * 1.33333337f);
            colour.G = (unsigned char)(colour.G * 1.33333337f);
            colour.B = (unsigned char)(colour.B * 1.33333337f);
        }
    }
};
static void Link(SStateNode& n, SStateNode* p, SStateNode* l, SStateNode* r) { n._M_parent = p; n._M_left = l; n._M_right = r; }
static void Set(SStateNode& n, unsigned char r, unsigned char g, unsigned char b)
{ n.second.Colour.R = r; n.second.Colour.G = g; n.second.Colour.B = b; n.second.Colour.A = 200; }
static int Check(const SStateNode& n, unsigned char r, unsigned char g, unsigned char b)
{ return n.second.Colour.R == (unsigned char)(r * 1.33333337f) && n.second.Colour.G == (unsigned char)(g * 1.33333337f) && n.second.Colour.B == (unsigned char)(b * 1.33333337f) && n.second.Colour.A == 200; }
int main()
{
    SStateNode header, root, left, right;
    Link(header, &root, &left, &right); Link(root, &header, &left, &right); Link(left, &root, 0, 0); Link(right, &root, 0, 0);
    Set(root, 120, 60, 30); Set(left, 100, 40, 8); Set(right, 90, 180, 3);
    Set(header, 1, 1, 1);
    CComponent c; c.states._M_header = &header;
    c.MakeAvailable();
    if (!Check(root, 120, 60, 30) || !Check(left, 100, 40, 8) || !Check(right, 90, 180, 3) || header.second.Colour.R != 1 || g_getStates != 5)
    { std::printf("00530ce0_TEST FAIL\n"); return 1; }
    std::printf("00530ce0_TEST PASS\n");
    return 0;
}