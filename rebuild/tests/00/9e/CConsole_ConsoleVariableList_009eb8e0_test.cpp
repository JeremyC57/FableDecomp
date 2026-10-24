// Behaviour test for 0x009EB8E0 CConsole::ConsoleVariableList: an in-order walk of the registered inputs that reports
// the name of each one whose type string is "variable".
#include <cstdio>
#include <cstring>
#include <string>

struct CConsoleInputBase
{
    const char* Type;
    std::string Name;
    std::string GetTypeAsString() const { return Type; }
    const std::string& GetName() const { return Name; }
};

struct SInputNode
{
    SInputNode* _M_parent;
    SInputNode* _M_left;
    SInputNode* _M_right;
    CConsoleInputBase* second;
};

struct CInputIterator
{
    SInputNode* _M_node;
    CInputIterator& operator++()
    {
        SInputNode* x = _M_node;
        if (x->_M_right) {
            x = x->_M_right;
            while (x->_M_left) x = x->_M_left;
        } else {
            SInputNode* y = x->_M_parent;
            while (x == y->_M_right) { x = y; y = y->_M_parent; }
            if (x->_M_right != y) x = y;
        }
        _M_node = x;
        return *this;
    }
};

struct CConsole
{
    SInputNode* Header;
    std::string Output;
    void SendMessageToUser(const std::string& m) { Output += m; Output += ";"; }
    static void ConsoleVariableList(CConsole& console)
    {
        CInputIterator it;
        it._M_node = console.Header->_M_left;
        for (; it._M_node != console.Header; ++it)
        {
            if (it._M_node->second->GetTypeAsString() == "variable")
                console.SendMessageToUser(it._M_node->second->GetName());
        }
    }
};

int main()
{
    // tree: b (root) with children a and d; d has left child c.  In order: a b c d.
    CConsoleInputBase ia = { "command", "alpha" }, ib = { "variable", "beta" }, ic = { "command", "gamma" },
                      id = { "command", "delta" };
    SInputNode header, a, b, c, d;
    b._M_parent = &header; b._M_left = &a; b._M_right = &d; b.second = &ib;
    a._M_parent = &b; a._M_left = 0; a._M_right = 0; a.second = &ia;
    d._M_parent = &b; d._M_left = &c; d._M_right = 0; d.second = &id;
    c._M_parent = &d; c._M_left = 0; c._M_right = 0; c.second = &ic;
    header._M_parent = &b; header._M_left = &a; header._M_right = &d; header.second = 0;

    CConsole console;
    console.Header = &header;
    CConsole::ConsoleVariableList(console);
    bool ok = console.Output == "beta;";

    header._M_left = &header; header._M_parent = 0; header._M_right = &header;   // empty map
    console.Output = "";
    CConsole::ConsoleVariableList(console);
    ok = ok && console.Output.empty();

    if (!ok) { std::printf("009eb8e0_TEST FAIL\n"); return 1; }
    std::printf("009eb8e0_TEST PASS\n");
    return 0;
}