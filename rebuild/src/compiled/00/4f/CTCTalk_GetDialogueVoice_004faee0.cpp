// CTCTalk::GetDialogueVoice() const -- return this talker's reference-counted dialogue voice.
// Retail: Fable.exe 0x004FAEE0, 22 bytes. PDB places DialogueVoice at +0x38; the vtable slot
// and caller identify CTCTalk. The shared CTCTalk view is local because no generated header exists.
#include "fable_string.h"
// The shared header predates the retail user copy constructor. This ABI view adds that
// real copy operation without changing the one-pointer CCharString lifetime/layout.
struct CRetailCharString : CCharString {
    CRetailCharString(const CCharString& other); // native CCharString copy ctor @ 0x0099EC30
};
struct CTCTalk {
    unsigned char _beforeDialogueVoice[0x38];
    CCharString DialogueVoice;
    CRetailCharString GetDialogueVoice() const;
};
CRetailCharString CTCTalk::GetDialogueVoice() const { return DialogueVoice; }