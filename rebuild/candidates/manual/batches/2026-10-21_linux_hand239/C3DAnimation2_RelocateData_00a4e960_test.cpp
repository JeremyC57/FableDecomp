#include <cstdio>
// Behaviour copy of C3DAnimation2::RelocateData (0x00A4E960): rebase owned pointers by (new - old), keep nulls.
template <class T> inline void RelocatePointer(T*& p, void* o, void* n) { if (p) p = (T*)((char*)p - (char*)o + (char*)n); }
struct SeqData { char* BoneName; void* T14; void* T20; void* T24; void* T28; int outOfLine;
    void RelocateData(void* o, void* n) { ++outOfLine; RelocateDataInline(o, n); }
    void RelocateDataInline(void* o, void* n) { RelocatePointer(BoneName, o, n); RelocatePointer(T14, o, n);
        RelocatePointer(T20, o, n); RelocatePointer(T24, o, n); RelocatePointer(T28, o, n); } };
struct Seq { SeqData* Data; short ParentIndex; bool Shared; };
struct Anim;
struct Node { Node* next; Node* prev; Anim* item; };
struct Anim {
    SeqData* MovementTrackData; unsigned long SequenceCount; Seq* Sequences; unsigned long SequenceDataBufferSize;
    SeqData* SequenceDataBuffer; bool* BoneMask; bool* RigidBoneMask; float* RigidBonePositions; Node* InstanceList;
    void RelocateData(long, void* o, void* n, long) {
        if (MovementTrackData) MovementTrackData->RelocateData(o, n);
        RelocatePointer(Sequences, o, n); RelocatePointer(SequenceDataBuffer, o, n); RelocatePointer(BoneMask, o, n);
        RelocatePointer(RigidBoneMask, o, n); RelocatePointer(RigidBonePositions, o, n);
        for (unsigned long i = 0; i < SequenceCount; ++i) { Seq& s = Sequences[i]; if (!s.Shared) RelocatePointer(s.Data, o, n); }
        for (unsigned long j = 0; j < SequenceDataBufferSize; ++j) SequenceDataBuffer[j].RelocateDataInline(o, n);
        for (Node* node = InstanceList->next; node != InstanceList; node = node->next) {
            Anim* inst = node->item;
            if (inst) for (unsigned long k = 0; k < inst->SequenceCount; ++k) { Seq& s = inst->Sequences[k]; if (s.Shared) RelocatePointer(s.Data, o, n); }
        }
    }
};
static char oldBlock[256], newBlock[256];
int main() {
    // Old block holds: sequences (2) at +0, data buffer (1) at +64. Copy to new block, then relocate.
    Seq* seqs = (Seq*)oldBlock; SeqData* buf = (SeqData*)(oldBlock + 64);
    seqs[0].Data = buf; seqs[0].Shared = false; seqs[1].Data = (SeqData*)0; seqs[1].Shared = false;
    buf[0].BoneName = oldBlock + 200; buf[0].T14 = 0; buf[0].T20 = oldBlock + 210; buf[0].T24 = 0; buf[0].T28 = 0; buf[0].outOfLine = 0;
    for (int i = 0; i < 256; ++i) newBlock[i] = oldBlock[i];
    Node head; Node n1; Anim inst; Seq instSeq; instSeq.Data = buf; instSeq.Shared = true; inst.SequenceCount = 1; inst.Sequences = &instSeq;
    head.next = &n1; head.prev = &n1; n1.next = &head; n1.prev = &head; n1.item = &inst;
    Anim a; a.MovementTrackData = 0; a.SequenceCount = 2; a.Sequences = seqs; a.SequenceDataBufferSize = 1; a.SequenceDataBuffer = buf;
    a.BoneMask = 0; a.RigidBoneMask = (bool*)(oldBlock + 100); a.RigidBonePositions = 0; a.InstanceList = &head;
    a.RelocateData(0, oldBlock, newBlock, 256);
    Seq* ns = (Seq*)newBlock; SeqData* nb = (SeqData*)(newBlock + 64);
    if (a.Sequences != ns || a.SequenceDataBuffer != nb || a.BoneMask != 0 || a.RigidBoneMask != (bool*)(newBlock + 100)
        || ns[0].Data != nb || ns[1].Data != 0 || nb[0].BoneName != newBlock + 200 || nb[0].T20 != newBlock + 210 || nb[0].T14 != 0
        || instSeq.Data != nb || buf[0].outOfLine != 0) { std::printf("00a4e960_TEST FAIL\n"); return 1; }
    std::printf("00a4e960_TEST PASS\n"); return 0;
}
