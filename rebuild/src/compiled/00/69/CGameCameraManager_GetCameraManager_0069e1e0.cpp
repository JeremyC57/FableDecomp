// CGameCameraManager::GetCameraManager(void**) -- stores `this` through the out-pointer and returns true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069E1E0, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; mov dword ptr [eax], ecx ; mov al, 1 ; ret 4
// Tiny-body shape `GetSelf` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CGameCameraManager {
    bool GetCameraManager(void** out);
};

bool CGameCameraManager::GetCameraManager(void** out)
{
    *out = this;
    return true;
}