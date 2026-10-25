// dynamic initializer for CNavigatorExternalInterface::AgentLists @ 0x013D2834 -- allocates and self-links the sentinel node, registers the destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224320, oracle length 31 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0xc ; call malloc ; mov [eax],eax ; mov [eax+4],eax ; push offset thunk ; mov [0x13d2834],eax ; call _atexit ; add esp,8 ; ret
// Compiler-generated `??__E` initializer of the static member CNavigatorExternalInterface::AgentLists, an STLport
// list<vector<const CSteeringAgent*>*> (sentinel node of 0xc bytes, malloc @ 0x00BFEA0E) whose inline default constructor
// allocates the node and links it to itself, then registers the destructor thunk @ 0x0122AAA0 with _atexit @ 0x004012BC.
// Name: Ego_r.pdb 0x0101AED0 `dynamic initializer for 'CNavigatorExternalInterface::AgentLists'` (and the atexit destructor
// 0x0101E7F0); the list's users 0x006CE670 / 0x00A47D80 push and erase CTCCreatureNavigation::Agents pointers.
// Leaf: _$E1
extern "C" void* __cdecl malloc(unsigned int size);
class CSteeringAgent;
namespace std { template <class T> class allocator; template <class T, class A> class vector; }
typedef std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> > CSteeringAgentVector;

struct CAgentLists_Retail {                        // STLport list<vector<const CSteeringAgent*>*>: the sentinel node pointer
    struct node { node* next; node* prev; CSteeringAgentVector* value; };
    node* head;
    CAgentLists_Retail() { node* sentinel = (node*)malloc(sizeof(node)); sentinel->next = sentinel; sentinel->prev = sentinel; head = sentinel; }
    ~CAgentLists_Retail();
};

class CNavigatorExternalInterface {
public:
    static CAgentLists_Retail AgentLists;          // 0x013D2834
};

CAgentLists_Retail CNavigatorExternalInterface::AgentLists;
