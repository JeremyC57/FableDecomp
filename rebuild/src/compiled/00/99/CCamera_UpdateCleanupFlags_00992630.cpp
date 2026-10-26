// CCamera::UpdateCleanupFlags -- retail row 0x00992630 is a counted-pointer/string copy helper on a CCamera subobject (manifest label CGameCameraManager::UpdateCleanupFlags is propagated incorrectly); retail length 0xb3. Two counted pointer pairs own their Info blocks; four following CCharString members use operator= at 0x0099efb0. A block has refs at +0, destruction callback at +4, and pointee at +8.
// Grade: RELOCATION_MATCH -- verified behavior PASS under g++ and VC7.1.
struct CCameraInfo { long refs; void (__fastcall *destroy)(void *); void *object; };
struct CCountedPointer { void *data; CCameraInfo *info; };
class CCharString { public: void *data; CCharString *operator=(CCharString const&); };
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