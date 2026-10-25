// NSpeechGainManager::OnDialogueStart(const CWorld& world, CThing& thing, unsigned long soundID, EDialogueLayer layer) -- register a line a live thing has started playing: insert it into the active dialogue list ahead of the first line on a later layer (or, on its own layer, ahead of a live speaker farther from the hero by more than the layer's focus-shift threshold), count it on its layer and set its sound's volume to its target gain
// Grade: DIFFER(596v596) -- x87 term order only: retail sums the stored existing offset's squares y, z, x (`fld [esp+24h]` first), ours z, y, x; every other byte matches. 4 grades: plain inline operators, __forceinline operator-/GetMagnitude, + STLport list iterator copy ctor and a named gain local (fixed the shrink-wrapped pushes and the gain push), + reordered magnitude terms (no change: VC7.1 canonicalises the sum)
// Leaf: ?OnDialogueStart@NSpeechGainManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E31E0, 596 bytes (+ int3 padding to 608), `__fastcall` (ecx = world, edx =
//   thing), `ret 8`; always returns false (`xor al,al` on the one exit).
// Identity: FableWin.pdb ?OnDialogueStart@NSpeechGainManager@@YI_NABVCWorld@@AAVCThing@@KW4EDialogueLayer@1@@Z. The catalog's
//   "OnDialogueStart" is the PDB name.
// Callees: the interface map's LowerBound 0x0040F020 (the thing's sound player, TC 0x10),
//   CTCSoundPlayer::IsAttachedSoundPlaying 0x0066AE10, CBaseIntelligentPointer::GetPItem 0x00A01B50, CWorld::GetMainPlayer 0x0049D9D0, CPlayer::GetHeroCreature 0x00487DC0,
//   CThing::GetPosition 0x004C73D0, CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / ~ 0x00A01C10 (the
//   temporary CActiveDialogueInfo; CIntelligentPointer<CThing> vtable 0x01238C6C), list::_M_create_node 0x006E4CB0
//   (landed), the landed GetTargetGain 0x006E2B70, CTCSoundPlayer::SetSpecificSampleVolume 0x0066AFF0.
// Globals: the active line list 0x013BABA0 (static STLport list<CActiveDialogueInfo>), the per-layer speaker counts
//   0x013BAB94 (static vector<short>).
// Layout: as the landed NSpeechGainManager::Update 0x006E37D0: CWorld definition manager +0x10, global def +0xD0,
//   CGlobalDef::DialogueLayers retail +0x10C; CDialogueLayerDef (0x40) FocusShiftDistanceThreshold +0x3C.
// Shape: the sound player is assigned only when the thing has the component (else the frame word is read uninitialised);
//   the insertion point is a list iterator kept in ebp, the end re-read per step; STLport insert(it, x) with the
//   out-of-line _M_create_node; the new offset's magnitude stays on the x87 stack against the stored existing offset.
#include <math.h>
#pragma intrinsic(sqrt)

class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];                       // +0x20
    CInterfaceMap Components;                               // +0x44
    unsigned char _pad4c[0x91 - 0x4C];
    unsigned char StateBits;                                // +0x91 Dead 0x01
    bool IsDead() const { return (StateBits & 0x01) != 0; }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PItem;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CThing* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CThing* item);                            // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    T* operator->() const { return (T*)GetPItem(); }
};

class CPlayer {
public:
    CThing* GetHeroCreature();                              // 0x00487DC0
};

class CTCSoundPlayer {
public:
    bool IsAttachedSoundPlaying(unsigned long soundID) const;   // 0x0066AE10
    void SetSpecificSampleVolume(unsigned long sampleID, float volume);   // 0x0066AFF0
};

namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    const T* begin() const { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
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
    _List_iterator(_List_node<T>* node) : _M_node(node) { }
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) { }
    T& operator*() const { return _M_node->_M_data; }
    T* operator->() const { return &_M_node->_M_data; }
    _List_iterator& operator++() { _M_node = (_List_node<T>*)_M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;                                 // the sentinel node
    iterator begin() { return iterator((_List_node<T>*)_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    _List_node<T>* _M_create_node(const T& x);              // 0x006E4CB0
    iterator insert(iterator position, const T& x)
    {
        _List_node<T>* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
};
}

namespace NSpeechGainManager {

enum EDialogueLayer { DIALOGUE_LAYER_0 = 0, DIALOGUE_LAYER_1 = 1, DIALOGUE_LAYER_2 = 2, DIALOGUE_LAYER_NONE = 3 };

class CActiveDialogueInfo {                                 // 0x18 bytes
public:
    CIntelligentPointer<CThing> ThingSpeaking;              // +0x00
    unsigned long SoundID;                                  // +0x08
    EDialogueLayer Layer;                                   // +0x0C
    bool IsLayerHead;                                       // +0x10
    float OutputGain;                                       // +0x14
    CActiveDialogueInfo(CThing& thing, unsigned long soundID, EDialogueLayer layer, bool isLayerHead)
        : ThingSpeaking(&thing), SoundID(soundID), Layer(layer), IsLayerHead(isLayerHead), OutputGain(0.0f) { }
};

class CDialogueLayerDef {                                   // 0x40 bytes
public:
    unsigned char _base[0x3C];
    float FocusShiftDistanceThreshold;                      // +0x3C
};

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
    CPlayer* GetMainPlayer() const;                         // 0x0049D9D0
};

enum { TC_SOUND_PLAYER = 0x10 };

namespace NSpeechGainManager {

extern std::list<CActiveDialogueInfo> g_ActiveDialogue;     // 0x013BABA0
extern std::vector<short> g_LayerSpeakerCounts;             // 0x013BAB94

float __fastcall GetTargetGain(const CActiveDialogueInfo& info, const std::vector<CDialogueLayerDef>& layerDefs);   // 0x006E2B70

bool __fastcall OnDialogueStart(const CWorld& world, CThing& thing, unsigned long soundID, EDialogueLayer layer)
{
    if (layer < DIALOGUE_LAYER_NONE && &thing && !thing.IsDead()) {
        CTCSoundPlayer* player;
        if (thing.HasTC(TC_SOUND_PLAYER))
            player = thing.GetTC<CTCSoundPlayer>(TC_SOUND_PLAYER);
        if (player->IsAttachedSoundPlaying(soundID)) {
            const std::vector<CDialogueLayerDef>& layerDefs = world.PDefinitionManager->PGlobalDef->DialogueLayers;
            std::list<CActiveDialogueInfo>::iterator it = g_ActiveDialogue.begin();
            for (; it != g_ActiveDialogue.end(); ++it) {
                if (layer == it->Layer && it->ThingSpeaking.GetPItem() && !it->ThingSpeaking->IsDead()) {
                    CThing* hero = world.GetMainPlayer()->GetHeroCreature();
                    if (hero && !hero->IsDead()) {
                        C3DVector existingOffset = it->ThingSpeaking->GetPosition() - hero->GetPosition();
                        C3DVector newOffset = thing.GetPosition() - hero->GetPosition();
                        if (newOffset.GetMagnitude() < existingOffset.GetMagnitude() - layerDefs[layer].FocusShiftDistanceThreshold)
                            break;
                    }
                }
                else if (layer < it->Layer)
                    break;
            }
            std::list<CActiveDialogueInfo>::iterator inserted =
                g_ActiveDialogue.insert(it, CActiveDialogueInfo(thing, soundID, layer, true));
            ++g_LayerSpeakerCounts[layer];
            float gain = GetTargetGain(*inserted, layerDefs);
            inserted->OutputGain = gain;
            player->SetSpecificSampleVolume(soundID, gain);
        }
    }
    return false;
}

}
