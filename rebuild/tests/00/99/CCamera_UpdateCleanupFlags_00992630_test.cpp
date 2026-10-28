#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __fastcall
#endif
/* Candidate implementation exercised directly by this test. */
// CCamera::UpdateCleanupFlags -- retail row 0x00992630 is a counted-pointer/string copy helper on a CCamera subobject (manifest label CGameCameraManager::UpdateCleanupFlags is propagated incorrectly); retail length 0xb3. Two counted pointer pairs own their Info blocks; four following CCharString members use operator= at 0x0099efb0. A block has refs at +0, destruction callback at +4, and pointee at +8.
// Grade: RELOCATION_MATCH -- verified behavior PASS under g++ and VC7.1.
struct CCameraInfo { long refs; void (__fastcall *destroy)(void *); void *object; };
struct CCountedPointer { void *data; CCameraInfo *info; };
class CCharString { public: void *data; const CCharString& operator=(CCharString const&); };
class CCamera { public:
    CCountedPointer first;
    CCountedPointer second;
    CCharString text0, text1, text2, text3;
    CCamera *UpdateCleanupFlags(CCamera const&);
};
CCamera *CCamera::UpdateCleanupFlags(CCamera const& rhs)
{
    void *firstData = rhs.first.data;
    CCameraInfo *firstInfo = rhs.first.info;
    if (first.info != firstInfo) {
        if (first.info) {
            --first.info->refs;
            if (first.info->refs == 0) { first.info->destroy(first.info->object); ::operator delete(first.info); }
        }
        first.data = firstData; first.info = firstInfo;
        if (firstInfo) ++firstInfo->refs;
    }
    void *secondData = rhs.second.data;
    CCameraInfo *secondInfo = rhs.second.info;
    if (second.info != secondInfo) {
        if (second.info) {
            --second.info->refs;
            if (second.info->refs == 0) { second.info->destroy(second.info->object); ::operator delete(second.info); }
        }
        second.data = secondData; second.info = secondInfo;
        if (secondInfo) ++secondInfo->refs;
    }
    text0 = rhs.text0; text1 = rhs.text1; text2 = rhs.text2; text3 = rhs.text3;
    return this;
}

struct CountedObject { int *destroyed; };
static void __fastcall DestroyObject(void *p) { ++*static_cast<CountedObject *>(p)->destroyed; }
struct SharedText { long refs; char value[16]; };
static int copyCount = 0;
static int copyOrder[16];
static CCharString *tracked[4];
const CCharString& CCharString::operator=(CCharString const& rhs)
{
    int slot = -1;
    for (int i=0; i<4; ++i) if (tracked[i] == this) slot = i;
    copyOrder[copyCount++] = slot;
    if (rhs.data != data) {
        SharedText *oldText = static_cast<SharedText *>(data);
        if (oldText && --oldText->refs == 0) delete oldText;
        data = rhs.data;
        SharedText *newText = static_cast<SharedText *>(data);
        if (newText) ++newText->refs;
    }
    return *this;
}
static CCameraInfo *NewInfo(void *object) {
    CCameraInfo *p = new CCameraInfo;
    p->refs = 1; p->destroy = DestroyObject; p->object = object;
    return p;
}
static SharedText *NewText(char const *s) {
    SharedText *p = new SharedText; p->refs = 1;
    std::strncpy(p->value, s, sizeof(p->value)); p->value[sizeof(p->value)-1] = 0;
    return p;
}
int main() {
    int destroyed = 0;
    CountedObject oldObject = { &destroyed };
    CCamera dst, src;
    dst.first.data = (void*)1; dst.first.info = NewInfo(&oldObject);
    dst.second.data = (void*)2; dst.second.info = 0;
    src.first.data = (void*)3; src.first.info = NewInfo(&oldObject);
    src.second.data = (void*)4; src.second.info = NewInfo(&oldObject);
    tracked[0] = &dst.text0; tracked[1] = &dst.text1;
    tracked[2] = &dst.text2; tracked[3] = &dst.text3;
    dst.text0.data = NewText("old0"); dst.text1.data = NewText("old1");
    dst.text2.data = NewText("old2"); dst.text3.data = NewText("old3");
    src.text0.data = NewText("new0"); src.text1.data = NewText("new1");
    src.text2.data = NewText("new2"); src.text3.data = NewText("new3");
    CCamera *returned = dst.UpdateCleanupFlags(src);
    if (returned != &dst || dst.first.info != src.first.info || dst.first.data != src.first.data ||
        dst.second.info != src.second.info || dst.second.data != src.second.data ||
        dst.first.info->refs != 2 || dst.second.info->refs != 2 || destroyed != 1 ||
        std::strcmp(static_cast<SharedText *>(dst.text0.data)->value,"new0") ||
        std::strcmp(static_cast<SharedText *>(dst.text1.data)->value,"new1") ||
        std::strcmp(static_cast<SharedText *>(dst.text2.data)->value,"new2") ||
        std::strcmp(static_cast<SharedText *>(dst.text3.data)->value,"new3") ||
        copyCount != 4 || copyOrder[0] != 0 || copyOrder[1] != 1 || copyOrder[2] != 2 || copyOrder[3] != 3) {
        std::printf("00992630_TEST FAIL\n"); return 1;
    }
    copyCount = 0;
    dst.UpdateCleanupFlags(src);
    dst.UpdateCleanupFlags(dst);
    if (dst.first.info->refs != 2 || dst.second.info->refs != 2 || copyCount != 8 ||
        copyOrder[0] != 0 || copyOrder[1] != 1 || copyOrder[2] != 2 || copyOrder[3] != 3 ||
        copyOrder[4] != 0 || copyOrder[5] != 1 || copyOrder[6] != 2 || copyOrder[7] != 3) {
        std::printf("00992630_TEST FAIL\n"); return 1;
    }
    std::printf("00992630_TEST PASS\n"); return 0;
}