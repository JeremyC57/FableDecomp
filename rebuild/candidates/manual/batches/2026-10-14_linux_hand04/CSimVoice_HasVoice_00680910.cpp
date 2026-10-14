// CSimVoice::HasVoice(EReactionSpeechType type) -- is there an entry for the speech type in the Voices map?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00680910, oracle length 51 bytes.
// Callees: CVectorMap<EReactionSpeechType,long>::LowerBound(const key&) @ 0x006804C0 (manifest label carries a
// propagated template argument list; the sorted-vector lower bound returning an element pointer).
// Identity: the manifest label "C3DMeshStats::HasDummyObject" is propagated and rejected. The neighbours
// (0x006808C0 CSimVoice::TransferBinaryOut, 0x00680A40 CSimVoice::Add(EReactionSpeechType, ...)) and the
// PDB layout (struct_layouts_egor.tsv: CSimVoice +8 Voices = CVectorMap<EReactionSpeechType,long>) fix
// the class; the signed `jge` is the enum key compare.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; lea eax,[esp+0Ch] ; lea esi,[edi+8] ; push eax ; mov ecx,esi ; call LowerBound ;
//                mov esi,[esi+4] ; cmp eax,esi ; je E ; mov ecx,[esp+0Ch] ; cmp ecx,[eax] ; jge K ; E: mov eax,esi ; K: mov ecx,[edi+0Ch] ;
//                xor edx,edx ; cmp eax,ecx ; setne dl ; pop edi ; mov al,dl ; pop esi ; ret 4
// Shape: the map's Find() is inlined -- lower bound, then "not found" when the bound is end() or the key
// sorts before the element's key -- and the result is compared with end() (re-read from the map, the
// retail vector's `last` pointer at +4) to give the bool.
enum EReactionSpeechType { REACTION_SPEECH_NONE = 0 };

struct SVoiceEntry {                       // pair<EReactionSpeechType, long>
    EReactionSpeechType first;
    long                second;
};

struct CVoiceMap {                         // retail CVectorMap<EReactionSpeechType,long>: sorted vector {first, last, end} + compare + dirty
    SVoiceEntry* m_pFirst;
    SVoiceEntry* m_pLast;
    SVoiceEntry* m_pEnd;
    bool         m_compare;
    bool         m_dirty;
    SVoiceEntry* LowerBound(const EReactionSpeechType& key);   // 0x006804C0
    SVoiceEntry* End() { return m_pLast; }
    SVoiceEntry* Find(const EReactionSpeechType& key)
    {
        SVoiceEntry* it = LowerBound(key);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
};

struct CSimVoice {
    virtual void v00();
    long      m_reserved;                  // +0x4
    CVoiceMap Voices;                      // +0x8
    bool HasVoice(EReactionSpeechType type);
};

bool CSimVoice::HasVoice(EReactionSpeechType type)
{
    return Voices.Find(type) != Voices.End();
}
