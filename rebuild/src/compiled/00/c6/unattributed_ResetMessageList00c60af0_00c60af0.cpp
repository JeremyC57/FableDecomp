// _unattributed::ResetMessageList_00c60af0(CMessageNode**) -- clear the message list, post one fresh message when the state is valid, hand back the list head
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C60AF0, length 47 bytes (script module, 0x00C09000..).
// Callees: ClearMessages @ 0x00C60410 (walks the list at +0x3c4 through the +0x94 links freeing each node; manifest
// "CDrawGuildSeal::ClearMessages" is propagated), the message factory @ 0x00C60A30 (allocates a 0x98-byte node of type 2
// when the mode at +0x3d0 is 1 and links it at +0x3c4; manifest "CShadowRendererSceneData_OnAnimationLoaded" is propagated).
// The manifest label "CCreatureAction_BanditKingLunge::OnAnimationLoaded" is rejected: a creature action is 0x12c bytes
// at most (PDB) while this `this` reaches +0x3d0; the owner is an unidentified script-module object with a message list.
// Shape: the result local is declared (and zeroed in callee-saved edi) ahead of the ClearMessages call, the state at
// +0x3b4 is tested on its sign bit (`test eax,eax ; js`: a `>= 0` relational compiles to `jl`, the mask form to `js`), the list head
// re-read after the factory call and stored through the out-pointer, the node pointer returned. `ret 4`: one argument.
struct CMessageNode;

struct CMessageOwner {
    unsigned char _pad[0x3b4];
    unsigned long m_state;                  // +0x3b4: sign bit set = no message to post
    unsigned char _pad3b8[0x0c];            // +0x3b8
    CMessageNode* m_pMessages;              // +0x3c4 list head
    void ClearMessages();                   // 0x00C60410
    CMessageNode* PostMessage();            // 0x00C60A30
    CMessageNode* ResetMessageList_00c60af0(CMessageNode** ppHead);
};

CMessageNode* CMessageOwner::ResetMessageList_00c60af0(CMessageNode** ppHead)
{
    CMessageNode* pPosted = 0;                 // zeroed in edi BEFORE ClearMessages (declared first)
    ClearMessages();
    if (!(m_state & 0x80000000))
        pPosted = PostMessage();
    *ppHead = m_pMessages;
    return pPosted;
}