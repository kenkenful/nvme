#ifndef __NVME_MOF_DATA_H__
#define __NVME_MOF_DATA_H__

DEFINE_GUID(NVMe_QueryDevInfo_GUID,
    0x0f5937e9, 0xfeab, 0x40ff, 0xbb, 0xae, 0x9c, 0x22, 0x02, 0xc5, 0x4b, 0x1f);

DEFINE_GUID(NVMe_Method_GUID,
    0x0bd0ee0c, 0xbb49, 0x45f4, 0xa7, 0x98, 0x80, 0xd2, 0x21, 0xbe, 0x3d, 0xe1);

typedef struct _NVMe_QueryDevInfo
{
    ULONG maxDataXferSize;
    ULONG numberOfNamespaces;
    struct
    {
        ULONG Id;
        ULONGLONG u64;
    } Data;
} NVMe_QueryDevInfo, *PNVMe_QueryDevInfo;

#define NVMe_QueryDevInfo_SIZE sizeof(NVMe_QueryDevInfo)

typedef enum _NVMe_MethodId
{
    GetControllerInfo = 1,
    GetNameSpaceInfo = 2
} NVMe_MethodId;

typedef struct _GetControllerInfo_OUT
{
    USHORT pciVendorId;
    USHORT pciSsVendId;
} GetControllerInfo_OUT, *PGetControllerInfo_OUT;

#define GetControllerInfo_OUT_SIZE sizeof(GetControllerInfo_OUT)

typedef struct _GetNameSpaceInfo_IN
{
    ULONG lunId;
} GetNameSpaceInfo_IN, *PGetNameSpaceInfo_IN;

#define GetNameSpaceInfo_IN_SIZE sizeof(GetNameSpaceInfo_IN)

typedef struct _GetNameSpaceInfo_OUT
{
    ULONGLONG nSize;
    ULONGLONG nCap;
} GetNameSpaceInfo_OUT, *PGetNameSpaceInfo_OUT;

#define GetNameSpaceInfo_OUT_SIZE sizeof(GetNameSpaceInfo_OUT)

#endif /* __NVME_MOF_DATA_H__ */
