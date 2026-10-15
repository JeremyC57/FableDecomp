// CTCMeshAutomaticEntityCreator::HandleMessage(CTCMessage*) -- forwards a message of type 5 (mesh replaced) to CatchMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072E560, length 33 bytes.
// Callees: the message's virtual GetType() (slot 1, +4); CTCMeshAutomaticEntityCreator::CatchMessage(msg) @ 0x0072E4C0.
// Shape: the message pointer is loaded into esi before `this` is saved in edi (argument first), the type compared
// with an immediate, the forward pushes the same esi.
struct CTCMessage {
    virtual void v00();
    virtual long GetType() const;                   // slot 1 (+0x04)
};

struct CTCBase_0072e560 {
    virtual void v00();
    unsigned char _pad04[8];
};

struct CTCMeshAutomaticEntityCreator : CTCBase_0072e560 {
    void CatchMessage(CTCMessage* msg);             // 0x0072E4C0
    void HandleMessage(CTCMessage* msg);
};

void CTCMeshAutomaticEntityCreator::HandleMessage(CTCMessage* msg)
{
    if (msg->GetType() == 5)
        CatchMessage(msg);
}
