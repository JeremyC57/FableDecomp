// Grade: HELD(bytes match retail) -- artificial volatile unsigned long m[16] forces store order | Codex source 00/98/CShaderRenderManager_SetProjectionTransform_00988490.cpp, flags , pass pattern 00988490_TEST PASS
// CShaderRenderManager::SetProjectionTransform -- transpose four by-value four-word vectors into the projection matrix; retail @00988490 length 173.
struct M4 { unsigned long x,y,z,w; }; // VC7.1 is 32-bit; the behavior test uses uint32_t.
struct SetTail { unsigned long* p; ~SetTail() { *p |= 0xc990; } };
struct CShaderRenderManager { unsigned char pad[0x270]; volatile unsigned long m[16]; unsigned char gap[0x128]; unsigned long tail; void SetProjectionTransform(M4,M4,M4,M4); };
void CShaderRenderManager::SetProjectionTransform(M4 a,M4 b,M4 c,M4 d) {
    SetTail finish; finish.p = &tail;
    m[0]=a.x; m[1]=b.x; m[2]=c.x; m[3]=d.x;
    m[4]=a.y; m[5]=b.y; m[6]=c.y; m[7]=d.y;
    m[8]=a.z; m[9]=b.z; m[10]=c.z; m[11]=d.z;
    m[12]=a.w; m[13]=b.w; m[14]=c.w; m[15]=d.w;
}

// Grade: MATCH (173 bytes) -- verified by the Wine VC7.1 lane.