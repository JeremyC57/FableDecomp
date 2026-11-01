// naming wave 20 (chunk 687): the +0x80 object is a libpk CPKArray (GetCapacity 0x00C61BC0, descriptive); the other member names stay descriptive
class CPKArray
{
public:
    unsigned long GetCapacity() const;             // 0x00C61BC0
    void GetFramesRemaining(unsigned long key);
};
class InputEventOwner
{
public:
    unsigned char pad00[0x80];
    CPKArray event80;
    void InputEvent_CheckKeyAndGetFrames(unsigned long key);
};
void InputEventOwner::InputEvent_CheckKeyAndGetFrames(unsigned long key)
{
    if (key < event80.GetCapacity())
        event80.GetFramesRemaining(key);
}