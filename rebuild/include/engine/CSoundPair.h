#ifndef FABLE_CSOUNDPAIR_H
#define FABLE_CSOUNDPAIR_H
class CSoundPair {
public:
 long FirstSound,LastSound;
#ifdef FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
 // FableWin019f1f60/018c2550/02587c30 and both PDB type records.
 CSoundPair() { }
 explicit CSoundPair(long sound) { FirstSound=sound; LastSound=sound; }
 CSoundPair(long first,long last) { FirstSound=first; LastSound=last; }
#else
 CSoundPair():FirstSound(0),LastSound(0) { }
 explicit CSoundPair(long sound);
 CSoundPair(long first,long last);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CSoundPair)==8);
#endif
