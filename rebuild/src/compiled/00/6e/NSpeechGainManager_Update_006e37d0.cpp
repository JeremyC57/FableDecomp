// NSpeechGainManager::Update(const CWorld& world) -- once per frame: mark the first active dialogue line as the layer head, advance every line's gain by the frames elapsed since the last update (the global def's dialogue layers), drop the finished lines (and their layer speaker count), then record the current world frame
// Leaf: ?Update@NSpeechGainManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E37D0, 184 bytes (+ int3 padding), `__fastcall` (ecx = world), plain `ret`.
// Identity: Ego_r.pdb name set `?Update@NSpeechGainManager@@YIXABVCWorld@@@Z`; only caller 0x004A5E82 (the world update).
//   The catalog's bare "Update" is right.
// Callees: the main game component's world-frame getter @ 0x00416225 (x2; manifest "CWorld::DrawGetEnvironment" is a
//   byte twin), NSpeechGainManager::UpdateDialogueElement(CActiveDialogueInfo&, long, const vector<CDialogueLayerDef>&)
//   @ 0x006E3440 (`__fastcall`, bool), CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (the line's
//   ThingSpeaking pointer, the inlined ~CActiveDialogueInfo), free @ 0x00BFEA14 (STLport node deallocation).
// Globals: g_pMainGameComponent @ 0x013B86A0; the active line list @ 0x013BABA0 (a static STLport list<CActiveDialogueInfo>,
//   0x20-byte nodes, landed initialiser 0x0121E3F0); the per-layer speaker counts @ 0x013BAB94 (static vector<short>); the
//   last update frame @ 0x013BAB8C.
// Layout: CWorld definition manager +0x10 (CGameDefinitionManager, global def +0xD0 as in the landed CTCTalk
//   SetAsUsingNarrator), CGlobalDef::DialogueLayers at retail +0x10C (PDB +0x118, the 12-byte STLport vectors before it);
//   CActiveDialogueInfo (0x18) at node +8: Layer +0x0C, IsLayerHead +0x10.
// Shape: `next` is a second iterator stepped before the body (`mov edi,[edi]`) and replaced by STLport's inline
//   `erase(it)` (unlink, destroy, free) when the element update reports the line finished; the head flag compares the
//   node with a re-read begin(); the layer-def vector reference is spilled to the `push ecx` frame word; the count
//   decrement goes through STLport `*(begin() + n)`.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator() { }
    _List_iterator(_List_node<T>* node) : _M_node(node) { }
    T& operator*() const { return _M_node->_M_data; }
    T* operator->() const { return &_M_node->_M_data; }
    _List_iterator& operator++() { _M_node = (_List_node<T>*)_M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& x) const { return _M_node == x._M_node; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;                                 // the sentinel node
    iterator begin() { return iterator((_List_node<T>*)_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    bool empty() const { return _M_node->_M_next == _M_node; }
    iterator erase(iterator position)
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        _List_node<T>* n = position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~T();
        free(n);
        return iterator((_List_node<T>*)next);
    }
};
}

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class CMainGameComponent {
public:
    long GetWorldFrame();                                   // 0x00416225
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

namespace NSpeechGainManager {

enum EDialogueLayer { DIALOGUE_LAYER_0 = 0, DIALOGUE_LAYER_1 = 1, DIALOGUE_LAYER_2 = 2, DIALOGUE_LAYER_NONE = 3 };

class CActiveDialogueInfo {                                 // 0x18 bytes
public:
    CBaseIntelligentPointer ThingSpeaking;                  // +0x00 CIntelligentPointer<CThing>
    unsigned long SoundID;                                  // +0x08
    EDialogueLayer Layer;                                   // +0x0C
    bool IsLayerHead;                                       // +0x10
    float OutputGain;                                       // +0x14
};

class CDialogueLayerDef;

}

class CGlobalDef {
public:
    unsigned char _pad00[0x10c];
    std::vector<NSpeechGainManager::CDialogueLayerDef> DialogueLayers;   // +0x10C
};

class CGameDefinitionManager {
public:
    unsigned char _pad00[0xd0];
    CGlobalDef* PGlobalDef;                                 // +0xD0
};

class CWorld {
public:
    unsigned char _pad00[0x10];
    CGameDefinitionManager* PDefinitionManager;             // +0x10
};

namespace NSpeechGainManager {

bool __fastcall UpdateDialogueElement(CActiveDialogueInfo& info, long elapsedFrames,
                                      const std::vector<CDialogueLayerDef>& layerDefs);   // 0x006E3440

extern std::list<CActiveDialogueInfo> g_ActiveDialogue;     // 0x013BABA0
extern std::vector<short> g_LayerSpeakerCounts;             // 0x013BAB94
extern long g_LastUpdateFrame;                              // 0x013BAB8C

void __fastcall Update(const CWorld& world)
{
    if (!g_ActiveDialogue.empty()) {
        long elapsedFrames = g_pMainGameComponent->GetWorldFrame() - g_LastUpdateFrame;
        const std::vector<CDialogueLayerDef>& layerDefs = world.PDefinitionManager->PGlobalDef->DialogueLayers;
        std::list<CActiveDialogueInfo>::iterator it = g_ActiveDialogue.begin();
        std::list<CActiveDialogueInfo>::iterator next = it;
        while (it != g_ActiveDialogue.end()) {
            ++next;
            it->IsLayerHead = it == g_ActiveDialogue.begin();
            if (!UpdateDialogueElement(*it, elapsedFrames, layerDefs)) {
                --g_LayerSpeakerCounts[it->Layer];
                next = g_ActiveDialogue.erase(it);
            }
            it = next;
        }
    }
    g_LastUpdateFrame = g_pMainGameComponent->GetWorldFrame();
}

}